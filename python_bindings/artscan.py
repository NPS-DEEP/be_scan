#!/usr/bin/env python

from argparse import ArgumentParser
import os
import be_scan

class TopLevelUncompression():
    """Provides uncompressed buffers from the top level, specifically,
       from the file.  Uses optimization to avoid re-reading file data."""

    def __init__(self, f, filesize, min_cbuf_size):
        """Start a compression buffer reader for the open file"""
        # file and buffer attributes
        self.f = f
        self.filesize = filesize
        self.min_cbuf_size = min_cbuf_size

        # the currently active buffer, cbuf
        self.cbuf = ""
        self.stream_offset = 0;

    def _maybe_read(self, offset):
        # as an optimization we over-read by 2X and we do not rea-read if
        # the offset is within the lower 1X half.
        end_offset = stream_offset + len(self.cbuf)
        if offset >= stream_offset and (
                         end_offset >= offset + self.min_cbuf_size or
                         end_offset == self.filesize):
            # no need to read
            return

        # check for EOF
        if offset == self.filesize:
            # no more
            self.cbuf = ""
            self.stream_offset = offset;
            return

        # read
        size = self.min_cbuf_size * 2
        if offset + size > self.filesize:
            size = self.filesize - offset

        # read size bytes into the compression buffer
        f.seek(offset)
        self.cbuf = f.read(size)
        self.stream_offset = offset

    # uncompress top level buffer
    def uncompress(self, stream_offset):
        """Get the uncompressed buffer at the top level stream offset."""
        self._maybe_read(stream_offset)
        buffer_offset = stream_offset - self.stream_offset
        if buffer_offset >= len(self.cbuf):
            # invalid input
            raise ValueError("buffer offset is out of range")

        uncompressed = uncompressor.uncompress(self.cbuf, buffer_offset)
        if uncompressed.status != "":
            print("uncompression failure %s at offset %u" %
                                 (uncompressed.status, buffer_offset))
        return uncompressed.buffer

# recurse
def recurse(uncompressed_buffer, recursion_prefix, depth):
    """Recursively scan and uncompress until max depth."""

    # runtime status
    print("recursion prefix %s depth %d" % (recursion_prefix, depth))

    # verbose status
    if args.verbose:
        print("%s\n" % be_scan.escape(uncompressed_buffer))

    # open a scanner
    scanner = be_scan.scanner_t(scan_engine)
    scanner.scan_setup(filename, recursion_prefix)

    # scan
    status = scanner.scan(0, "", uncompressed_buffer)
    if status != "":
        raise ValueError(status)
    status = scanner.scan_fence_finalize(0, "", uncompressed_buffer)
    if status != "":
        raise ValueError(status)

    # consume artifacts
    while not scanner.empty():
        artifact = scanner.get();
        print(artifact.to_string())

        # manage uncompression
        if depth < MAX_RECURSION_DEPTH and \
                        (artifact.artifact_class == "zip" or \
                        artifact.artifact_class == "gzip"):

            # uncompress from previously uncompressed buffer
            next_uncompressed_buffer = uncompressor.uncompress(
                                     uncompressed_buffer, artifact.offset)
            next_recursion_prefix = "%s%d-%s-" % (artifact.recursion_prefix,
                         artifact.offset, artifact.artifact_class.upper())
            recurse(next_uncompressed_buffer, next_recursion_prefix, 1)

# main
if __name__=="__main__":

    # parse filename
    parser = ArgumentParser(prog='artscan_all.py',
             description="Scan a file for forensic artifacts.")
    parser.add_argument("filename", help="file to scan", type=str)

    # parse scan range and scan interval
    parser.add_argument("-b", "--begin",
                        help="the offset in the file to begin scanning",
                        default=0, type=int)
    parser.add_argument("-e", "--end",
                        help="the offset in the file to end scanning",
                        default=0, type=int)
    parser.add_argument("-p", "--scan_partition_interval",
                        help="the increment size of the scan interval",
                        default=2**27, type=int)  # 2^27 = 128MiB = 134217728=

    # uncompression
    parser.add_argument("-c", "--min_compressed_size",
                        help="the minimum size of the compression buffer",
                        default=2**21, type=int)  # 2^21 = 2MiB = 2097152

    # parse scanner selection, depth, verbose
    default_scanners = be_scan.available_scanners()
    parser.add_argument("-s", "--enable",
                        help="enable specific scanners, default: '%s'" %
                        default_scanners, default = default_scanners, type=str)
    parser.add_argument("-r", "--recursion_depth",
                        help="recursively scan into decompressed regions",
                        default=7, type=int, choices=[1, 2, 3, 4, 5, 6, 7])
    parser.add_argument("-v", "--verbose",
                        help="show all the bytes being scanned",
                        action="store_true")

    # args
    args = parser.parse_args()
    filename = args.filename
    filesize = os.stat(filename).st_size
    scan_interval = args.scan_partition_interval
    selected_scanners = args.enable
    min_compressed_size = args.min_compressed_size
    begin = args.begin
    if begin > filesize:
        begin = filesize
    end = args.end
    if end == 0 or end > filesize:
        end = filesize
    MAX_RECURSION_DEPTH = args.recursion_depth

    # open f for reading intervals of binary data
    # and open f2 for reading compressed data to uncompress
    with open(filename, mode='rb') as f, \
                             open(filename, mode='rb') as f2:

        # set up the scan stream
        previous_buffer = ""
        f.seek(begin)
        stream_offset = begin

        # open the scanner
        scan_engine = be_scan.scan_engine_t(selected_scanners)
        scanner = be_scan.scanner_t(scan_engine)
        scanner.scan_setup(filename, "")

        # open the uncompressor
        uncompressor = be_scan.uncompressor_t()

        # set up for top level uncompression
        top_level_uncompression = TopLevelUncompression(f2, filesize,
                                                min_compressed_size)

        # loop through the file one interval at a time
        while stream_offset != end:
            # read into current_buffer
            read_size = scan_interval
            if stream_offset + read_size > filesize:
                read_size = filesize - stream_offset
            current_buffer = f.read(read_size)
            if len(current_buffer) == 0:
                raise ValueError("unexpected end of file")

            # runtime status
            print("File %s stream offset %d, buffer size %d" %
                  (filename, stream_offset, len(current_buffer)))

            # verbose status
            if args.verbose:
                print("%s\n" % be_scan.escape(current_buffer))

            # scan
            status = scanner.scan(stream_offset,
                                  previous_buffer, current_buffer)
            if status != "":
                raise ValueError(status)
            status = scanner.scan(stream_offset, "", current_buffer)
            if status != "":
                raise ValueError(status)
            status = scanner.scan_fence_finalize(stream_offset,
                                                 "", current_buffer)
            if status != "":
                raise ValueError(status)

            # consume artifacts
            while not scanner.empty():
                artifact = scanner.get();
                print(artifact.to_string())

                # manage uncompression
                if MAX_RECURSION_DEPTH > 1 and \
                                (artifact.artifact_class == "zip" or \
                                artifact.artifact_class == "gzip"):

                    # uncompress from file
                    uncompressed_buffer = top_level_uncompression.uncompress(
                                                    artifact.offset)
                    recursion_prefix = "%d-%s-" % (artifact.offset,
                                         artifact.artifact_class.upper())
                    recurse(uncompressed_buffer, recursion_prefix, 1)

            stream_offset += read_size

    print("Done.")


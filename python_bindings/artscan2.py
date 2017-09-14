#!/usr/bin/env python

# To install futures for Python2.7, try something like:
# sudo /usr/bin/pip2.7 install futures

from argparse import ArgumentParser
import os
from concurrent import futures
import md5
import be_scan
from file_reader import FileReader

# get_args
def get_args():
    # argument parser
    parser = ArgumentParser(prog='artscan_multi.py', description=
                  "Scan a file for forensic artifacts, multiple processes.")
    parser.add_argument("filename", help="file to scan", type=str)

    # enabled scanners
    default_scanners = be_scan.available_scanners()
    parser.add_argument("-e", "--enabled_scanners",
                        help="enable specific scanners, default: '%s'" %
                        default_scanners, default = default_scanners, type=str)

    # recursion depth
    parser.add_argument("-d", "--recursion_depth",
                        help="recursively scan into decompressed regions",
                        default=7, type=int, choices=[1, 2, 3, 4, 5, 6, 7])

    # verbosity
    parser.add_argument("-v", "--verbose",
                        help="show runtime status, 0=none, 1=scan status, 2=scan status plus all the bytes being scanned",
                        default=0, type=int, choices=[0, 1, 2])

    # return args
    return parser.parse_args()

# set artifact text for compression artifact
def set_compression_text(artifact, uncompressed):
    if uncompressed.status:
        # uncompression failure so put failure into artifact text
        print("uncompression failure '%s' at file offset %s%u" %
                 (uncompressed.status,
                  artifact.recursion_prefix,
                  artifact.offset))
        artifact.artifact = uncompressed.status
    else:
        # valid so put MD5 into artifact text
        artifact.artifact = md5.new(uncompressed.buffer).hexdigest()
        artifact.context = "%d" % len(uncompressed.buffer)

# recurse
def recurse(uncompressed_buffer, recursion_prefix, depth, formatted_artifacts,
            uncompressor, max_depth, verbose):
    """Recursively scan and uncompress until max depth."""

    # runtime status
    if verbose >= 1:
        print("# Processing %s count %d depth %d..." % (recursion_prefix,
                                        len(uncompressed_buffer), depth))
    if verbose >= 2:
        print("# %s\n" % be_scan.escape(uncompressed_buffer))

    # open a scanner
    artifacts = be_scan.artifacts_t()
    scanner = be_scan.scanner_t(scan_engine, artifacts)
    scanner.scan_setup(args.filename, recursion_prefix)

    # scan
    status = scanner.scan_final(0, "", uncompressed_buffer)
    if status:
        raise ValueError(status)

    # consume recursed artifacts
    while not artifacts.empty():
        artifact = artifacts.get()

        # prepare for zip or gzip
        if artifact.artifact_class == "zip" or \
                        artifact.artifact_class == "gzip":

            # uncompress
            uncompressed = uncompressor.uncompress(
                                 uncompressed_buffer, artifact.offset)
            if not uncompressed.status and not uncompressed.buffer:
                # no error and nothing uncompressed so disregard artifact
                continue

            set_compression_text(artifact, uncompressed)

            # skip popular useless uncompressed data
            if artifact.artifact == "8da7a0b0144fc58332b03e90aaf7ba25":
                continue

        # prepare for other artifact types as needed
        # none.

        # consume this artifact
        formatted_artifacts.append(artifact.to_string())
 
        # manage recursion
        if (artifact.artifact_class == "zip" or \
                        artifact.artifact_class == "gzip") and \
                        depth <= max_depth:

            # calculate next recursion prefix
            next_recursion_prefix = "%s-%d-%s" % (
                           artifact.recursion_prefix,
                           artifact.offset,
                           artifact.artifact_class.upper())

            # recurse
            recurse(uncompressed.buffer, next_recursion_prefix, depth + 1,
                    uncompressor, max_depth, verbose)

# scan_range
def scan_range(filename, offset, page_size, margin_size, max_depth, verbose):
    with open(filename, 'rb') as f:
        f.seek(offset)
        buffer1 = f.read(page_size)
        buffer2 = f.read(margin_size)

        # print runtime status
        if verbose >= 1:
            # print buffer offset
            print("# File: '%s', offset: %d, size: %d, overflow size: %d" % (
                      filename, offset, len(buffer1), len(buffer2)))
        if verbose >= 2:
            # print buffer contents
            print("# previous: '%s'\nCurrent: '%s'" %
                           (be_scan.escape(buffer1),
                            be_scan.escape(buffer2)))

        # the uncompressor this process will use, if needed
        uncompressor = None

        # the formatted artifacts
        formatted_artifacts = list()

        # create scanner
        artifacts = be_scan.artifacts_t()
        scanner = be_scan.scanner_t(scan_engine, artifacts)
        scanner.scan_setup(args.filename, "")

        # scan buffer
        status = scanner.scan_stream(offset, "", buffer1)
        if status:
            raise ValueError(status)

        # scan fence
        status = scanner.scan_fence_final(offset, buffer1, buffer2)
        if status:
            raise ValueError(status)

        # consume
        while not artifacts.empty():
            artifact = artifacts.get()

            # check for compression
            if artifact.artifact_class == "zip" or \
                            artifact.artifact_class == "gzip":

                # get uncompressor ready
                if uncompressor == None:
                    uncompressor = be_scan.uncompressor_t()

                # get buffer to uncompress
                buffer1_offset = artifact.offset - offset
                if buffer1_offset + margin_size > len(buffer1):
                    # use margin size
                    buffer3 = buffer1[buffer1_offset:] + buffer2

                    # uncompress
                    uncompressed = uncompressor.uncompress(buffer3, 0)

                else:
                    # use buffer1
                    uncompressed = uncompressor.uncompress(buffer1,
                                                       buffer1_offset)

                # no error and nothing uncompressed so disregard this artifact
                if not uncompressed.status and not uncompressed.buffer:
                    continue

                # set artifact text for this uncompression
                set_compression_text(artifact, uncompressed)

                # skip popular useless uncompressed data
                if artifact.artifact == "8da7a0b0144fc58332b03e90aaf7ba25":
                    continue

            # prepare for other artifact types as needed
            # none.

            # consume this artifact
            formatted_artifacts.append(artifact.to_string())
 
            # manage recursion
            if (artifact.artifact_class == "zip" or \
                        artifact.artifact_class == "gzip") and \
                        max_depth >= 1:

                # calculate recursion prefix for first recursion depth
                next_recursion_prefix = "%d-%s" % (
                           artifact.offset,
                           artifact.artifact_class.upper())

                # recurse
                recurse(uncompressed.buffer, next_recursion_prefix, 1,
                        uncompressor, formatted_artifacts, max_depth, verbose)

        # return the accumulated list of formatted artifacts
        return formatted_artifacts

# main
if __name__=="__main__":

    args = get_args()

    # file size
    filesize = os.stat(args.filename).st_size

    # step
    page_size = 2**27     # 2^27 = 128MiB = 134217728
    margin_size = 2**20   # 2^20 = 1MiB = 1,048,576

    # the global scan engine
    scan_engine = be_scan.scan_engine_t(args.enabled_scanners)

    # parameters for scan_range
    range_parameters = list()
    for i in range(0, filesize, page_size):
        range_parameters.append((args.filename, i, page_size, margin_size,
                                 args.recursion_depth, args.verbose))

    # run the scan_range job on each parameter tuple
    executor = futures.ProcessPoolExecutor() # default to use all CPUs
    # future_objects = executor.submit(scan_range, range_parameters)
    future_objects = [executor.submit(scan_range, *range_parameter_tuple)
                      for range_parameter_tuple in range_parameters]

    # asynchronously wait for and consume formatted artifacts
    for future in futures.as_completed(future_objects):
        formatted_artifacts = future.result()
        for formatted_artifact in formatted_artifacts:
            print(formatted_artifact)

    print("# Done.")


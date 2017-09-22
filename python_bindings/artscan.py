#!/usr/bin/env python

from argparse import ArgumentParser
import os
import md5
import be_scan
from file_reader import FileReader

# load_custom_regex
def load_custom_regex(regex_file):
    if not os.path.exists(regex_file):
        print("Error: custom_regex file does not exist.")
        exit(1)
    with open(regex_file, 'r') as f:
        for regex_pattern in f:
            print("pattern '%s'" % regex_pattern)
            regex_pattern = regex_pattern.rstrip("\n")
            print("pattern stripped '%s'" % regex_pattern)
            be_scan.add_custom_regex_pattern(regex_pattern)

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
def recurse(uncompressed_buffer, recursion_prefix, depth):
    """Recursively scan and uncompress until max depth."""

    # runtime status
    if args.verbose >= 1:
        print("# Processing %s count %d depth %d..." % (recursion_prefix,
                                        len(uncompressed_buffer), depth))
    if args.verbose >= 2:
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
    while True:
        artifact = artifacts.get()
        if artifact.blank():
            break

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

        # print the artifact
        print(artifact.to_string())

        # manage recursion
        if (artifact.artifact_class == "zip" or \
                        artifact.artifact_class == "gzip") and \
                        depth <= MAX_RECURSION_DEPTH:

            # calculate next recursion prefix
            next_recursion_prefix = "%s-%d-%s" % (
                           artifact.recursion_prefix,
                           artifact.offset,
                           artifact.artifact_class.upper())

            # recurse
            recurse(uncompressed.buffer, next_recursion_prefix, depth + 1)

# consume top-level artifacts
def consume_top_level_artifacts(scanner):

    while True:
        # get cached artifact from queue
        artifact = artifacts.get()
        if artifact.blank():
            break

        # prepare for zip or gzip
        if artifact.artifact_class == "zip" or \
                        artifact.artifact_class == "gzip":

            # uncompress
            uncompressed = uncompressor.uncompress(file_reader.chunk,
                              artifact.offset - file_reader.stream_offset)

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

        # print the artifact
        print(artifact.to_string())

        # manage recursion
        if (artifact.artifact_class == "zip" or \
                        artifact.artifact_class == "gzip") and \
                        MAX_RECURSION_DEPTH >= 1:

            # calculate recursion prefix for first recursion depth
            next_recursion_prefix = "%d-%s" % (
                           artifact.offset,
                           artifact.artifact_class.upper())

            # recurse
            recurse(uncompressed.buffer, next_recursion_prefix, 1)

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
                        help="show runtime status, 0=none, 1=scan status, 2=scan status plus all the bytes being scanned",
                        default=0, type=int, choices=[0, 1, 2])
    parser.add_argument("-g", "--custom_regex_file",
                        help="a file containing custom regular expressions to grep for",
                        type=str)

    # args
    args = parser.parse_args()
    selected_scanners = args.enable
    MAX_RECURSION_DEPTH = args.recursion_depth

    # load custom regex
    if args.custom_regex_file:
        load_custom_regex(args.custom_regex_file)

    # file reader
    file_reader = FileReader(args.filename, args.begin, args.end)

    # runtime status
    file_reader.print_status(args.verbose)

    # the scan engine
    scan_engine = be_scan.scan_engine_t(selected_scanners)

    # the top-level scanner
    artifacts = be_scan.artifacts_t()
    scanner = be_scan.scanner_t(scan_engine, artifacts)
    scanner.scan_setup(args.filename, "")

    # the uncompressor
    uncompressor = be_scan.uncompressor_t()

    # scan buffer intervals from the file reader
    while file_reader.more():

        # scan stream
        status = scanner.scan_stream(file_reader.stream_offset,
                                     file_reader.previous,
                                     file_reader.current)
        if status:
            raise ValueError(status)

        consume_top_level_artifacts(scanner)

        # next
        file_reader.read()

        # runtime status
        file_reader.print_status(args.verbose)

    # capture any remaining open artifacts
    status = scanner.scan_final(file_reader.stream_offset,
                                file_reader.previous,
                                file_reader.current)
    if status:
        raise ValueError(status)

    consume_top_level_artifacts(scanner)


    print("# Done.")


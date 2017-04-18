#!/usr/bin/env python

from argparse import ArgumentParser
import os
import be_scan

# escape unprintable bytes
def escape(data):
    escaped = ""
    for c in data:
        if c < ' ' or c > '~' or c == '\\':
            escaped += "\\x%02X" % ord(c)
        else:
            escaped += c
    return escaped

# main
if __name__=="__main__":

    # parse filename, offset, count
    parser = ArgumentParser(prog='artscan_all.py',
             description="Scan a file for forensic artifacts.")
    parser.add_argument("filename", help="file to scan", type=str)
    parser.add_argument("-s", "--start",
                        help="the start offset in the file",
                        default=0, type=int)
    parser.add_argument("-c", "--count",
                        help="the number of bytes to process",
                        default=0, type=int)
    parser.add_argument("-b", "--break_size",
                        help="the size, in bytes, to split processing into",
                        default=2**27, type=int)  # 2^27 = 128MiB = 134217728
    default_scanners = be_scan.available_scanners()
    parser.add_argument("-e", "--enable",
                        help="enable specific scanners, default: '%s'" %
                        default_scanners, default = default_scanners, type=str)
    parser.add_argument("-v", "--verbose",
                        help="show the bytes being scanned",
                        action="store_true")
    args = parser.parse_args()

    # get inputs, fixing out-of-bounds input
    offset = args.start
    filesize = os.stat(args.filename).st_size
    if offset > filesize:
        offset = filesize
    count = args.count
    if count == 0 or count+offset > filesize:
        count = filesize - offset
    splitsize = args.break_size
    scanners = args.enable

    while (count > 0):

        # open the file for reading binary
        with open(args.filename, mode='rb') as f:
            f.seek(offset)
            if count < splitsize:
                data = f.read(count)
            else:
                data = f.read(splitsize)
            count -= len(data)

            # runtime status
            print("File %s start %d count %d\n" % (args.filename, offset,
                                                   len(data)))
            if args.verbose:
                print("%s\n" % escape(data))

            # scan the data
            scanner = be_scan.be_scan_t(scanners, data, len(data))
            while True:
                artifact = scanner.next()
                if artifact.artifact_class == "":
                    break
                print("artifact at %d: %s: '%s', '%s'" %(
                         offset + artifact.buffer_offset,
                         artifact.artifact_class,
                         escape(artifact.artifact),
                         escape(artifact.context)))

        offset += splitsize

    print("Done.")


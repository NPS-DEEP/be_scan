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
                        help="start offset in file",
                        default=0, type=int)
    parser.add_argument("-c", "--count",
                        help="number of bytes to process",
                        default=0, type=int)
    parser.add_argument("-b", "--break_size",
                        help="size to split processing into",
                        default=2**27, type=int)  # 2^27 = 128MiB = 134217728
    parser.add_argument("-v", "--verbose",
                        help="show the input bytes",
                        action="store_true")
    args = parser.parse_args()

    offset = args.start
    filesize = os.stat(args.filename).st_size
    splitsize = args.break_size

    while (offset < filesize):

        # open the file for reading binary
        with open(args.filename, mode='rb') as f:
            f.seek(offset)
            data = f.read(splitsize)

            # runtime status
            print("File %s start %d count %d\n" % (args.filename, offset,
                                                   len(data)))
            if args.verbose:
                print("%s\n" % escape(data))

            # scan the data
            scanner = be_scan.be_scan_t("email", data, len(data))
            while True:
                artifact = scanner.next_artifact()
                if artifact.artifact_class == "":
                    break
                print("artifact at %d: %s: '%s', '%s'" %(
                         offset + artifact.buffer_offset,
                         artifact.artifact_class,
                         escape(artifact.artifact),
                         escape(artifact.context)))

        offset += splitsize

    print("Done.")


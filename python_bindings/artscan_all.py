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
    args = parser.parse_args()

    filesize = os.stat(args.filename).st_size
    splitsize = 134217728     # 2^27 = 128MiB
    offset = 0
    while (offset < filesize):

        # open the file for reading binary
        with open(args.filename, mode='rb') as f:
            f.seek(offset)
            data = f.read(splitsize)

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
        print("offset: %d" %offset)

    print("Done.")


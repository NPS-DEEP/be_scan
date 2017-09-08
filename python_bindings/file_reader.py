#!/usr/bin/env python

import os
import sys
import be_scan

class FileReader():
    """Provides media image reader functions for artscan.

       Call read() to read another interval of buffer content.
       Access file and range attributes as desired.
       Access buffer variable stream_offset to know your buffer offset.
       Access buffer variables previous and current for scanning.
       Access buffer variable chunk for uncompressing.
       Do not access internal buffer variable next.

       Done when current="".  Also done when stream_offset=end
    """


    def __init__(self, filename, start, stop):
        """Create a reader for reading the range of the file from start
           to one minus stop."""
        # file and range attributes
        self.filename = filename
        self.start = start
        self.stop = stop
        self.interval = 2**20   # 2^20 = 1MiB = 1,048,576

        # buffer variables
        self.stream_offset = 0  # offset to current
        self.previous = ""      # last few hunderd bytes of previous current
        self.current = ""       # current buffer
        self._next = ""         # next buffer
        self.chunk = ""         # current + _next, used for uncompression

        # file object
        self.f = open(filename, mode='rb')
        self.filesize = os.stat(filename).st_size
        if self.filesize == 0:
            sys.exit("Error: File '%s' is empty.  Aborting." % self.filename)
        print("Opened file %s size %d" % (filename, self.filesize))

        # maybe fix start and stop
        if self.start > self.filesize:
            self.start = self.filesize
        if self.stop == 0:
            self.stop = self.filesize
        if not self.start < self.stop:
            sys.exit("Error: Sstart value %d is not less than stop value %d.  Aborting."
                             % (self.start, self.stop))

        # perform first read
        self.stream_offset = self.start
        self.previous = ""
        count = self.interval
        if count > self.stop - self.start:
            count = self.stop - self.start
        self.f.seek(self.stream_offset)
        self.current = self.f.read(count)
        if len(self.current) != count:
            raise ValueError("read error: read %d bytes but %d bytes expected"
                             % (len(self.current), count))
        count2 = self.interval
        if count2 > self.stop - self.start + count:
            count2 = self.stop - self.start - count
        self._next = self.f.read(count2)
        if len(self._next) != count2:
            raise ValueError("read error: read %d bytes but %d bytes expected"
                             % (len(self._next), count2))
        self.chunk = self.current + self._next

    def read(self):
        """Read next interval into stream and set buffer variables accordingly.
        """

        # get count to read
        count = self.interval
        print("read stop %d" % self.stop)
        if self.stream_offset + len(self.current) + len(self._next) + count \
                                                           > self.stop:
            count = self.stop - self.stream_offset - len(self.current) \
                                                   - len(self._next)

        # ripple back buffers
        self.stream_offset += len(self.current)
        self.previous = self.current[-500:] # last few hundred bytes of current
        self.current = self._next
        self._next = self.f.read(count)
        if len(self._next) != count:
            raise ValueError("read error: read %d bytes but %d bytes expected"
                             % (len(self._next), count))
        self.chunk = self.current + self._next

    def more(self):
        """More is available when _next is not empty."""
        return len(self._next) > 0

    def print_status(self, verbose):
        """Print file reader status text."""
        print("File: '%s', stream_offset: %s, count: %s, more: %r" %
                           (self.filename, self.stream_offset,
                            len(self.current), self.more()))
        if (verbose):
            print("previous: '%s'\nCurrent: '%s'" %
                           (be_scan.escape(self.previous),
                            be_scan.escape(self.current)))


libmdf4
=======

Libary to read mdf4-files

Compile & Install
-----------------

~~~
./autogen.sh
./configure
make -j4
sudo make install
~~~

Build requirements:

Ubuntu:
~~~
sudo apt-get install g++ libtool automake libboost-dev
~~~

mdf4-export
-----------

~~~
Usage: mdf4-export [OPTION]... [FILE]
Export data channels from a mdf4 file to csv format.
Mandatory arguments to long options are mandatory for short options too.
  -s, --column-header     print column header with channel name (default)
  -S, --no-column-header  do not print column header with channel name
  -u, --unit-row          print row with channel units (default)
  -U, --no-unit-row       do not print row with channel units
  -d, --delimiter=DELIM   use DELIM instead of ',' for field delimiter
  -r, --row-delimiter=DELIM use DELIM instead of new line for row delimiter
  -g, --data-group=GROUP  use only this data group
  -p, --channel-group=GROUP use only this channel group
  -c, --channels=LIST     print only channels in LIST
  -o, --output=FILE       writes output to FILE (default is stdout)
  -h, --help              print this help
      --version           print current version

The channel LIST is made up of one range, or many ranges separated
by commas. Selected input is written in the same order that it is
read.
Each range is one of:

  N     N'th channel, counted from 0
  N-    from N'th channel to last channel
  N-M   from N'th to M'th (included) channel
  -M    from first to M'th (included) channel

The delimiter DELIM in option -d and -r can have escape sequences.
Supported escape sequences are \\, \t, \n, \r, \', \" and \0.
~~~

mdf4-info
-----------

~~~
Usage: mdf4-info FILE
-h, --help              print this help
    --version           print current version
~~~

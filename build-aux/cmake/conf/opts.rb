#!/usr/bin/env ruby
##
## bootstrap: This file is part of qt-ext.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

require 'pathname'
require 'optparse'

MEDIR, ME = Pathname.new($0).split

class Opts

  def initialize(*deplibs)
    @deplibs = deplibs
    @opts = {
      # Miscellaneous options.
      :verbose => false,
      # Directories options.
      :prefix => '/usr/local',
      :bindir => 'bin',
      :includedir => 'include',
      :libdir => 'lib',
    }
    @optparser = OptionParser.new do |op|

      op.banner = "usage: #{ME} [options]"
      op.summary_indent = '  '
      op.summary_width = 30

      ### Dependencies libraries

      op.separator ''
      op.separator 'Dependencies libraries:'

      @deplibs.each do |x|
        op.on("--with-#{x} DIR", "Use the library `#{x}'.") do |dir|
          check_directory(dir)
          @opts[x] = dir
        end
      end

      op.on('--with-qt DIR', "Path to your Qt install directory.") do |dir|
        @opts[:qtdir] = dir
      end

      ### Directories options

      op.separator ''
      op.separator 'Installation directories:'

      op.on('--prefix DIR', 'Installation prefix.') do |dir|
        @opts[:prefix] = dir
      end

      ### Miscellaneous options

      op.separator ''
      op.separator 'Miscellaneous options:'

      op.on('-v', '--verbose', 'Set cmake verbose mode.') do
        @opts[:verbose] = true
      end

      op.on('-e', '--extra FLAGS', 'Add extra cmake flags') do |flags|
        @opts[:extraflags] = flags
      end

      op.on_tail('-h', '--help', 'Show this message.') do
        puts op
        exit 0
      end
    end
  end

  attr_reader :opts, :deplibs

  def parse(argv)
    @optparser.parse!(argv)
    @opts
  end

  def parse!(argv)
    @optparser.parse!(argv)
    self
  end

  def cmakeflags
    o = ' ' + MEDIR
    o += " \\\n"
    # Miscellaneous
    o += cmake_def("CMAKE_VERBOSE_MAKEFILE", opts[:verbose] ? 'ON' : 'OFF')
    o += cmake_def("CMAKE_INSTALL_PREFIX", opts[:prefix])
    # Configure dependent library paths
    @deplibs.each do |x|
      o += cmake_def("#{x}_ROOT_DIR", @opts[x]) if @opts[x]
    end
    o += ' ' + @opts[:extraflags] + " \\\n" if @opts[:extraflags]
    o
  end

  private

  def check_directory(dir)
    unless File.directory?(dir)
      raise(ArgumentError, "not a directory - '#{dir}'")
    end
  end

  def cmake_def(varname, value, alinea="    ")
    "#{alinea}-D#{varname}='#{value}' \\\n"
  end

end # class Opts

if $0 == __FILE__

  require 'pp'

  go = Opts.new('SDK_REMOTE', 'QSCINTILLA2')
  pp go.parse(ARGV)
  puts go.cmakeflags

end

# --verbose=on/off -> -DCMAKE_VERBOSE_MAKEFILE=ON
# --prefix=DIR -> -DCMAKE_INSTALL_PREFIX=$HOME/install/qt-ext/master
# --sdk-remote=DIR -> -DSDK_REMOTE_PATH=$HOME/install/sdk-remote/1.5
# --qscintilla2=DIR -> -DQSCINTILLA2_PATH=.....
# --help

# echo "   --bindir=<dir>     : -DBIN_INSTALL_DIR:PATH=<dir>"
# echo "   --includedir=<dir> : -DINCLUDE_INSTALL_DIR:PATH=<dir>"
# echo "   --libdir=<dir>     : -DLIB_INSTALL_DIR:PATH=<dir>"

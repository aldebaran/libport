#!/usr/bin/env ruby
##
## conf-cmake.rb: This file is part of qt-ext.
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
require 'shellwords'
require 'fileutils'

MEDIR, ME = Pathname.new($0).split if MEDIR.nil? and ME.nil?

class ConfCmake

  def initialize
    @opts = {
      # Miscellaneous options.
      :verbose => true,
      :color => true,
      :extraflags => [],
      :dry_run => false,
      :cmake => 'cmake',
      # Directories options.
      :prefix => nil,
      # Packages
      :pkg => {},
    }
  end

  attr_reader :opts

  def usage
    STDERR.puts "usage: #{ME} [options]"
  end

  def help
    usage
    STDERR.puts <<EOF

    --with-XXX=PATH     Add package XXX.
    --no-verbose        Set cmake verbose mode off.
-e, --extra=FLAGS       Add extra cmake flags.
    --no-color          Do not create Makefile with colored output.
-n, --dry-run           Do not execute cmake, just print the command.
-h, --help              Show this message.
EOF
end

  def parse(argv)
    if argv.empty?
      usage
      exit 1
    end
    argv.reject do |o|
      case o
      when /^--no-verbose$/
        @opts[:verbose] = false
        true
      when /^--no-color$/
        @opts[:color] = false
        true
      when /^-h$/, /^--help$/
        help
        exit 1
      when /^--extra=(.*)$/
        @opts[:extraflags] << $1
        true
      when /^--with-([^=]+)=(.*)$/
        @opts[:pkg][$1] = $2
        true
      when /^-n$/, /^--dry-run$/
        @opts[:dry_run] = true
        true
      when /^-/, /^([^=]+)=/
        STDERR.puts "unknown option '#{o}'"
        true
      else
        false
      end
    end
  end

  def cmakeflags
    o = ' ' + MEDIR + ' '
    # Miscellaneous
    o += cmake_def("CMAKE_VERBOSE_MAKEFILE", opts[:verbose] ? 'ON' : 'OFF')
    o += cmake_def("CMAKE_INSTALL_PREFIX", opts[:prefix])
    o += cmake_def("CMAKE_COLOR_MAKEFILE", opts[:color] ? 'ON' : 'OFF')
    # Configure dependent library paths
    cmake_search_paths.each do |k, v|
      o += cmake_def(v[:var], v[:list].join(';'))
    end
    o += ' ' + @opts[:extraflags].join(" ")
    o
  end

  def [](key)
    @opts[key]
  end

  def run
    cmd = opts[:cmake] + cmakeflags
    pp_cmd cmd
    if opts[:dry_run]
      0
    else
      system cmd
      status = $?.exitstatus
      if ENV['BUILDFARM']
        FileUtils.ln_s('CMakeCache.txt', 'config.log',
                       :verbose => true, :force => true)
      end
      status
    end
  end

  private

  def cmake_search_paths
    search_path = {
      'lib' => {
        :var => 'CMAKE_LIBRARY_PATH',
        :list => []
      },
      'include' => {
        :var => 'CMAKE_INCLUDE_PATH',
        :list => []
      },
      'bin' => {
        :var => 'CMAKE_PROGRAM_PATH',
        :list => []
      }
    }
    @opts[:pkg].values.each do |v|
      search_path.keys.each do |x|
        path = Pathname.new(v) + x
        search_path[x][:list] << path if path.directory?
      end
    end
    search_path
  end

  def cmake_def(varname, value)
    if value
      "-D#{varname}='#{value}' "
    else
      ''
    end
  end

  def pp_cmd(cmd, alinea='  ')
    a = Shellwords.shellwords(cmd)
    puts a[0..1].join(' ') + ' \\'
    a[2..-1].each { |x| puts alinea + x + (x != a.last ? ' \\' : '') }
  end

end # class ConfCmake

if $0 == __FILE__

  require 'pp'

  go = ConfCmake.new('SDK_REMOTE', 'QSCINTILLA2')
  pp go.parse(ARGV)
  puts go.cmakeflags

end

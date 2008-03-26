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
require 'rbconfig'

MEDIR, ME = Pathname.new($0).split if MEDIR.nil? and ME.nil?

class ConfCmake

  def initialize
    @opts = {
      # Miscellaneous options.
      :verbose => true,
      :color => false,
      :extraflags => [],
      :dry_run => false,
      :cmake => 'cmake',
      :bf_makefile => {
        # make tool to use. By default it is make but it may be nmake.exe.
        :make => 'make',
        # Rules to wrap. If nil is associated then it is a straight forward
        # wrapping: $(MAKE) <rule>
        # Otherwise associated a hash like this:
        #  {
        #    :depends => [ dep, list ],
        #    :actions => "code"
        #  }
        # or simply a string of "code" if there is no dependencies.
        # The buildfarm uses the following rules: all, check-buildfarm, install
        # dist and distcheck.
        :rules => {
          'all' => nil,
          'check-buildfarm' => nil,
          'install' => nil,
          'package' => nil,
          # Used by package rule.
          'preinstall' => nil,
        }
      },
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

    --prefix=PATH       Installation prefix.
    --with-XXX=PATH     Add package XXX.
    --no-verbose        Set cmake verbose mode off.
    --extra=FLAGS       Add extra cmake flags.
    --color             Do not create Makefile with colored output.
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
      when /^--color$/
        @opts[:color] = true
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
      when /^--prefix=(.*)$/
        @opts[:prefix] = $1
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
    o += cmake_def("CMAKE_INSTALL_PREFIX", opts[:prefix]) if opts[:prefix]
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

  def []=(key, val)
    @opts[key] = val
  end

  def run
    cmd = opts[:cmake] + cmakeflags
    pp_cmd cmd
    if opts[:dry_run]
      0
    else
      system cmd
      status = $?.exitstatus
      bf_post_conf if ENV['BUILDFARM']
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

  def bf_post_conf
    puts "Buildfarm post configuration..."
    FileUtils.ln_s('CMakeCache.txt', 'config.log',
                   :verbose => true, :force => true)
    cmakefile = 'Makefile.cmake'
    FileUtils.cp('Makefile', cmakefile)
    File.open('Makefile', 'w') { |io| io.puts(gen_bf_makefile(cmakefile)) }
  end

  def gen_bf_makefile(makefilename)
    opt = @opts[:bf_makefile]
    if File.basename(opt[:make]) == "make"
      od = '-'
    else
      od = '/'
    end
    out = <<EOF
MAKE = #{opt[:make]}
MAKE_OPT = #{od}f #{makefilename} #{od}k

EOF
    opt[:rules].each do |r, v|
      out += (if v.class == NilClass
                gen_mf_rule(r, "$(MAKE) $(MAKE_OPT) #{r}")
              elsif v.class == String
                gen_mf_rule(r, v)
              elsif v.class == Hash
                gen_mf_rule(r, v[:depends], v[:actions])
              else
                raise(ArgumentError,
                      "bad value type '#{v.class}' for key '#{r}'")
              end)
    end
    out
  end

  def gen_mf_rule(rule, code="", deps=[])
    "#{rule}: #{deps.join(' ')}\n#{code.gsub(/^/, "\t")}\n\n"
  end

end # class ConfCmake

if $0 == __FILE__

  require 'pp'

  go = ConfCmake.new('SDK_REMOTE', 'QSCINTILLA2')
  pp go.parse(ARGV)
  puts go.cmakeflags

end

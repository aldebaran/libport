#!/usr/bin/env ruby
##
## emacsen-qtestlib.rb: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2007.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# DOCUMENTATION:
#
# QTestLib output is compatible with KDevelop and Visual Studio. This filter
# parses its output and make it compatible with Emacs.
#
# Example:
#
# FAIL!  : TestQURBIConsole::foo() '1 == 2' returned FALSE. ()
#   Loc: [../../../../../src/common/urbi-console/tests/urbi-console/test-qurbi-console.hh(40)]
#
# becomes:
#
# /home/despres/src/common/urbi-console/tests/urbi-console/test-qurbi-console.hh:40: TestQURBIConsole::foo() '1 == 2' returned FALSE. ()
#

require 'pathname'

error = nil
(
 if ARGV.empty?
   STDIN
 else
   `#{ARGV.join(' ')}`
 end
).each do |line|
  if line =~ /^FAIL! +: (.*)$/
    previousLine = line
    error = $1
  elsif error
    if line =~ /^ *Loc: \[(.*?)\((.*?)\)\]$/
      puts "#{Pathname.getwd + Pathname.new($1)}:#$2: #{error}"
    else
      STDERR.puts "#$0: enable to parse:\n#{previousLine}#{line}"
    end
    error = nil
  else
    error = nil
    puts line
  end
end
if $?.signaled?
  exit 128 + $?.termsig
elsif $?.exited?
  exit $?.exitstatus
else
  exit 0
end

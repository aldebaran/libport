#!/usr/bin/env
##
## conf.rb: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

require 'Getopts'

def configure_cmake(*deplibs)
  go = Getopts.new(*deplibs)
  go.parse!(ARGV)
  puts "** Configuration:"
  cmakeflags = go.cmakeflags
  cmd = "  cmake " + cmakeflags
  puts cmd
  puts '** Running cmake...'
  system cmd
end

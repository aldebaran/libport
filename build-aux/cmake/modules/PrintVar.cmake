##
## PrintVar.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Display the value of the variables given as argument.
macro(PRINT_VAR)
  foreach(i ${ARGV})
    message(STATUS "${i}='${${i}}'")
  endforeach(i)
endmacro(PRINT_VAR)

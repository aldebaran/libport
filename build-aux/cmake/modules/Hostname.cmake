##
## Hostname.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

if(NOT COMMAND HOSTNAME)

# Find the hostname of machine running cmake.
# outvar: the name of the variable where to store the result.
macro(HOSTNAME outvar)
  EXECUTE_PROCESS(
    COMMAND hostname
    OUTPUT_VARIABLE ${outvar}
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endmacro(HOSTNAME)

endif(NOT COMMAND HOSTNAME)

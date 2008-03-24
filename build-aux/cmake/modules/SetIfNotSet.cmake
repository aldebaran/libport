##
## SetIfNotSet.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

if(NOT COMMAND SET_IF_NOT_SET)

# Set the given variable with the given value if the variable is not already
# set.
macro(SET_IF_NOT_SET var val)
  IF(NOT DEFINED "${var}")
    SET(${var} "${val}")
  ENDIF(NOT DEFINED "${var}")
endmacro(SET_IF_NOT_SET var val)

endif(NOT COMMAND SET_IF_NOT_SET)


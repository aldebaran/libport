##
## CheckCMakeVarsExists.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

if(NOT COMMAND CHECK_CMAKE_VARS_EXISTS_CMAKE)

  # Check whether the given variables exists. Send an error message if it
  # doesn't.
  macro(CHECK_CMAKE_VARS_EXISTS)
    foreach(i ${ARGV})
      if(NOT DEFINED ${i})
	message(SEND_ERROR "Variables '${i}' must be defined.")
      endif()
    endforeach(i)
  endmacro(CHECK_CMAKE_VARS_EXISTS)

endif(NOT COMMAND CHECK_CMAKE_VARS_EXISTS_CMAKE)

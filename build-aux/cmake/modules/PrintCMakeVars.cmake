##
## PrintCMakeVars.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

if(NOT COMMAND PRINT_CMAKE_VARS)

include(PrintVar)

# Print all cmake vars.
function(PRINT_CMAKE_VARS)
  get_cmake_property(vars VARIABLES)
  foreach(var in ${vars})
    print_var(${var})
  endforeach(var)
endfunction(PRINT_CMAKE_VARS)

endif(NOT COMMAND PRINT_CMAKE_VARS)

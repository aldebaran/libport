##
## FindMakeinfo.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

#
# This module find the makeinfo program on your system
#
# Output variables:
#   MAKEINFO_EXECUTABLE
#

if(NOT MAKEINFO_EXECUTABLE)

find_program(
  MAKEINFO_EXECUTABLE
  makeinfo
  DOC "The path to the makeinfo program used."
  )
if(NOT MAKEINFO_EXECUTABLE)
  message(SEND_ERROR "Cannot find makeinfo")
endif()

endif(NOT MAKEINFO_EXECUTABLE)

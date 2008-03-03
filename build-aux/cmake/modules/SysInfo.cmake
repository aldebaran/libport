##
## SysInfo.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Include this file in order to print system information.

macro(LOG line)
   message(STATUS ${line})
   file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/SystemInfo.txt "${line}\n")
endmacro(LOG line)

macro(SYS_INFO)
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/SystemInfo.txt "System information:\n")
  message(STATUS "-----------------------------------")
  message(STATUS "System informations: ")
  message(STATUS "-----------------------------------")

  log("C compiler: ${CMAKE_C_COMPILER}")
  log("CXX compiler: ${CMAKE_CXX_COMPILER}")
  log("C compiler is gcc: ${CMAKE_COMPILER_IS_GNUCC}")
  log("CXX compiler is g++: ${CMAKE_COMPILER_IS_GNUCXX}")
  log("System: ${CMAKE_SYSTEM}")
  log("System name: ${CMAKE_SYSTEM_NAME}")
  log("System version: ${CMAKE_SYSTEM_VERSION}")

  if(UNIX)
    log("System kind: UNIX like")
  endif(UNIX)

  if(WIN32)
    log("System kind: Win32")
  endif(WIN32)

  if(APPLE)
    log("System kind: Apple")
  endif(APPLE)

  if(MINGW)
    log("System kind: MinGW")
  endif(MINGW)

  if(CYGWIN)
    log("System kind: Cygwin")
  endif(CYGWIN)

  if(BORLAND)
    log("System kind: Borland compiler")
  endif(BORLAND)

  message(STATUS "-----------------------------------")
endmacro(SYS_INFO)

##
## PreferedSearchedLibrary.cmake: This file is part of libport.
## Copyright (C) 2006-2008, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

if(NOT SWITCH_PREFERED_LIB_SEARCH)

  function(SWITCH_PREFERED_LIB_SEARCH static_or_shared)
    if(static_or_shared STREQUAL "STATIC")
      prefer_static_lib()
    elseif(static_or_shared STREQUAL "SHARED")
      prefer_shared_lib()
    else(static_or_shared STREQUAL "STATIC")
      message(FATAL_ERROR "switch_prefered_lib_search: argument must be STATIC
or SHARED")
    endif(static_or_shared STREQUAL "STATIC")
  endfunction(SWITCH_PREFERED_LIB_SEARCH)

  #FIXME: We duplicate cmake's code that set the CMAKE_FIND_LIBRARY_SUFFIXES.

  function(PREFER_STATIC_LIB)
    if(UNIX)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so" PARENT_SCOPE)
    endif(UNIX)
  endfunction(PREFER_STATIC_LIB)

  function(PREFER_SHARED_LIB)
    if(UNIX)
      set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a" PARENT_SCOPE)
    endif(UNIX)
  endfunction(PREFER_SHARED_LIB)

endif(NOT SWITCH_PREFERED_LIB_SEARCH)


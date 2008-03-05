##
## QtTools.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

macro(check_qtversion version)
  if(QTVERSION STRLESS ${version})
    message(FATAL_ERROR "We require Qt-${DESIRED_QT_VERSION}. "
      "We found Qt-${QTVERSION}.")
  endif(QTVERSION STRLESS ${version})
endmacro(check_qtversion)

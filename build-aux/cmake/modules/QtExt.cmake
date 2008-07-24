##
## QtExt.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Qt
include(QtTools)
set(QT_REQUIRED TRUE)
set(DESIRED_QT_VERSION "4.3.2")
find_package(MyQt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})
# link_directories(${QT_LIBRARY_DIR})
check_qtversion(${DESIRED_QT_VERSION})

# sdk-remote
find_package(SdkRemote REQUIRED)

# QScintilla2
if(WITH_QURBI_EDITOR)
  find_package(QScintilla2 REQUIRED)
endif(WITH_QURBI_EDITOR)

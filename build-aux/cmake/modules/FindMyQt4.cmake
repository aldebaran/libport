##
## FindMyQt4.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# This is a work around file.

find_package(Qt4 REQUIRED)

# FindQt4 documentation talks about QT_LIBRARIES but it is not set.
set(QT_LIBRARIES
  ${QT_QT3SUPPORT_LIBRARY}
  ${QT_QTASSISTANT_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDESIGNER_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTMOTIF_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTNSPLUGIN_LIBRARY}
  ${QT_QTOPENGL_LIBRARY}
  ${QT_QTSQL_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTSVG_LIBRARY}
  ${QT_QTTEST_LIBRARY}
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTUITOOLS_LIBRARY}
  )

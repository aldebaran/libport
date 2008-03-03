##
## QtInfo.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

macro(QT_INFO)
  message(STATUS "-----------------------------------")
  message(STATUS "Qt informations")
  message(STATUS "-----------------------------------")
  if(QT_FOUND STREQUAL "YES")
    foreach(i
	QT4_FOUND
	QT_QTCORE_FOUND
	QT_QTGUI_FOUND
	QT_QT3SUPPORT_FOUND
	QT_QTASSISTANT_FOUND
	QT_QTDESIGNER_FOUND
	QT_QTMOTIF_FOUND
	QT_QTNETWORK_FOUND
	QT_QTNSPLUGIN_FOUND
	QT_QTOPENGL_FOUND
	QT_QTSQL_FOUND
	QT_QTXML_FOUND
	QT_QTSVG_FOUND
	QT_QTTEST_FOUND
	QT_QTUITOOLS_FOUND
	)
      message(STATUS "${i}: ${${i}}")
    endforeach(i)
  else(QT_FOUND STREQUAL "YES")
    message("no qt found")
  endif(QT_FOUND STREQUAL "YES")
  message(STATUS "-----------------------------------")
endmacro(QT_INFO)

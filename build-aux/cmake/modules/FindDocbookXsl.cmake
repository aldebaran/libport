##
## FindDocbookXsl.cmake: This file is part of cmake-aux.
## Copyright (C) 2008, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

set(XSL_SEARCH_PATHS
    # Fedora
    "/usr/share/sgml/docbook/xsl-stylesheets"
    # Cygwin
    "/usr/share/docbook-xsl"
    # Debian
    "/usr/share/xml/docbook/stylesheet/nwalsh"
    # SUSE
    "/usr/share/xml/docbook/stylesheet/nwalsh/current"
    # FreeBSD
    "/usr/local/share/xsl/docbook"
    # Gentoo
    "/usr/share/sgml/docbook/xsl-stylesheets-*"
    # Mac OS X + fink
    "/sw/share/xml/xsl/docbook-xsl"
    # Mac OS X + Mac ports
    "/opt/local/share/xsl/docbook-xsl"
    )

if(NOT XSL_DIR)
  find_path(XSL_DIR
    NAMES catalog.xml
    PATHS ${XSL_SEARCH_PATHS}
    NO_DEFAULT_PATH)
  if(XSL_DIR)
    message(STATUS "Found Docbook XSL directory: "
      "${XSL_DIR}")
  else(XSL_DIR)
    message(SEND_ERROR "no Docbook XSL found "
      "(setting -DXSL_DIR may solve this problem).")
  endif(XSL_DIR)
endif(NOT XSL_DIR)

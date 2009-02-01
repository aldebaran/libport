##
## FindUrbiOpenSSL.cmake: This file is part of libport.
## Copyright (C) Gostai S.A.S., 2006-2009.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##
##-----------------------------------------------------------------
## CMake PACKAGE - Urbi Open SSL
## Find Urbi Engine Generic libraries and paths.
#
# Current FindOpenSSL module provided by CMake is laking:
#    * support for choosing between static/shared library to search for
#    * search of the libcrypto
# that's why we provide this alternative package waiting for better solution
# coming from upstream.
#
##-----------------------------------------------------------------

set(PACKAGE_FULLNAME "OpenSSL/TLS")

##-----------------------------------------------------------------
# This package tries to respect CMake guidelines to be easily
# reusable.
# This module handles the following variables. Please note XXX is
# the name specified in the PACKAGE_NAME variable (just above).
#
# XXX_INCLUDE_DIRS     The final set of include directories listed in
#                      one variable for use by client code.
#
# XXX_LIBRARY_DIRS     Optionally, the final set of library
#                      directories listed in one variable for use
#                      by client code.
#
# XXX_LIBRARIES        The libraries to link against to use XXX. These
#                      should include full paths.
#
# XXX_SHARED_LIBRARIES The list of all shared libraries found in the
#                      package. This can be usefull if you're looking
#                      for .dll or .so to export during the install.
#
# XXX_YY_FOUND         FALSE/Unidefined if YY part of XXX package has
#                      not been found.
#
# XXX_FOUND            FALSE/Undefined if the whole package has not
#                      been found.
#                      Set it manually to FALSE don't want to use XXX.
#
# XXX_ROOT_DIR         Paths you want to add in the path list in order
#                      to help cmake find your package on your computer.
#
# QUIET option         You can ask the package not to complain if there
#                      is library/path not found. This means no output.
#                      If you specify both QUIET and REQUIRED option,
#                      the QUIET option has a lower priority and
#                      critical messages are displayed.
#
# REQUIRED option      You can ask the package to throw a FATAL_ERROR
#                      if the whole package has not been found.
#
# Here is the XXX_YY list:
# URBI_OPEN_SSL_CRYPTO_LIBRARY
# URBI_OPEN_SSL_DL_LIBRARY
#
##-----------------------------------------------------------------

include(Package-toolbox)
package_header(${CMAKE_CURRENT_LIST_FILE})

if(UNIX)
    # Search for openssl library.
    find_package(OpenSSL)
    if(OPENSSL_FOUND)
      # FIXME: Check if other variable are requiered
      list(APPEND ${PACKAGE_NAME}_LIBRARIES ${OPENSSL_LIBRARIES})
      list(APPEND ${PACKAGE_NAME}_INCLUDE_DIRS ${OPENSSL_INCLUDE_DIRS})
      list(APPEND ${PACKAGE_NAME}_LIBRARY_DIRS ${OPENSSL_LIBRARY_DIRS})
    endif()

    # Search for the associated crypto library.
    package_search(LIBRARY ${PACKAGE_NAME}_CRYPTO_LIBRARY crypto
                   FULLNAME "Urbi OpenSSL's Cryptography"
                   STATIC)

    # Search for the associated dl library.
    package_search(LIBRARY ${PACKAGE_NAME}_DL_LIBRARY dl
                   FULLNAME "Urbi OpenSSL's dl"
                   SHARED)
endif(UNIX)

# In windows, some programs could have already installed the libeay32
# Because it may not be the same version as ours, we must search in
# the install dir our lib.
if(WIN32)

    # Search for the include directory.
    package_search(PATH ${PACKAGE_NAME}_INCLUDE openssl/ssl.h
                   FULLNAME "OpenSSL SSL INCLUDE"
                   PATHS ${${PACKAGE_NAME}_ROOT_DIR}/Debug/include
                         ${${PACKAGE_NAME}_ROOT_DIR}/Release/include
                         ${${PACKAGE_NAME}_ROOT_DIR}/include)

    package_search(LIBRARY ${PACKAGE_NAME}_EAY32_LIBRARY eay32 libeay32
                   FULLNAME "OpenSSL Crypto for Windows"
                   INSTALL "Urbi Engine Generic"
                   PATHS ${URBI_ENGINE_GENERIC_ROOT_DIR}/Release/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Debug/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Release/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Debug/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/lib
                         ${${PACKAGE_NAME}_ROOT_DIR}/lib)

    package_search(LIBRARY ${PACKAGE_NAME}_SSLEAY32_LIBRARY ssleay32
                   FULLNAME "OpenSSL for Windows"
                   INSTALL "URBI Engine Generic"
                   PATHS ${URBI_ENGINE_GENERIC_ROOT_DIR}/Release/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Debug/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Release/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Debug/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/lib
                         ${${PACKAGE_NAME}_ROOT_DIR}/lib)

    package_search(LIBRARY ${PACKAGE_NAME}_IPHLPAPI_LIBRARY iphlpapi
                   FULLNAME "OpenSSL IPHLPAPI for Windows"
                   INSTALL "Urbi Engine Generic"
                   PATHS ${URBI_ENGINE_GENERIC_ROOT_DIR}/Release/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Debug/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Release/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/Debug/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/gostai/core/i686-pc-cygwin/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/gostai/core/i686-pc-linux-gnu/engine
                         ${URBI_ENGINE_GENERIC_ROOT_DIR}/lib
                         ${${PACKAGE_NAME}_ROOT_DIR}/lib)

endif(WIN32)

package_foot(UNIX  ${PACKAGE_NAME}_DL_LIBRARY ${PACKAGE_NAME}_CRYPTO_LIBRARY
             WIN32 ${PACKAGE_NAME}_EAY32_LIBRARY)

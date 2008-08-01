##
## Package-toolbox.cmake: This file is part of cmake-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##
##-----------------------------------------------------------------
## CMake PACKAGE - Toolbox
## Usefull macro to use into package file
##-----------------------------------------------------------------


#-----------------------------------------------------------------
#                           Set PACKAGE_NAME
#-----------------------------------------------------------------
# You must give the macro the CMAKE_CURRENT_LIST_FILE.
# Then it deduces from the filename the most appropriate package name.
# Set it in PACKAGE_NAME variable. It also set you PACKAGE_FILENAME
# which is the XXX part of FindXXX.cmake with the file case if you
# want to retrieve parameters like XXX_FIND_QUIET and
# XXX_FIND_REQUIRED.
#
# e.g. "foo/bar/FindYourCoolPACKAGEwithNEWfeature.cmake"
#      will generate : "YOUR_COOL_PACKAGE_WITH_NEW_FEATURE"
#
# Algorithm: 1. Extract the XXX in /current/dir/FindXXX.cmake string
#            2. Put a '_' between lowcase/upcase
#            3. Put a '_' between upcase(2 or more)/lowcase
#            4. Upper case the output string
#

MACRO(set_package_name CURRENT_LIST_FILE)

    string(REGEX MATCH "Find(.+)\\.cmake$" DUMMY ${CURRENT_LIST_FILE})
    set(PACKAGE_FILENAME ${CMAKE_MATCH_1})
    string(REGEX REPLACE "([^A-Z])([A-Z])" "\\1_\\2" CMAKE_MATCH_1 ${CMAKE_MATCH_1})
    string(REGEX REPLACE "([A-Z][A-Z]+)([^A-Z])" "\\1_\\2" CMAKE_MATCH_1 ${CMAKE_MATCH_1})
    string(TOUPPER ${CMAKE_MATCH_1} PACKAGE_NAME)

ENDMACRO(set_package_name)

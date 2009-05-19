## Copyright (C) 2006-2008, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
## --------------------------------------------------------------
## CMake - Macro toolbox
## The aim of this file is to group usefull for user-defined macro.
## --------------------------------------------------------------
##

if(NOT MODULE_MACRO_TOOLBOX_LOADED)
set(MODULE_MACRO_TOOLBOX_LOADED TRUE)

#-----------------------------------------------------------------
#                       List of shared libraries
#-----------------------------------------------------------------
# Input : List of libraries (must be full path).
# Output: List of shared libraries in the input variable.
#         Note that output is not overwritten; results are put
#         at the end of the list.
# Remember shared libraries must be present in the same folder
# as the executable (or in common paths) when it is launched.
#
# e.g. : list_shared_libraries(${PACKAGE_NAME}_LIBRARIES
#                              ${PACKAGE_NAME}_SHARED_LIBRARIES)
#
# Note : Output and input are the name of your output/input
#        variable, NOT their values.

MACRO(list_shared_libraries INPUT OUTPUT)

    # List of suffix CMake will look for.
    if(MSVC_VERSION)
        set(LIBRARY_SHARED_SUFFIX .dll)

        foreach(i ${${INPUT}})
            # Avoid symbolic links
            get_filename_component(PATH_TEMP ${i} ABSOLUTE)
            get_filename_component(PATH_TEMP ${PATH_TEMP} PATH)
            # And look for the shared libraries
            get_filename_component(NAME_TEMP ${i} NAME_WE)

            foreach(j ${LIBRARY_SHARED_SUFFIX})
                if(EXISTS ${PATH_TEMP}/${NAME_TEMP}${j})
                    list(APPEND ${OUTPUT}
                                ${PATH_TEMP}/${NAME_TEMP}${j})
                endif(EXISTS ${PATH_TEMP}/${NAME_TEMP}${j})
            endforeach(j ${LIBRARY_SHARED_SUFFIX})
        endforeach(i ${${INPUT}})

    endif(MSVC_VERSION)

    if(UNIX)
        set(LIBRARY_SHARED_SUFFIX .so)
        foreach(i ${${INPUT}})
            get_filename_component(EXT_TEMP ${i} EXT)
            if(EXT_TEMP)
                foreach(j ${LIBRARY_SHARED_SUFFIX})
	            if(${EXT_TEMP} STREQUAL ${j})
	                list(APPEND ${OUTPUT} ${i})
	            endif(${EXT_TEMP} STREQUAL ${j})
	        endforeach(j ${LIBRARY_SHARED_SUFFIX})
	    endif(EXT_TEMP)
        endforeach(i ${${INPUT}})
    endif(UNIX)

    # Send the shared libraries to the root CMakeLists
    set(${OUTPUT} ${${OUTPUT}} PARENT_SCOPE)

ENDMACRO(list_shared_libraries)

endif(NOT MODULE_MACRO_TOOLBOX_LOADED)

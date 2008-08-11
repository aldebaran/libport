## Copyright (C) Gostai S.A.S., 2006-2008.
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

#------------------------------------------------------------------------------
#                                 Arguments parser
#------------------------------------------------------------------------------
# This macro can be found on the wiki page of CMake.
# Example given :
# --------------

# SET(arguments
#   hello OPTION3 world
#   LIST3 foo bar
#   OPTION2
#   LIST1 fuz baz
#   )
# parse_arguments(ARG "LIST1;LIST2;LIST3" "OPTION1;OPTION2;OPTION3" ${arguments})

# * results
#     > ARG_DEFAULT_ARGS: hello;world
#     > ARG_LIST1: fuz;baz
#     > ARG_LIST2:
#     > ARG_LIST3: foo;bar
#     > ARG_OPTION1: FALSE
#     > ARG_OPTION2: TRUE
#     > ARG_OPTION3: TRUE


MACRO (parse_arguments prefix arg_names option_names)

  # Init. all section variables
  set (DEFAULT_ARGS)
  foreach (arg_name ${arg_names})
    set (${prefix}_${arg_name})
  endforeach (arg_name)

  foreach (option ${option_names})
    set (${prefix}_${option} FALSE)
  endforeach (option)

  set (current_arg_name DEFAULT_ARGS)
  set (current_arg_list)

  # Browse all the arguments
  foreach (arg ${ARGN})

    # Look for name of sections
    set (larg_names ${arg_names})
    list (FIND larg_names "${arg}" is_arg_name)

    if (is_arg_name GREATER -1)

      set (${prefix}_${current_arg_name} ${current_arg_list})
      set (current_arg_name ${arg})
      set (current_arg_list)

    else (is_arg_name GREATER -1)

      set (loption_names ${option_names})
      list (FIND loption_names "${arg}" is_option)

      # Options
      if (is_option GREATER -1)
	     set (${prefix}_${arg} TRUE)
      else (is_option GREATER -1)
	     set (current_arg_list ${current_arg_list} ${arg})
      endif (is_option GREATER -1)

    endif (is_arg_name GREATER -1)
  endforeach (arg)

  set (${prefix}_${current_arg_name} ${current_arg_list})

ENDMACRO (parse_arguments)

endif(NOT MODULE_MACRO_TOOLBOX_LOADED)



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

if(NOT COMMAND parse_arguments)

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

endif(NOT COMMAND parse_arguments)

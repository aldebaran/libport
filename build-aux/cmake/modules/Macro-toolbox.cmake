## --------------------------------------------------------------
## CMake - Macro toolbox    
## The aim of this file is to group usefull for
## user-defined macro
## --------------------------------------------------------------
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

#-----------------------------------------------------------------------------------------
#                                     Arguments parser
#-----------------------------------------------------------------------------------------

# Example :
# ---------

# SET ( arguments
#   hello OPTION3 world
#   LIST3 foo bar
#   OPTION2
#   LIST1 fuz baz
#   )
# parse_arguments ( ARG "LIST1;LIST2;LIST3" "OPTION1;OPTION2;OPTION3" ${arguments} )

# * results
#     > ARG_DEFAULT_ARGS: hello;world
#     > ARG_LIST1: fuz;baz
#     > ARG_LIST2:
#     > ARG_LIST3: foo;bar
#     > ARG_OPTION1: FALSE
#     > ARG_OPTION2: TRUE
#     > ARG_OPTION3: TRUE 


MACRO ( parse_arguments prefix arg_names option_names )

  set ( DEFAULT_ARGS )
  foreach ( arg_name ${arg_names} )    
    set ( ${prefix}_${arg_name} )
  endforeach ( arg_name )
  
  foreach ( option ${option_names} )
    set ( ${prefix}_${option} FALSE )
  endforeach ( option )

  set ( current_arg_name DEFAULT_ARGS )
  set ( current_arg_list )
  
  foreach  ( arg ${ARGN} )            
    set ( larg_names ${arg_names} )    
    list ( FIND larg_names "${arg}" is_arg_name )                   
  
    if ( is_arg_name GREATER -1 )
  
      set ( ${prefix}_${current_arg_name} ${current_arg_list} )
      set ( current_arg_name ${arg} )
      set ( current_arg_list )
  
    else ( is_arg_name GREATER -1)
  
      set ( loption_names ${option_names} )    
      list ( FIND loption_names "${arg}" is_option )
  
      if ( is_option GREATER -1 )
	
	     set ( ${prefix}_${arg} TRUE )
      else ( is_option GREATER -1 )
	     set ( current_arg_list ${current_arg_list} ${arg} )
  
      endif ( is_option GREATER -1 )
  
    endif ( is_arg_name GREATER -1 )
  
  endforeach ( arg )
  
  set ( ${prefix}_${current_arg_name} ${current_arg_list} )

ENDMACRO( parse_arguments )

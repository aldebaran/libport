# - Implement escape functions.

if(NOT COMMAND ESCAPE_DOUBLE_QUOTE)

  # Escape double quote string
  macro(ESCAPE_DOUBLE_QUOTE out in)
    string(REGEX REPLACE "([\"\\])" "\\\\\\1" out ${in})
  endmacro(ESCAPE_DOUBLE_QUOTE)

endif(NOT COMMAND ESCAPE_DOUBLE_QUOTE)

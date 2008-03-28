# - Configure texinfo file
# This function is very similar to configure_file except that it takes care to
# escape @ characters. Prefers to use ${var} expansion form instead of @var@

if(NOT COMMAND CONFIGURE_TEXINFO)

function(CONFIGURE_TEXINFO input output)
  # Collect all variables ${var} in the file.
  file(READ ${input} inbuf)
  # We write [{] instead of { to avoid pattern like ${..} that cmake tries to
  # expand.
  string(REGEX MATCHALL "\\$[{]([^}]+)}" varlist ${inbuf})
  # Removes $, { and } from the variables list.
  set(tmpvarlist "")
  foreach(i ${varlist})
    string(REGEX REPLACE "[$}{]" "" tmp ${i})
    list(APPEND tmpvarlist ${tmp})
  endforeach(i)
  set(varlist ${tmpvarlist})
  set(tmpvarlist)
  # Save and escape variables.
  foreach(i ${varlist})
    set(__configure_textinfo_copy__${i} ${${i}})
    string(REGEX REPLACE "@([^@])" "@@\\1" tmp "${${i}}")
    set(${i} ${tmp})
  endforeach(i)
  # Configure the file.
  configure_file(${input} ${output} ESCAPE_QUOTES)
  # Restore variables and destroy backup.
  foreach(i ${varlist})
    set(${i} ${__configure_textinfo_copy__${i}})
    set(__configure_textinfo_copy__${i})
  endforeach(i)
  message(STATUS "Configure '${input}' to '${output}'")
endfunction(CONFIGURE_TEXINFO)

endif(NOT COMMAND CONFIGURE_TEXINFO)


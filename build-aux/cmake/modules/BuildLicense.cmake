# - Build the license file.
# On mac and windows we can keep the RTF format but on linux we need a plain
# text format.

if(NOT COMMAND BUILD_LICENSE)

include(UnrtfFile)

function(BUILD_LICENSE input output)
  if(UNIX AND NOT CYGWIN AND NOT APPLE)
    set(tmpout ${output}-workaround-unrtf.tmp)
    unrtf_file(${input} ${tmpout} --text)
    file(READ ${tmpout} buf)
    #FIXME: unrtf introduces ` that confuse shitty dash shell by default on
    #       all Ubuntu distro. So we substitue all ` by '.
    string(REGEX REPLACE "`" "'" buf ${buf})
    #FIXME: We also replace stupid ? introduces by unrtf where " is prefered.
    string(REGEX REPLACE "\\?" "\"" buf ${buf})
    file(WRITE ${output} ${buf})
    file(REMOVE ${tmpout})
  else()
    # Simple copy.
    configure_file(${input} ${output} COPYONLY)
  endif()
  message(STATUS "Build license '${input}' to '${output}'")
endfunction(BUILD_LICENSE)

endif(NOT COMMAND BUILD_LICENSE)


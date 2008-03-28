# - Build the license file.
# On mac and windows we can keep the RTF format but on linux we need a plain
# text format.

if(NOT COMMAND BUILD_LICENSE)

include(UnrtfFile)

function(BUILD_LICENSE input output)
  if(UNIX AND NOT CYGWIN AND NOT APPLE)
    unrtf_file(${input} ${output} --text)
  else()
    # Simple copy.
    configure_file(${input} ${output} COPYONLY)
  endif()
  message(STATUS "Build license '${input}' to '${output}'")
endfunction(BUILD_LICENSE)

endif(NOT COMMAND BUILD_LICENSE)


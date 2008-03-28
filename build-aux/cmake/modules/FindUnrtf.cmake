# - Find unrtf program.
# Output variables:
#  UNRTF_EXECUTABLE - the path to the unrtf program
#

if(NOT UNRTF_EXECUTABLE)

find_program(
  UNRTF_EXECUTABLE
  unrtf
  DOC "The path to the unrtf program used."
  )
if(NOT UNRTF_EXECUTABLE)
  message(SEND_ERROR "Cannot find unrtf")
endif()

endif(NOT UNRTF_EXECUTABLE)

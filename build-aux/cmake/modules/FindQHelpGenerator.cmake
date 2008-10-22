# - Find qhelpgenerator program.
# Output variables:
#  QHELPGENERATOR_EXECUTABLE - the path to the qhelpgenerator program
#

if(NOT QHELPGENERATOR_EXECUTABLE)

find_program(
  QHELPGENERATOR_EXECUTABLE
  qhelpgenerator
  DOC "The path to the qhelpgenerator program used."
  )
if(NOT QHELPGENERATOR_EXECUTABLE)
  message(SEND_ERROR "Cannot find qhelpgenerator")
endif(NOT QHELPGENERATOR_EXECUTABLE)

endif(NOT QHELPGENERATOR_EXECUTABLE)

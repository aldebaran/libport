# - Find qhelpgenerator program.
# Output variables:
#  QCOLLECTIONGENERATOR_EXECUTABLE - the path to the qcollectiongenerator
#                                    program.
#

if(NOT QCOLLECTIONGENERATOR_EXECUTABLE)

find_program(
  QCOLLECTIONGENERATOR_EXECUTABLE
  qcollectiongenerator
  DOC "The path to the qcollectiongenerator program used."
  )
if(NOT QCOLLECTIONGENERATOR_EXECUTABLE)
  message(SEND_ERROR "Cannot find qcollectiongenerator")
endif(NOT QCOLLECTIONGENERATOR_EXECUTABLE)

endif(NOT QCOLLECTIONGENERATOR_EXECUTABLE)

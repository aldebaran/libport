# - Find xsltproc program.
# Output variables:
#  XSLTPROC_EXECUTABLE - the path to the xsltproc program.
#

if(NOT XSLTPROC_EXECUTABLE)

find_program(
  XSLTPROC_EXECUTABLE
  xsltproc
  DOC "The path to the xsltproc program used."
  )
if(NOT XSLTPROC_EXECUTABLE)
  message(SEND_ERROR "Cannot find xsltproc.")
endif(NOT XSLTPROC_EXECUTABLE)

endif(NOT XSLTPROC_EXECUTABLE)

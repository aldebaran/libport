# - Search for Qt assistant executable file.

if(NOT QT_QTASSISTANT_EXECUTABLE)

  if(NOT QT_BINARY_DIR)
    message(SEND_ERROR
      "Can't find Qt binary path. Make sure you have searched for Qt4 first.")
  endif(NOT QT_BINARY_DIR)
  find_program(QT_QTASSISTANT_EXECUTABLE
    NAMES assistant
    PATHS ${QT_BINARY_DIR}
    NO_DEFAULT_PATH
    )

endif(NOT QT_QTASSISTANT_EXECUTABLE)

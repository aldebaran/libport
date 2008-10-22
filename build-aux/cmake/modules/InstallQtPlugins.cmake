# Install all qt plugins

if(NOT COMMAND install_qt_plugins)

function(install_qt_plugins destination)

  if(NOT DEFINED QT_PLUGINS_DIR)

    message(STATUS
      "Cannot install Qt's plugins because QT_PLUGINS_DIR is not defined.")
    return()

  endif(NOT DEFINED QT_PLUGINS_DIR)

  if(WIN32)
    set(qtplugin_pattern "*.dll")
  else(WIN32)
    if(UNIX)
      if(APPLE)
	set(qtplugin_pattern "*.dylib")
      else(APPLE)
	set(qtplugin_pattern "*.so")
      endif(APPLE)
    endif(UNIX)
  endif(WIN32)
  install(DIRECTORY "${QT_PLUGINS_DIR}/"
    DESTINATION ${destination}
    FILES_MATCHING PATTERN ${qtplugin_pattern})

endfunction(install_qt_plugins)

endif(NOT COMMAND install_qt_plugins)



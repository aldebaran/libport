
if(NOT TARGET release)

  #FIXME: add_dependencies does not work properly with cmake generated target
  # such as rebuild_cache and package.
  add_custom_target(release
    COMMAND ${CMAKE_BUILD_TOOL} tag-release
    COMMAND ${CMAKE_BUILD_TOOL} rebuild_cache
    COMMAND ${CMAKE_BUILD_TOOL} package
    COMMAND ${CMAKE_BUILD_TOOL} upload-package
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Releasing ${PROJECT_NAME}"
    )

endif(NOT TARGET release)

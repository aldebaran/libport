# - Find OpenSSL for use in Urbi
# Current FindOpenSSL module provided by CMake is laking:
#  - support for choosing between static/shared library to search for ;
#  - search of the libcrypto ;
# that's why we provide this alternative package waiting for better solution
# coming from upstream.
#

if(NOT URBIOPENSSL_FOUND)

  include(PreferedSearchedLibrary)

  # Save default library suffixes.
  set(old_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})

  prefer_static_lib()

  find_package(OpenSSL REQUIRED)
  if(OPENSSL_FOUND)

    # Search for lib crypto.
    find_library(CRYPTO_LIBRARY crypto)
    if(CRYPTO_LIBRARY)
      set(OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES} ${CRYPTO_LIBRARY})
      message(STATUS "Found lib crypto: ${CRYPTO_LIBRARY}")

      # Search for lib dl.
      prefer_shared_lib()
      find_library(DL_LIBRARY dl)
      if(DL_LIBRARY)
	set(OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES} ${DL_LIBRARY})
	message(STATUS "Found lib dl: ${DL_LIBRARY}")
	set(URBIOPENSSL_FOUND TRUE)
      endif(DL_LIBRARY)
    endif(CRYPTO_LIBRARY)
  else(OPENSSL_FOUND)
    message(FATAL_ERROR "library open SSL not found")
  endif(OPENSSL_FOUND)

  # Restore default library suffixes.
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${old_CMAKE_FIND_LIBRARY_SUFFIXES})
  set(old_CMAKE_FIND_LIBRARY_SUFFIXES)

  if(URBIOPENSSL_FOUND)
    message(STATUS "OpenSSL's include dir: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OpenSSL's libraries: ${OPENSSL_LIBRARIES}")
  endif(URBIOPENSSL_FOUND)

endif(NOT URBIOPENSSL_FOUND)

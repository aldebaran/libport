# CMake - Remote UObject project
# Include this file in your CMakeLists.txt file.
# Then type "add_uobject( NAME_OF_EXECUTABLE CPP1 CPP2 )

MACRO ( add_uobject UOBJECT_EXE FILES )

    # Autofind libraries
    if ( WIN32 )
      set ( CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "" "lib" )
    endif ( WIN32 )

    find_library ( SDK_REMOTE_LIBRARY urbi )
    find_library ( JPEG_LIBRARY jpeg )
    find_path ( SDK_REMOTE_INCLUDE_DIR urbi )

    # Keeps only required libs
    if ( WIN32 )
      set ( CMAKE_CXX_STANDARD_LIBRARIES "gdi32.lib" )
      set ( CMAKE_C_STANDARD_LIBRARIES "gdi32.lib")
    endif ( WIN32 )

    # Force release mode only
    if ( WIN32 )
      set ( CMAKE_BUILD_TYPE "Release" )
    endif ( WIN32 )

    # Default libraries, in case they're not found
    if ( NOT SDK_REMOTE_INCLUDE_DIR )
          message ( "SDK Remote INCLUDE directory has not been found. Make sure you've installed URBI SDK remote on your computer or please specify the library path manually (-DSDK_REMOTE_INCLUDE_DIR)." )
    endif ( NOT SDK_REMOTE_INCLUDE_DIR )

    if ( NOT JPEG_LIBRARY )
          message ( "The JPEG library has not been found. Make sure you've installed URBI SDK remote on your computer or please specify the library path manually (-DJPEG_LIBRARY)." )
    endif ( NOT JPEG_LIBRARY )

    if ( NOT SDK_REMOTE_LIBRARY )
          message ( "The URBI library has not been found. Make sure you've installed URBI SDK remote on your computer or please specify the library path manually (-DSDK_REMOTE_LIBRARY)." )
    endif ( NOT SDK_REMOTE_LIBRARY )

    # Create the uobject executable
    add_executable ( ${UOBJECT_EXE} ${FILES} )
    include_directories ( ${SDK_REMOTE_INCLUDE_DIR} )

    # Link 
    target_link_libraries ( ${UOBJECT_EXE} ${SDK_REMOTE_LIBRARY} ${JPEG_LIBRARY} )
    if ( WIN32 )
      target_link_libraries ( ${UOBJECT_EXE} ws2_32 )
    elseif ( UNIX )
      target_link_libraries ( ${UOBJECT_EXE} pthread )
      else ( UNIX )
        message ( "[UOBJECT] No auto-link has been pre-configured for your platform into CMake." )
    endif ( WIN32 )
        
ENDMACRO ( add_uobject )

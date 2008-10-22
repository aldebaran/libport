# - Add the lock system option.
# - Search for the lock system library

if(NOT DEFINED LOCK_SYSTEM_CMAKE)
  set(LOCK_SYSTEM_CMAKE TRUE)

# Add the lock system option.
option(ENABLE_LOCKSYSTEM
  "Enable the lock-system service: whether generated binaries will require a
 valid license file. (default is ON)"
  ON)

# Search for it.
if(ENABLE_LOCKSYSTEM)
  #FIXME: Implement a module that search for lock system without cmake
  #       configuration.
  find_package(lock-system REQUIRED NO_MODULE PATH_SUFFIXES lib)
  set(lock_system_LIBRARIES qt-lock-system lock-system)
  set(lock_system_CPPFLAGS LOCKSYSTEM_ENABLED)
endif(ENABLE_LOCKSYSTEM)

endif(NOT DEFINED LOCK_SYSTEM_CMAKE)

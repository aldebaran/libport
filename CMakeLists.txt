cmake_minimum_required(VERSION 2.8)
project(libport)

find_package(qibuild)


# Add a simple test:
enable_testing()


set(LIBPORT_COMPILATION_MODE_SPEED OFF)
set(LIBPORT_COMPILATION_MODE_DEBUG OFF)
set(LIBPORT_COMPILATION_MODE_SPACE OFF)

if (CMAKE_BUILD_TYPE STREQUAL Release)
  set(LIBPORT_COMPILATION_MODE_SPEED ON)
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(LIBPORT_COMPILATION_MODE_DEBUG ON)
endif()

if (CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
  set(LIBPORT_COMPILATION_MODE_SPACE ON)
endif()


qi_create_config_h(CONFIG_H
   include/libport/config.h.in
   libport/config.h
)

add_definitions(-DLIBPORT_URBI_UFLOAT_DOUBLE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/sched/libcoroutine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


qi_generate_header(
   ${CMAKE_CURRENT_BINARY_DIR}/include/libport/format.hh
   SRC include/libport/format.hh.gen
   COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/include/libport/format.hh.gen > ${CMAKE_CURRENT_BINARY_DIR}/include/libport/format.hh
)



set(PORT_SOURCES
lib/libport/asio-impl.hxx
lib/libport/asio-ssl.cc
lib/libport/asio.cc
lib/libport/backtrace.cc
lib/libport/base64.cc
lib/libport/buffer-stream.cc
lib/libport/cli.cc
lib/libport/csignal.cc
lib/libport/cstdio.cc
lib/libport/cstdlib.cc
lib/libport/cstring.cc
lib/libport/ctime.cc
lib/libport/damerau-levenshtein-distance.cc
lib/libport/debug.cc
lib/libport/deref.cc
lib/libport/dirent.cc
lib/libport/echo.cc
lib/libport/escape.cc
lib/libport/fd-stream.cc
lib/libport/file-library.cc
lib/libport/file-system.cc
lib/libport/fnmatch.cc
lib/libport/format.cc
lib/libport/hmac-sha1.cc
lib/libport/indent.cc
lib/libport/input-arguments.cc
lib/libport/io-stream.cc
lib/libport/ip-semaphore.cc
lib/libport/locale.cc
lib/libport/markup-ostream.cc
lib/libport/option-parser.cc
lib/libport/package-info.cc
lib/libport/path.cc
lib/libport/pid-file.cc
lib/libport/program-name.cc
lib/libport/read-stdin.cc
lib/libport/sched.cc
lib/libport/semaphore-rpl.cc
lib/libport/semaphore.cc
lib/libport/symbol.cc
lib/libport/synchronizer.cc
lib/libport/sys/utsname.cc
lib/libport/sysexits.cc
lib/libport/thread-pool.cc
lib/libport/timer.cc
lib/libport/tokenizer.cc
lib/libport/type-info.cc
lib/libport/ufloat.cc
lib/libport/umatrix.cc
lib/libport/unique-pointer.cc
lib/libport/unistd.cc
lib/libport/utime.cc
lib/libport/uvector.cc
lib/libport/version.cc
lib/libport/xalloc.cc
lib/libport/xlocale.cc
lib/libport/xltdl.cc
)


set(PORT_HEADERS
  ${CMAKE_CURRENT_BINARY_DIR}/include/libport/format.hh
  include/libport/deref.hh
include/libport/synchronizer.hh
include/libport/timer.hxx
include/libport/cstdlib
include/libport/base64.hh
include/libport/symbol.hh
include/libport/instance-tracker.hh
include/libport/buffer-stream.hh
include/libport/cassert
include/libport/io-stream.hh
include/libport/file-system.hh
include/libport/cctype
include/libport/symbol.hxx
include/libport/boost-error.hh
include/libport/thread.hh
include/libport/escape.hxx
include/libport/hash.hh
include/libport/bind.hh
include/libport/detect-win32.h
include/libport/compilation.hh
include/libport/xlocale.hh
include/libport/locale.hxx
include/libport/uvector.hh
include/libport/ref-counted.hh
include/libport/cstdlib.hxx
include/libport/asio.hxx
include/libport/foreach.hh
include/libport/hierarchy.hh
include/libport/file-library.hh
include/libport/local-data.hxx
include/libport/pair.hxx
include/libport/exception.hh
include/libport/boost-error.hxx
include/libport/network.h
include/libport/unit-test.hh
include/libport/containers.hh
include/libport/thread-data.hh
include/libport/sched.hh
include/libport/local.mk
include/libport/debug.hxx
include/libport/exception.hxx
include/libport/ulong-fixed-point.hh
include/libport/map.hh
include/libport/intrusive-ptr.hxx
include/libport/throw-exception.hh
${CMAKE_CURRENT_BINARY_DIR}/include/libport/config.h
include/libport/pid-file.hh
include/libport/singleton-ptr.hh
include/libport/synchronizer.hxx
include/libport/intrusive-ptr.hh
include/libport/allocator-static.hh
include/libport/ref-counted.hxx
include/libport/destructible.hh
include/libport/cmath
include/libport/tr1/type_traits
include/libport/system-warning-push.hh
include/libport/bench.hh
include/libport/path.hxx
include/libport/xltdl.hh
include/libport/echo.hxx
include/libport/hmac-sha1.hh
include/libport/option-parser.hxx
include/libport/pod-cast.hxx
include/libport/local-data.hh
include/libport/ufloat.hh
include/libport/indent.hh
include/libport/valgrind.hh
include/libport/lexical-cast.hh
include/libport/xalloc.hh
include/libport/ull-fixed-point.hh
include/libport/sysexits.hh
include/libport/escape.hh
include/libport/iostream
include/libport/timer.hh
include/libport/fd-stream.hh
include/libport/attributes.hh
include/libport/pair.hh
include/libport/smart-allocated.hxx
include/libport/reserved-vector.hh
include/libport/time.hxx
include/libport/read-stdin.hh
include/libport/thread-pool.hh
include/libport/program-name.hh
include/libport/path.hh
include/libport/unique-pointer.hxx
include/libport/allocator-static.hxx
include/libport/locale.hh
include/libport/algorithm
include/libport/unistd.h
include/libport/thread.hxx
include/libport/deref.hxx
include/libport/format.hh.gen
include/libport/compiler-section.hh
include/libport/semaphore.hh
include/libport/backtrace.hxx
include/libport/cctype.hxx
include/libport/select-const.hh
include/libport/separator.hh
include/libport/csignal
include/libport/markup-ostream.hh
include/libport/type-info.hh
include/libport/version.hh
include/libport/atomic.hh
include/libport/statistics.hxx
include/libport/weak-ptr.hh
include/libport/utime.hh
include/libport/cli.hxx
include/libport/fwd.hh
include/libport/damerau-levenshtein-distance.hh
include/libport/debug.hh
include/libport/utime.hxx
include/libport/file-library.hxx
include/libport/option-parser.hh
include/libport/lockable.hxx
include/libport/fifo.hxx
include/libport/cmath.hxx
include/libport/finally.hxx
include/libport/meta.hh
include/libport/fcntl.h
include/libport/fnmatch.h
include/libport/range.hxx
include/libport/map.hxx
include/libport/windows.hh
include/libport/cast.hh
include/libport/statistics.hh
include/libport/input-arguments.hxx
include/libport/select-ref.hh
include/libport/typelist.hh
include/libport/cstring.hxx
include/libport/instance-tracker.hxx
include/libport/pod-cast.hh
include/libport/time.hh
include/libport/warning-pop.hh
include/libport/condition.hh
include/libport/visitor.hh
include/libport/cast.hxx
include/libport/semaphore.hxx
include/libport/sstream.hxx
include/libport/ctime
include/libport/smart-allocated.hh
include/libport/xltdl.hxx
include/libport/compiler.hh
include/libport/singleton-ptr.hxx
include/libport/tokenizer.hh
include/libport/finally.hh
include/libport/range.hh
include/libport/algorithm.hxx
include/libport/pthread.h
include/libport/weak-ptr.hxx
include/libport/vector.hh
include/libport/ip-semaphore.hh
include/libport/destructible.hxx
include/libport/specific-ptr.hxx
include/libport/fifo.hh
include/libport/ufloat.h
include/libport/cerrno
include/libport/pthread.hxx
include/libport/errors.hh
include/libport/fnmatch.hxx
include/libport/warning-push.hh
include/libport/cli.hh
include/libport/uffloat.hh
include/libport/semaphore.h
include/libport/echo.hh
include/libport/specific-ptr.hh
include/libport/input-arguments.hh
include/libport/reserved-vector.hxx
include/libport/traits.hh
include/libport/package-info.hh
include/libport/sstream
include/libport/lockable.hh
include/libport/export.hh
include/libport/dirent.h
include/libport/condition.hxx
include/libport/containers.hxx
include/libport/preproc.hh
include/libport/backtrace.hh
include/libport/instrument.hh
include/libport/separate.hh
include/libport/ref-pt.hh
include/libport/dlfcn.h
include/libport/cstring
include/libport/xalloc.hxx
include/libport/separate.hxx
include/libport/system-warning-pop.hh
include/libport/ufloat.hxx
include/libport/unique-pointer.hh
include/libport/asio.hh
include/libport/safe-container.hxx
include/libport/safe-container.hh
include/libport/cstdint
include/libport/netdb.h
include/libport/type-info.hxx
include/libport/umatrix.hh
include/libport/cstdio
)
set(PORT_HEADERS_SYS
include/libport/sys/socket.h
include/libport/sys/stat.h
include/libport/sys/param.h
include/libport/sys/utsname.hxx
include/libport/sys/time.hxx
include/libport/sys/types.h
include/libport/sys/stat.hxx
include/libport/sys/utsname.h
include/libport/sys/select.h
include/libport/sys/wait.h
include/libport/sys/time.h
include/libport/sys/prctl.h
)
set(PORT_HEADERS_TR1
include/libport/tr1/type_traits
)
set(PORT_HEADERS_ARPA
include/libport/arpa/inet.h
)
qi_install_header(${PORT_HEADERS} SUBFOLDER libport)
qi_install_header(${PORT_HEADERS_SYS} SUBFOLDER libport/sys)
qi_install_header(${PORT_HEADERS_ARPA} SUBFOLDER libport/arpa)
qi_install_header(${PORT_HEADERS_TR1} SUBFOLDER libport/tr1)

set(SCHED_SOURCES
lib/sched/configuration.cc
lib/sched/coroutine-hooks.cc
lib/sched/job.cc
lib/sched/libcoroutine/Base.h
lib/sched/libcoroutine/Coro.cc
lib/sched/libcoroutine/asm.S
lib/sched/libcoroutine/context.c
lib/sched/libcoroutine/context.cc
lib/sched/scheduler.cc
lib/sched/tag.cc
lib/sched/uclibc-workaround.cc
)

set(SCHED_HEADERS
include/sched/exception.hh
include/sched/coroutine.hh
include/sched/local.mk
include/sched/exception.hxx
include/sched/coroutine-hooks.hh
include/sched/coroutine-coro.hxx
include/sched/scheduler.hxx
include/sched/tag.hxx
include/sched/configuration.hh
include/sched/fwd.hh
include/sched/scheduler.hh
include/sched/job.hxx
include/sched/coroutine-local-storage.hh
include/sched/coroutine.hxx
include/sched/job.hh
include/sched/export.hh
include/sched/coroutine-local-storage.hxx
include/sched/tag.hh
)

set(SCHED_HEADERS_LIBCOROUTINE
include/sched/libcoroutine/Coro.h
include/sched/libcoroutine/amd64-ucontext.h
include/sched/libcoroutine/taskimpl.h
include/sched/libcoroutine/power-ucontext.h
include/sched/libcoroutine/Common.h
include/sched/libcoroutine/386-ucontext.h
)
qi_install_header(${SCHED_HEADERS} SUBFOLDER sched)
qi_install_header(${SCHED_HEADERS_LIBCOROUTINE} SUBFOLDER sched/libcoroutine)

set(SERIALIZE_SOURCES
lib/serialize/binary-i-serializer.cc
lib/serialize/binary-o-serializer.cc
lib/serialize/exception.cc
lib/serialize/o-serializer.cc
)

set(SERIALIZE_HEADERS
include/serialize/o-serializer.hh
include/serialize/exception.hh
include/serialize/local.mk
include/serialize/xml-i-serializer.hh
include/serialize/xml-o-serializer.hh
include/serialize/binary-i-serializer.hh
include/serialize/serialize.hh
include/serialize/fwd.hh
include/serialize/i-serializer.hh
include/serialize/binary-o-serializer.hxx
include/serialize/o-serializer.hxx
include/serialize/binary-i-serializer.hxx
include/serialize/binary-o-serializer.hh
include/serialize/i-serializer.hxx
include/serialize/export.hh
)

qi_install_header(${SERIALIZE_HEADERS} SUBFOLDER serialize)


qi_create_lib(port
  SRC ${PORT_SOURCES} ${PORT_HEADERS}
  DEPENDS BOOST BOOST_FILESYSTEM BOOST_DATE_TIME BOOST_SYSTEM BOOST_THREAD)

if (NOT APPLE AND NOT WIN32)
 qi_use_lib(port RT)
endif()

qi_stage_lib(port)

qi_create_lib(sched
  SRC ${SCHED_SOURCES}
  DEPENDS port BOOST BOOST_SIGNALS BOOST_THREAD)

qi_stage_lib(sched)

qi_create_lib(serialize
  SRC ${SERIALIZE_SOURCES}
  DEPENDS port BOOST BOOST_THREAD
)
qi_stage_lib(serialize)





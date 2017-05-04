cmake_minimum_required(VERSION 2.6)
project(monik C CXX)

if(UNIX)
set(CMAKE_CXX_FLAGS "-Wall -std=c++14")
endif(UNIX)

message(STATUS "CXXFLAGS: ${CMAKE_CXX_FLAGS}")

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
message(FATAL_ERROR "In-source builds not allowed.")
endif()

# Default build is in Release mode
if(NOT CMAKE_BUILD_TYPE)
    if(MSVC)
        set(CMAKE_BUILD_TYPE "Debug")
    else()
        set(CMAKE_BUILD_TYPE "Release")
    endif(MSVC)
endif(NOT CMAKE_BUILD_TYPE)

if(WIN32)
  message(STATUS "WIN32: ${WIN32}")
  add_definitions(-DMONIK_OS_WIN32 -DWIN32)

endif(WIN32)

if(UNIX)
  message(STATUS "UNIX: ${UNIX}")
  add_definitions(-DMONIK_OS_UNIX)
endif(UNIX) 

#if(UNIX) need for libprotobuf_files
#find_package(Threads REQUIRED)
#if (CMAKE_USE_PTHREADS_INIT)
#  add_definitions(-DHAVE_PTHREAD)
#endif (CMAKE_USE_PTHREADS_INIT)
#endif(UNIX) 

if(APPLE)
  message(STATUS "APPLE: ${APPLE}")
  add_definitions(-DMONIK_OS_APPLE)
endif(APPLE) 

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DMONIK_DEBUG=1)
else()
    add_definitions(-DMONIK_DEBUG=0)
endif()

#set the default path for built executables to the "bin" directory
#set the default path for built libraries to the "lib" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

message( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )

if(WIN32)
add_definitions(-DCMAKE_CURRENT_SOURCE_DIR=R\"\(${CMAKE_CURRENT_SOURCE_DIR}\)\")
endif(WIN32)

#set(protobuf_source_dir monik/third_party/libprotobuf )

include_directories(
    ${ADDITIONAL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    monik/log/third_party
    "${DEPLOY_DIR}/include"
    #${protobuf_source_dir}/src
  )

set( MONIK_SOFTWARE_MAIN
  monik/software/main.cpp
  monik/software/cmdLine.h
  )

set( MONIK_SOURCE_COMMON
  monik/common/stdcommon.h
  monik/common/common.h
  monik/common/config.h
  monik/common/config.cpp
  monik/common/noncopyable.h
  monik/common/quantity.h
  monik/common/array_enum.h
  monik/common/static.h
  monik/common/locale.h
  monik/common/format.h
  monik/common/format.cpp
  monik/common/static_type.h
  monik/common/type_seq.h
  monik/common/time_util.h
  monik/common/time_util.cpp
  monik/common/algorithm.h
  monik/common/algorithm.cpp
  monik/common/singleton.h
  monik/common/circular_buffer.h
  monik/common/circular_buffer.cpp
  monik/common/forward_queue.h 
  monik/common/forward_queue.cpp
  monik/common/static.cpp
  )

set( MONIK_SOURCE_LOG
  monik/log/spinlock.h
  monik/log/spinlock.cpp
  monik/log/thread.h
  monik/log/message.h
  monik/log/message.cpp
  monik/log/log_define.h
  monik/log/log_define.cpp
  monik/log/log_buffer.h
  monik/log/log_buffer.cpp
  monik/log/log_thread.h
  monik/log/log_thread.cpp
  monik/log/timer_thread.h
  monik/log/timer_thread.cpp
  monik/log/log_util.h
  monik/log/log_util.cpp
  monik/log/logger.h
  monik/log/logger.cpp 
  monik/log/console_log.h
  monik/log/console_log.cpp
  monik/log/file_log.h
  monik/log/file_log.cpp
  monik/log/remote_log.h
  monik/log/remote_log.cpp
  monik/log/monik_log.h
  monik/log/monik_log.cpp
  monik/log/keepalive_log.h
  monik/log/keepalive_log.cpp
  monik/log/logger_config.h
  monik/log/logger_config.cpp 
  monik/log/pattern_formatter.h
  monik/log/pattern_formatter.cpp
  )

set( MONIK_SOURCE_LOG_JSON
  monik/log/third_party/json/allocator.h
  monik/log/third_party/json/assertions.h
  monik/log/third_party/json/autolink.h
  monik/log/third_party/json/config.h
  monik/log/third_party/json/features.h
  monik/log/third_party/json/forwards.h
  monik/log/third_party/json/json.h
  monik/log/third_party/json/json_reader.cpp
  monik/log/third_party/json/json_tool.h
  monik/log/third_party/json/json_value.cpp
  monik/log/third_party/json/json_valueiterator.inl
  monik/log/third_party/json/json_writer.cpp
  monik/log/third_party/json/reader.h
  monik/log/third_party/json/value.h
  monik/log/third_party/json/version.h
  monik/log/third_party/json/writer.h  
  monik/log/third_party/json/json_parser.h
  monik/log/third_party/json/json_parser.cpp
  monik/log/third_party/json/json_test.cpp
  )
  
set( MONIK_SOURCE_LOG_AMQP
  monik/log/third_party/amqp/amqp_config.h
  monik/log/third_party/amqp/AmqpException.h
  monik/log/third_party/amqp/BasicMessage.h
  monik/log/third_party/amqp/Channel.h
  monik/log/third_party/amqp/ChannelImpl.h
  monik/log/third_party/amqp/Envelope.h
  monik/log/third_party/amqp/MessageReturnedException.h
  monik/log/third_party/amqp/Table.h
  monik/log/third_party/amqp/TableImpl.h
  monik/log/third_party/amqp/MessageSender.h
  monik/log/third_party/amqp/AmqpException.cpp
  monik/log/third_party/amqp/BasicMessage.cpp
  monik/log/third_party/amqp/Channel.cpp
  monik/log/third_party/amqp/ChannelImpl.cpp
  monik/log/third_party/amqp/Envelope.cpp
  monik/log/third_party/amqp/MessageReturnedException.cpp
  monik/log/third_party/amqp/Table.cpp
  monik/log/third_party/amqp/TableImpl.cpp
  monik/log/third_party/amqp/MessageSender.cpp
  )  

if (Rabbitmqc_LIBRARY)
set( MONIK_SOURCE_PROTOBUF
  monik/protobuf/monik.pb.cc
  monik/protobuf/monik.pb.h
  )
else()
set( MONIK_SOURCE_PROTOBUF )
endif()  
  
set( MONIK_SOURCE_FILES
  ${MONIK_SOURCE_COMMON}
  ${MONIK_SOURCE_LOG}
  ${MONIK_SOURCE_LOG_JSON}
  )

#set(libprotobuf_files
  #${protobuf_source_dir}/src/google/protobuf/any.cc
  #${protobuf_source_dir}/src/google/protobuf/any.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/api.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/compiler/importer.cc
  #${protobuf_source_dir}/src/google/protobuf/compiler/parser.cc
  #${protobuf_source_dir}/src/google/protobuf/descriptor.cc
  #${protobuf_source_dir}/src/google/protobuf/descriptor.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/descriptor_database.cc
  #${protobuf_source_dir}/src/google/protobuf/duration.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/dynamic_message.cc
  #${protobuf_source_dir}/src/google/protobuf/empty.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/extension_set_heavy.cc
  #${protobuf_source_dir}/src/google/protobuf/field_mask.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/generated_message_reflection.cc
  #${protobuf_source_dir}/src/google/protobuf/io/gzip_stream.cc
  #${protobuf_source_dir}/src/google/protobuf/io/printer.cc
  #${protobuf_source_dir}/src/google/protobuf/io/strtod.cc
  #${protobuf_source_dir}/src/google/protobuf/io/tokenizer.cc
  #${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl.cc
  #${protobuf_source_dir}/src/google/protobuf/map_field.cc
  #${protobuf_source_dir}/src/google/protobuf/message.cc
  #${protobuf_source_dir}/src/google/protobuf/reflection_ops.cc
  #${protobuf_source_dir}/src/google/protobuf/service.cc
  #${protobuf_source_dir}/src/google/protobuf/source_context.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/struct.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/mathlimits.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/substitute.cc
  #${protobuf_source_dir}/src/google/protobuf/text_format.cc
  #${protobuf_source_dir}/src/google/protobuf/timestamp.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/type.pb.cc
  #${protobuf_source_dir}/src/google/protobuf/unknown_field_set.cc
  #${protobuf_source_dir}/src/google/protobuf/util/field_comparator.cc
  #${protobuf_source_dir}/src/google/protobuf/util/field_mask_util.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/datapiece.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/default_value_objectwriter.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/error_listener.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/field_mask_utility.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/json_escaping.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/json_objectwriter.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/json_stream_parser.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/object_writer.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/proto_writer.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectsource.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectwriter.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/type_info.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/type_info_test_helper.cc
  #${protobuf_source_dir}/src/google/protobuf/util/internal/utility.cc
  #${protobuf_source_dir}/src/google/protobuf/util/json_util.cc
  #${protobuf_source_dir}/src/google/protobuf/util/message_differencer.cc
  #${protobuf_source_dir}/src/google/protobuf/util/time_util.cc
  #${protobuf_source_dir}/src/google/protobuf/util/type_resolver_util.cc
  #${protobuf_source_dir}/src/google/protobuf/wire_format.cc
  #${protobuf_source_dir}/src/google/protobuf/wrappers.pb.cc
#)  

#set(libprotobuf_lite_files
  #${protobuf_source_dir}/src/google/protobuf/arena.cc
  #${protobuf_source_dir}/src/google/protobuf/arenastring.cc
  #${protobuf_source_dir}/src/google/protobuf/extension_set.cc
  #${protobuf_source_dir}/src/google/protobuf/generated_message_util.cc
  #${protobuf_source_dir}/src/google/protobuf/io/coded_stream.cc
  #${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream.cc
  #${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl_lite.cc
  #${protobuf_source_dir}/src/google/protobuf/message_lite.cc
  #${protobuf_source_dir}/src/google/protobuf/repeated_field.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/bytestream.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/common.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/int128.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/once.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/status.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/statusor.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/stringpiece.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/stringprintf.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/structurally_valid.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/strutil.cc
  #${protobuf_source_dir}/src/google/protobuf/stubs/time.cc
  #${protobuf_source_dir}/src/google/protobuf/wire_format_lite.cc
#)

source_group(common FILES ${MONIK_SOURCE_COMMON} )
source_group(software FILES ${MONIK_SOFTWARE_MAIN} )
source_group(log FILES ${MONIK_SOURCE_LOG} )
source_group(third_party/json FILES ${MONIK_SOURCE_LOG_JSON} )
source_group(third_party/amqp FILES ${MONIK_SOURCE_LOG_AMQP} )
source_group(protobuf FILES ${MONIK_SOURCE_PROTOBUF} )  
#source_group(libprotobuf_files FILES ${libprotobuf_files} )
#source_group(libprotobuf_lite_files FILES ${libprotobuf_lite_files} )

link_directories(
   "${DEPLOY_DIR}/lib" 
)

if (UNIX)
link_directories(
   "${DEPLOY_DIR}/lib/x86_64-linux-gnu" 
)
endif (UNIX)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
set(LIB_TYPE "d")
endif()

message(STATUS "LIB_TYPE: ${LIB_TYPE}")

if(WIN32)
set(PROTOBUF_LIBS libprotobuf${LIB_TYPE} )
else(WIN32)
set(PROTOBUF_LIBS -lprotobuf )
endif(WIN32)

message(STATUS "PROTOBUF_LIBS: ${PROTOBUF_LIBS}")

if (Rabbitmqc_LIBRARY)
message(STATUS "build monik/log/third_party/amqp")
message(STATUS "build monik/protobuf")
add_definitions(-DMONIK_INCLUDE_AMQP=1)
add_library(monikcpp${LIB_TYPE} STATIC 
    ${MONIK_SOURCE_FILES}
    ${MONIK_SOURCE_LOG_AMQP}
    ${MONIK_SOURCE_PROTOBUF} 
    #${libprotobuf_files}
    #${libprotobuf_lite_files}
    )
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_executable(test_monik${LIB_TYPE}
        ${MONIK_SOFTWARE_MAIN}
        ${MONIK_SOURCE_FILES}
        ${MONIK_SOURCE_LOG_AMQP}
        ${MONIK_SOURCE_PROTOBUF}
        #${libprotobuf_files}
        #${libprotobuf_lite_files}
        )
endif()
else(Rabbitmqc_LIBRARY)
message(STATUS "ignore monik/log/third_party/amqp")
message(STATUS "ignore monik/protobuf")
add_definitions(-DMONIK_INCLUDE_AMQP=0)
add_library( monikcpp${LIB_TYPE} STATIC 
    ${MONIK_SOURCE_FILES}
    )
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_executable(test_monik${LIB_TYPE}
        ${MONIK_SOFTWARE_MAIN}
        ${MONIK_SOURCE_FILES}
        )
endif()
endif(Rabbitmqc_LIBRARY)

if (UNIX)
target_link_libraries(monikcpp${LIB_TYPE} -lpthread)
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
target_link_libraries(test_monik${LIB_TYPE} -lpthread)
endif()
endif (UNIX)

if (WIN32)
set(SOCKET_LIBRARY ws2_32)
endif()

if (Rabbitmqc_LIBRARY)
message(STATUS "Rabbitmqc_LIBRARY: ${Rabbitmqc_LIBRARY}") #librabbitmq.4.lib or rabbitmq
message(STATUS "SOCKET_LIBRARY: ${SOCKET_LIBRARY}")
target_link_libraries(monikcpp${LIB_TYPE} ${Rabbitmqc_LIBRARY} ${SOCKET_LIBRARY} ${PROTOBUF_LIBS})
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_link_libraries(test_monik${LIB_TYPE} ${Rabbitmqc_LIBRARY} ${SOCKET_LIBRARY} ${PROTOBUF_LIBS})
endif()
else(Rabbitmqc_LIBRARY)
target_link_libraries(monikcpp${LIB_TYPE})
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_link_libraries(test_monik${LIB_TYPE})
endif()
endif(Rabbitmqc_LIBRARY)


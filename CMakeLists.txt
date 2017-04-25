cmake_minimum_required(VERSION 2.6)
project(monik C CXX)

set(MONIK_UNIX_CXX_FLAGS "" CACHE STRING "more C++ compiler flags if UNIX")
message(STATUS "MONIK_UNIX_CXX_FLAGS: ${MONIK_UNIX_CXX_FLAGS}")

if(UNIX)
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 ${MONIK_UNIX_CXX_FLAGS}")
endif(UNIX)

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
  IF (MSVC)
    ADD_DEFINITIONS(/bigobj)
  ENDIF (MSVC)  
endif(WIN32)

if(UNIX)
  message(STATUS "UNIX: ${UNIX}")
  add_definitions(-DMONIK_OS_UNIX)
endif(UNIX) 

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

MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )

if(WIN32)
add_definitions(-DCMAKE_CURRENT_SOURCE_DIR=R\"\(${CMAKE_CURRENT_SOURCE_DIR}\)\")
endif(WIN32)

if (Boost_INCLUDE_DIR) #optional
message(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
# Require boost system library
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS date_time chrono system )
include_directories(${Boost_INCLUDE_DIR})
add_definitions(-DMONIK_INCLUDE_BOOST=1)
else()
message(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
endif()

if (WIN32)
set(SOCKET_LIBRARY ws2_32)
endif ()

include_directories(
    ${ADDITIONAL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    monik/log/third_party
    "${DEPLOY_DIR}/include"
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

set( MONIK_SOURCE_LOG_PROTOBUF
  monik/protobuf/monik.pb.cc
  monik/protobuf/monik.pb.h
  )
  
set( MONIK_SOURCE_FILES
  ${MONIK_SOURCE_COMMON}
  ${MONIK_SOURCE_LOG}
  ${MONIK_SOURCE_LOG_JSON}
  )

source_group(common FILES ${MONIK_SOURCE_COMMON} )
source_group(software FILES ${MONIK_SOFTWARE_MAIN} )
source_group(log FILES ${MONIK_SOURCE_LOG} )
source_group(third_party/json FILES ${MONIK_SOURCE_LOG_JSON} )
source_group(third_party/amqp FILES ${MONIK_SOURCE_LOG_AMQP} )

link_directories(
   "${DEPLOY_DIR}/lib" 
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
set(LIB_TYPE "d")
endif()

message(STATUS "LIB_TYPE: ${LIB_TYPE}")

if (Rabbitmqc_LIBRARY)
if(WIN32)
set(PROTOBUF_LIBS libprotobuf${LIB_TYPE} libprotobuf-lite${LIB_TYPE} libprotoc${LIB_TYPE})
else(WIN32)
set(PROTOBUF_LIBS -lprotobuf -lprotobuf-lite -lprotoc)
#set(PROTOBUF_LIBS -lprotobuf${LIB_TYPE} -lprotobuf-lite${LIB_TYPE} -lprotoc${LIB_TYPE})
endif(WIN32)
endif()

if (Rabbitmqc_LIBRARY)
message(STATUS "build monik/log/third_party/amqp")
message(STATUS "build monik/protobuf")
add_definitions(-DMONIK_INCLUDE_AMQP=1)
add_library( monikcpp${LIB_TYPE} STATIC 
  ${MONIK_SOURCE_FILES}
  ${MONIK_SOURCE_LOG_AMQP}
  ${MONIK_SOURCE_LOG_PROTOBUF}
  )
add_executable( test_monik${LIB_TYPE}
  ${MONIK_SOFTWARE_MAIN}
  ${MONIK_SOURCE_FILES}
  ${MONIK_SOURCE_LOG_AMQP}
  ${MONIK_SOURCE_LOG_PROTOBUF}
  )
source_group(protobuf FILES ${MONIK_SOURCE_LOG_PROTOBUF} )  
else()
message(STATUS "ignore monik/log/third_party/amqp")
message(STATUS "ignore monik/protobuf")
add_definitions(-DMONIK_INCLUDE_AMQP=0)
add_library( monikcpp${LIB_TYPE} STATIC ${MONIK_SOURCE_FILES} )
add_executable( test_monik${LIB_TYPE} ${MONIK_SOFTWARE_MAIN} ${MONIK_SOURCE_FILES} )
endif()

if (UNIX)
target_link_libraries(test_monik${LIB_TYPE} -lpthread)
endif (UNIX)

if (Boost_LIBRARIES) #optional
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
target_link_libraries(test_monik${LIB_TYPE} ${Boost_LIBRARIES})
endif()

if (Rabbitmqc_LIBRARY)
message(STATUS "Rabbitmqc_LIBRARY: ${Rabbitmqc_LIBRARY}") #librabbitmq.4.lib or rabbitmq
message(STATUS "SOCKET_LIBRARY: ${SOCKET_LIBRARY}")
target_link_libraries(test_monik${LIB_TYPE} ${Rabbitmqc_LIBRARY} ${SOCKET_LIBRARY} ${PROTOBUF_LIBS})
target_link_libraries(monikcpp${LIB_TYPE} ${Rabbitmqc_LIBRARY} ${SOCKET_LIBRARY} ${PROTOBUF_LIBS})
endif()

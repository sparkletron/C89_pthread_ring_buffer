cmake_minimum_required(VERSION 3.1.0)

set(LIB_NAME "ringBuffer")

if(NOT DEFINED BUILD_EXAMPLES)
  set(BUILD_EXAMPLES OFF)
endif()

project(${LIB_NAME} VERSION 1.6.0 DESCRIPTION "Thread safe C ring buffer")

file(GLOB SOURCES "src/*.c")

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_library(${LIB_NAME} ${SOURCES})

set_target_properties(${LIB_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1 PUBLIC_HEADER ringBuffer.h)

target_include_directories(${LIB_NAME} PUBLIC .)

include(GNUInstallDirs)

install(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(!CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr/local/" CACHE PATH "..." FORCE)
endif()

if(BUILD_EXAMPLES)
  file(GLOB EXAMPLE_SOURCES eg/src/*.c)

  foreach(app_source ${EXAMPLE_SOURCES})
      get_filename_component(app_name ${app_source} NAME_WLE)
      add_executable(${app_name} ${app_source})
      target_link_libraries(${app_name} ${LIB_NAME})
  endforeach(app_source ${EXAMPLE_SOURCES})

endif()

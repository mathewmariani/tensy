# General Purpose CMakeLists.txt

# cmake build script
cmake_minimum_required(VERSION 2.8)

# set compiler
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# project
project(tensy)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# binary directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# options
set(REQUIRES_SDL2 false CACHE BOOL "Includes SDL2 to project")
set(REQUIRES_SDL2_IMAGE false CACHE BOOL "Includes SDL2_Image to project")
set(BUILD_CONFIG false CACHE BOOL "Builds config.h")
set(BUILD_TESTS false CACHE BOOL "Builds unit testing executable from ./tests")
set(BUILD_EXE false CACHE BOOL "Adds an executable target (Requires main.cpp).")
set(BUILD_LIBRARY false CACHE BOOL "Adds a library target.")
set(BUILD_SHARED_LIBS false CACHE BOOL "Adds proper notation")

if(BUILD_CONFIG)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h
  )
endif()

# add include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

if(REQUIRES_SDL2)
  find_package(SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR})
endif()

if(REQUIRES_SDL2_IMAGE)
  find_package(REQUIRES_SDL2_IMAGE REQUIRED)
  include_directories(${SDL2_INCLUDE_DIR})
endif()

# building
file(GLOB_RECURSE PROJECT_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

file(GLOB_RECURSE PROJECT_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE PROJECT_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

if(BUILD_EXE)
	add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
endif()

if(BUILD_LIBRARY)
	if(BUILD_SHARED_LIBS)
	  set(CMAKE_DEBUG_POSTFIX "_d")
	else()
	  add_definitions(-DSTATIC_LIB)
	  set(CMAKE_DEBUG_POSTFIX "_d_s")
	  set(CMAKE_RELEASE_POSTFIX "_s")
	endif()

	add_library(${PROJECT_NAME} ${PROJECT_FILES})
endif()

# seperate into source and header files
foreach(FILE ${PROJECT_SRC})
  get_filename_component(PARENT_DIR "${FILE}" PATH)

  # skip src or include and changes /'s to \\'s
  string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
  string(REPLACE "/" "\\" GROUP "${GROUP}")
	message(STATUS "${PARENT_DIR}")
  # group into "Source Files" and "Header Files"
  if ("${FILE}" MATCHES ".*\\.cpp")
    set(GROUP "Source Files\\${GROUP}")
  elseif("${FILE}" MATCHES ".*\\.h")
    set(GROUP "Header Files\\${GROUP}")
  endif()

  source_group("${GROUP}" FILES "${FILE}")
endforeach()

# installation
if(REQUIRES_SDL2)
  target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
endif()

if(REQUIRES_SDL2_IMAGE)
  target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARY})
endif()

if(BUILD_TESTS)
	enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES OSX_ARCHITECTURES "i386;x86_64;")
endif()

INSTALL(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}
	ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH}
	LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH}
)

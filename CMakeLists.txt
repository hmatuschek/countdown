cmake_minimum_required(VERSION 2.8.8)
project(countdown)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
include(InstallHeadersWithDirectory)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})

INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Multimedia_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/src)

set(LIBS ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Multimedia_LIBRARIES})

# System specific libraries
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
 FIND_LIBRARY(COREFOUNDATION NAMES CoreFoundation)
 FIND_LIBRARY(IOKIT NAMES IOKit)
 set(LIBS ${LIBS} ${IOKIT} ${COREFOUNDATION})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}  -O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ggdb")

# Sources...
add_subdirectory(src)

# Source distribution packages:
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES
  "/build/;/doc/;/dist/;/.git/;~$;.qm;.user$;${CPACK_SOURCE_IGNORE_FILES}")

#
# Install logo and .desktop file under Linux:
#
IF(UNIX AND NOT APPLE)
 INSTALL(FILES shared/countdown.svg
         DESTINATION share/icons)
 INSTALL(FILES shared/countdown.desktop
         DESTINATION share/applications)
ENDIF(UNIX AND NOT APPLE)

include(CPack)

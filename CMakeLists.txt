cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

file(READ "version.txt" ver)
string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})
string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
set(ver_patch ${CMAKE_MATCH_1})

project(
  root
  VERSION "${ver_major}.${ver_minor}.${ver_patch}"
  LANGUAGES CXX
  HOMEPAGE_URL "http://hangingman.github.io/wxMMDViewer/"
  DESCRIPTION "Simple OpenGL testing application powered by wxWidgets")
set(PACKAGE_NAME "wxMMDViewer" CACHE INTERNAL "")
set(PACKAGE_STRING "wxMMDViewer ${PROJECT_VERSION}" CACHE INTERNAL "")
set(PACKAGE_VERSION "${PROJECT_VERSION}" CACHE INTERNAL "")

# detect OS
include(IdentifyOS)
if(${CMAKE_OS_NAME} MATCHES "OSX")
  set(ENV{OPENSSL_ROOT_DIR} "/usr/local/opt/openssl")
  set(ENV{HOMEBREW_NO_AUTO_UPDATE} 1)
endif()

set(UPSTREAM_URL
    "https://github.com/hangingman/wxMMDViewer"
    CACHE INTERNAL "")
set(PACKAGE_BUGREPORT
    "idiotpanzer@gmail.com"
    CACHE INTERNAL "")
set(DEB_STANDARDS_VERSION
    3.9.3
    CACHE INTERNAL "")

find_package(
  wxWidgets
  COMPONENTS aui
             base
             core
             gl
  REQUIRED)
set(wxWidgets_CONFIG_OPTIONS --toolkit=aui,base,core,gl)

find_package(ZLIB REQUIRED)
find_package(Iconv REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# for debug
option(ENABLE_BACKWARD "Enable backward-cpp" OFF)
if(ENABLE_BACKWARD)
  add_subdirectory(backward-cpp)
endif()
add_subdirectory(src)

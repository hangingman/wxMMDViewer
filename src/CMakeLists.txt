project(wxmmdviewer CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")

set(wxmmdviewer_SRCS
  main.cpp
  main.hpp
  enumstring.hpp
  wxmmdviewer.cpp
  wxmmdviewer.hpp
  basicglpane.cpp
  basicglpane.hpp
  common.hpp
  wxmmdutil.hpp
  wxmmdutil.cpp
  ../babel/babel.cpp
  ../babel/babel.h
  ../kaitai_runtime/kaitai/kaitaistream.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/pmd.cpp)

# kaitai struct
add_custom_command(
  OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/pmd.cpp
  COMMAND ksc -t cpp_stl --cpp-standard 11 ${CMAKE_CURRENT_SOURCE_DIR}/rc/pmd.ksy
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rc/pmd.ksy
)


if(${CMAKE_OS_NAME} MATCHES "OSX")
  # Generate Info.plist
  configure_file(${root_SOURCE_DIR}/Info.plist.in ${root_BINARY_DIR}/Info.plist @ONLY)
  # Resources
  file(GLOB RC_FILES ${wxmmdviewer_SOURCE_DIR}/rc/*)
  # Binary
  add_executable(wxmmdviewer MACOSX_BUNDLE ${wxmmdviewer_SRCS} ${RC_FILES})

  set_target_properties(wxmmdviewer PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${root_BINARY_DIR}/Info.plist
    RESOURCE "${RC_FILES}")
else()
  add_executable(wxmmdviewer ${wxmmdviewer_SRCS})
endif()


include("${wxWidgets_USE_FILE}")
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${Iconv_INCLUDE_DIRS})
include_directories(${root_SOURCE_DIR}/babel)
include_directories(${root_SOURCE_DIR}/kaitai_runtime)
include_directories(${CMAKE_BINARY_DIR}/src)

target_link_libraries(wxmmdviewer ${wxWidgets_LIBRARIES})
target_link_libraries(wxmmdviewer ${ZLIB_LIBRARIES})
target_link_libraries(wxmmdviewer ${Iconv_LIBRARIES})
target_link_libraries(wxmmdviewer ${OPENGL_LIBRARIES})
target_link_libraries(wxmmdviewer ${GLUT_LIBRARIES})
target_link_libraries(wxmmdviewer ${GLUT_LIBRARIES})
add_definitions(-DKS_STR_ENCODING_ICONV)

#
# Debianパッケージの設定
#
if(${CMAKE_OS_NAME} MATCHES "(Debian|Ubuntu)")
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Hiroyuki Nagata")
  set(CPACK_PACKAGE_NAME "wxmmdviewer")
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxbase3.0-0v5, libwxgtk3.0-0v5")

  set(WXMMDVIEWER_EXEC_RC_DIR "/usr/share/pixmaps/wxmmdviewer")

  install(TARGETS wxmmdviewer RUNTIME DESTINATION "/usr/bin/")
  file(GLOB RC_FILES ${wxmmdviewer_SOURCE_DIR}/rc/*)
  install(FILES ${RC_FILES} DESTINATION ${WXMMDVIEWER_EXEC_RC_DIR})

  configure_file(${root_SOURCE_DIR}/misc/wxmmdviewer.desktop.in
    ${CMAKE_BINARY_DIR}/misc/wxmmdviewer.desktop @ONLY)
  install(FILES ${CMAKE_BINARY_DIR}/misc/wxmmdviewer.desktop
    DESTINATION /usr/share/applications/)

  include(CPack)
endif()

#
# Mac用Bundleパッケージの設定
#
if(${CMAKE_OS_NAME} MATCHES "OSX")

  set(CPACK_DMG_FORMAT "UDBZ")
  set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}-osx64-${PACKAGE_VERSION}")
  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-osx64-${PACKAGE_VERSION}")
  set(CPACK_PACKAGE_ICON "${root_SOURCE_DIR}/src/rc/wxmmdviewer.icns")
  set(CPACK_SYSTEM_NAME "OSX")

  # Note Mac specific extension .app
  set(BUNDLE "\${CMAKE_INSTALL_PREFIX}/wxmmdviewer.app")

  # Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}
  install(TARGETS wxmmdviewer
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime)

  # Directories to look for dependencies
  set(BUNDLE_LIB_DIRS
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/libvmdconv/src)

  # ref: https://gitlab.kitware.com/cmake/cmake/-/issues/19663
  install(CODE "include(BundleUtilities)
    set(BU_CHMOD_BUNDLE_ITEMS TRUE)
    fixup_bundle(\"${BUNDLE}\" \"\" \"${BUNDLE_LIB_DIRS}\")")

  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_BUNDLE_NAME "WxMMDViewer")
  set(CPACK_BUNDLE_ICON "${root_SOURCE_DIR}/src/rc/wxmac.icns")
  set(CPACK_BUNDLE_PLIST "${CMAKE_BINARY_DIR}/Info.plist")

  include(CPack)
endif()

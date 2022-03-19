project(vmdconv CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb")

set(vmdconv_HEADERS
  wx/wxsqlite3.h
  wx/wxsqlite3def.h
  wx/wxsqlite3dyn.h
  wx/wxsqlite3opt.h)
set(vmdconv_SRCS
  wxsqlite3.cpp)

add_library(vmdconv SHARED ${vmdconv_SRCS})

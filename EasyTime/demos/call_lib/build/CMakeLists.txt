
cmake_policy(VERSION 3.6)
project(demo_easytime VERSION 0.1)
cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)  #编译.ui程序相关
#set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
SET(PATH_COM_REPO "/home/sxy/Work/com_repo/scripts/com_repo_init/../../" CACHE STRING "Some user-specified option")
SET(PATH_COMPILER_SDX "" CACHE STRING "Some user-specified option")
set(DEPEND_ON_MACRO_GLOG ON)


#设置子项目，假设所有库都位于同一层级，且具有相同的文件结构


#list(CMAKE_PREFIX_PATH
#  APPEND
#  /home/sxy/Work/com_repo/com_lib/opencv_3_4_1/build/opencv_3_4_1/opencv_3_4_1-build
#  )

# 由于opencv可能安装再不同路径，因此需要传入参数进行配置
set(CMAKE_PREFIX_PATH  "${CMAKE_PREFIX_PATH};/home/sxy/Software/opencv-3.4.1/")



# 位于<com_repo>/com_lib的库
set(LIST_COM_LIB
  #  cjsonobject
  #  glog
  #  opencv_3_4_1
  )

# 位于<com_repo>/private_lib的库
set(LIST_PRIVATE_LIB
  # fod
  #  encrypt
  )

# 位于<com_repo>/public_lib的库
set(LIST_PUBLIC_LIB
  #  uio
  #  msgchannel
  #  socket_channel
#   toml
EasyTime
  )

# 本地的模块
set(LIST_LOCAL_LIB
  #  def
  #  basic
  #  tool
  #  json_explain
  #  http_def
  )

if(DEPEND_ON_MACRO_GLOG)
  list(FIND LIST_PUBLIC_LIB macro_glog find_glog)
  if(${find_glog} STREQUAL -1)
    list (APPEND LIST_PUBLIC_LIB macro_glog)
  endif()
  include(${PATH_COM_REPO}/public_lib/macro_glog/cmake/macro_glog_option.cmake)
endif()

find_package(Qt5 COMPONENTS
  Core
  Gui
  Network
  Widgets
  LinguistTools
  Quick
  Qml
  WebSockets
  Concurrent
#  PrintSupport
  REQUIRED)

# 编译选项
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
##编译输出路径
#set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

#预设<com_repo>路径 使用cmake 时可以通过cmake . -DPATH_COM_REPO=<com_repo>进行自定义设置
SET(PATH_COM_REPO "/home/sxy/Work/com_repo/scripts/com_repo_init/../../" CACHE STRING "Some user-specified option")

#include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/common_define.cmake)

#包含文件，以便头文件能够被识别
# <>该处需要优化，可能具有风险
#include_directories(${PATH_COM_REPO})


#编译输出路径
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../../device/www/cgi-bin/)

STRING(REGEX REPLACE ".+/(.+)\\.*" "\\1" COMPILER_SUFFIX ${CMAKE_C_COMPILER})

#将依赖的库添加到当前项目
FOREACH(MYSUBLIB ${LIST_COM_LIB})
if(NOT TARGET ${MYSUBLIB})
  message("add_subdirectory ${MYSUBLIB}")
  add_subdirectory(${PATH_COM_REPO}/com_lib/${MYSUBLIB}/
    ${PATH_COM_REPO}/com_lib_out/${MYSUBLIB}_${COMPILER_SUFFIX}.out/)
endif()
ENDFOREACH(MYSUBLIB)

FOREACH(MYSUBLIB ${LIST_PRIVATE_LIB})
if(NOT TARGET ${MYSUBLIB})
  message("add_subdirectory ${MYSUBLIB}")
  add_subdirectory(${PATH_COM_REPO}/private_lib/${MYSUBLIB}/
    ${PATH_COM_REPO}/private_lib_out/${MYSUBLIB}_${COMPILER_SUFFIX}.out/)
endif()
ENDFOREACH(MYSUBLIB)

FOREACH(MYSUBLIB ${LIST_PUBLIC_LIB})
if(NOT TARGET ${MYSUBLIB})
  message("add_subdirectory ${MYSUBLIB}")
  add_subdirectory(${PATH_COM_REPO}/public_lib/${MYSUBLIB}/
    ${PATH_COM_REPO}/public_lib_out/${MYSUBLIB}_${COMPILER_SUFFIX}.out/)
endif()
ENDFOREACH(MYSUBLIB)

FOREACH(MYSUBLIB ${LIST_LOCAL_LIB})
if(NOT TARGET ${MYSUBLIB})
  message("add_subdirectory ${MYSUBLIB}")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../common/${MYSUBLIB}/ ${CMAKE_CURRENT_BINARY_DIR}/${MYSUBLIB}_${COMPILER_SUFFIX}.out/)
endif()
ENDFOREACH(MYSUBLIB)

message("OpenCV_LIBS:" ${OpenCV_LIBS})

add_executable (${PROJECT_NAME}
  ../src/main.cpp
  )

# 本项目链接相应的库
target_link_libraries(
  ${PROJECT_NAME}
  -lpthread
  ${LIST_COM_LIB}
  ${LIST_PRIVATE_LIB}
  ${LIST_PUBLIC_LIB}
  ${LIST_LOCAL_LIB}
  Qt5::Core
  Qt5::Gui
  Qt5::Network
  Qt5::Widgets
  Qt5::Quick
  Qt5::Qml
  Qt5::WebSockets
  Qt5::Concurrent
#  ${OpenCV_LIBS}
  )

##set( CMAKE_BUILD_TYPE Debug )

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(gtest-toml CXX)

#该程序需要使用gtest目录下的CMakeLists.txt运行

string(REGEX REPLACE ".*/\(.*\)" "\\1" CURDIR_NAME ${CMAKE_CURRENT_SOURCE_DIR})



set(MYTESTLIBS
  main
  #  encrypt
#  toml
  # def
  # tool
  # basic
  #  jpeg_op
  )

set(MYTPGPATHS
  main.cpp
  )


# 位于<com_repo>/com_lib的库
set(LIST_COM_LIB
  # cjsonobject
  gtest
  )

# 位于<com_repo>/private_lib的库
set(LIST_PRIVATE_LIB
  #  encrypt
  )

# 位于<com_repo>/public_lib的库
set(LIST_PUBLIC_LIB
  toml
  #  jpeg_op
  )

# check compile symbl

message("BUILD_SHARED_LIBS:" ${BUILD_SHARED_LIBS})


IF(${BUILD_SHARED_LIBS})
  set(TARGET_BUILD_TYPE "shared")
ELSE()
  set(TARGET_BUILD_TYPE "static")
ENDIF()
message("TARGET_BUILD_TYPE:" ${TARGET_BUILD_TYPE})


set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()
# 继承父项目的编译选项
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11" )

#预设<com_repo>路径 使用cmake 时可以通过cmake . -DPATH_COM_REPO=<com_repo>进行自定义设置
SET(PATH_COM_REPO "/home/sxy/Work/com_repo/scripts/com_repo_init/../../" CACHE STRING "Some user-specified option")

#包含文件，以便头文件能够被识别
# <>该处需要优化，可能具有风险
include_directories(${PATH_COM_REPO})

FOREACH(MYSUBLIB ${MYTESTLIBS})
if(NOT TARGET ${MYSUBLIB})
list(APPEND MYTPGPATHS ${CMAKE_CURRENT_SOURCE_DIR}/tpg_${MYSUBLIB}/tpg_${MYSUBLIB}.cpp)
endif()

ENDFOREACH(MYSUBLIB)

#将依赖的库添加到当前项目
FOREACH(MYSUBLIB ${LIST_COM_LIB})
if(NOT TARGET ${MYSUBLIB})
  message("add_subdirectory ${MYSUBLIB}")
  add_subdirectory(${PATH_COM_REPO}/com_lib/${MYSUBLIB}/ ${PATH_COM_REPO}/com_lib_out/${MYSUBLIB}_${COMPILER_SUFFIX}_${TARGET_BUILD_TYPE}.out/)
endif()
ENDFOREACH(MYSUBLIB)

FOREACH(MYSUBLIB ${LIST_PRIVATE_LIB})
if(NOT TARGET ${MYSUBLIB})
  message("add_subdirectory ${MYSUBLIB}")
  add_subdirectory(${PATH_COM_REPO}/private_lib/${MYSUBLIB}/ ${PATH_COM_REPO}/private_lib_out/${MYSUBLIB}_${COMPILER_SUFFIX}_${TARGET_BUILD_TYPE}.out/)
endif()
ENDFOREACH(MYSUBLIB)

FOREACH(MYSUBLIB ${LIST_PUBLIC_LIB})
if(NOT TARGET ${MYSUBLIB})
  message("add_subdirectory ${MYSUBLIB}")
  add_subdirectory(${PATH_COM_REPO}/public_lib/${MYSUBLIB}/ ${PATH_COM_REPO}/public_lib_out/${MYSUBLIB}_${COMPILER_SUFFIX}_${TARGET_BUILD_TYPE}.out/)
endif()
ENDFOREACH(MYSUBLIB)


add_executable(
  ${PROJECT_NAME}
  ${MYTPGPATHS}
  )

target_link_libraries(
  ${PROJECT_NAME}
  gtest_main
  ${LIST_COM_LIB}
  ${LIST_PRIVATE_LIB}
  ${LIST_PUBLIC_LIB}
  )

enable_testing()
add_test(
  NAME
  unit
  COMMAND
  ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}
  )


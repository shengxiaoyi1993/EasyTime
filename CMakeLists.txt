#<>cmake不同版本之间的兼容
# 该版本号应为胜任该模块编译的最小版本
cmake_minimum_required(VERSION 3.10)

#识别当前文件名，将当前文件夹名设置为项目名称
string(REGEX REPLACE ".*/\(.*\)" "\\1" CURDIR_NAME ${CMAKE_CURRENT_SOURCE_DIR})

project(EasyTime VERSION 0.1)

# 编译选项
set(CMAKE_CXX_STANDARD 11)

# main project may contain this moudle
# some resources may need to be copied to main project dir
# MAIN_BINARY_DIR
if(NOT CMAKE_BUILD_TYPE)
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  elseif (CMAKE_SYSTEM_NAME MATCHES "Win")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  endif()
endif()

if(MSVC)
  set(MAIN_BINARY_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
else()
  set(MAIN_BINARY_DIR ${CMAKE_BINARY_DIR})
endif()

set(CMAKE_GENERATOR "Visual Studio 15 2017 Win64")

# 将环境中设置的编译环境同步至cmake
SET(PATH_COM_REPO $ENV{PATH_COM_REPO})
SET(LD_LIBRARY_PATH $ENV{LD_LIBRARY_PATH})
SET(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH})

# !
# 自定义依赖
set(DEPEND_ON_QT OFF)
set(MACRO_NEED_QT_TRANSLATION OFF)
set(DEPEND_ON_OPENCV OFF)
set(DEPEND_ON_MACRO_GLOG OFF)



# !
#设置子项目，假设所有库都位于同一层级，且具有相同的文件结构
# 位于<com_repo>/com_lib的库
set(LIST_COM_LIB
  #    gtest
  )

# !
# 位于<com_repo>/private_lib的库
set(LIST_PRIVATE_LIB
  )

# !
# 位于<com_repo>/public_lib的库
set(LIST_PUBLIC_LIB
#  cppgc
#    macro_glog
  )

if(DEPEND_ON_MACRO_GLOG)
  list(FIND LIST_PUBLIC_LIB macro_glog find_glog)
  if(${find_glog} STREQUAL -1)
    list (APPEND LIST_PUBLIC_LIB macro_glog)
  endif()
  include(${PATH_COM_REPO}/public_lib/macro_glog/cmake/macro_glog_option.cmake)
endif()


if(DEPEND_ON_QT)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  set(CMAKE_AUTOUIC ON)  #编译.ui程序相关

  # !
  set(LIST_Qt5_SUPPORT_COMPONENTS
    Core
    Gui
    Widgets
    #    Core
    #    Gui
    #    Widgets
    #    Network
    #    WebSockets
    #    Quick
    #    Qml
    )

  FOREACH(Qt5_SUPPORT_COMPONENTS ${LIST_Qt5_SUPPORT_COMPONENTS})
  list (APPEND LIST_Qt5_SUPPORT_COMPONENTS_EX Qt5::${Qt5_SUPPORT_COMPONENTS})
  ENDFOREACH(LIST_Qt5_SUPPORT_COMPONENTS)

  # 需要自行挑选支持的模块
  find_package(Qt5 COMPONENTS
    ${LIST_Qt5_SUPPORT_COMPONENTS}
    REQUIRED)
else()
  set(MACRO_NEED_QT_TRANSLATION OFF)
endif()

if(DEPEND_ON_OPENCV)
  # !
  find_package(OpenCV  REQUIRED
    opencv_core
    opencv_highgui
    )

endif()


# # 将当前文件文件夹下的所有cpp和.h文件添加到一个列表，编译动态库时将包含所有cpp
file(GLOB_RECURSE CPP_LIB "${CMAKE_CURRENT_SOURCE_DIR}/src/*")
file(GLOB_RECURSE H_LIB "${CMAKE_CURRENT_SOURCE_DIR}/include/*")
file(GLOB_RECURSE CONFIG_CMAKE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*")

unset(QM_FILES)

if(MACRO_NEED_QT_TRANSLATION)
  find_package(Qt5 COMPONENTS
    LinguistTools
    REQUIRED)

  set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/lang_${PROJECT_NAME}_zh_CN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/lang_${PROJECT_NAME}_en.ts)

  set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION
    ${MAIN_BINARY_DIR}/translations)


  qt5_create_translation(QM_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CPP_LIB}
    ${H_LIB}
    ${TS_FILES}
    )
endif()

add_library(${PROJECT_NAME}
  ${CPP_LIB}
  ${H_LIB}
  ${QM_FILES}
  )

SET(EXPORT_TO_CURRENT_PROJECT OFF CACHE BOOL "Some user-specified option")

if(EXPORT_TO_CURRENT_PROJECT)
  SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/export)
endif()

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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

#<>为什么有版本限制
if (DEFINED CMAKE_VERSION AND NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.11")
  target_include_directories(${PROJECT_NAME}
    PUBLIC
#    "${CMAKE_CURRENT_SOURCE_DIR}/include")
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${INSTALL_INCLUDEDIR}>)


endif()

##设置安装路径,输出头文件与库文件
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/  DESTINATION  ${CMAKE_BINARY_DIR}/include)

#install(TARGETS ${PROJECT_NAME} DESTINATION  ${CMAKE_BINARY_DIR}/lib)

#install(FILES ${CONFIG_CMAKE} DESTINATION  ${CMAKE_BINARY_DIR}/share)


# 本本项目链接相应的库
target_link_libraries(
  ${PROJECT_NAME}
  ${LIST_COM_LIB}
  ${LIST_PRIVATE_LIB}
  ${LIST_PUBLIC_LIB}
  ${LIST_LOCAL_LIB}
  )

if(DEPEND_ON_QT)
  target_link_libraries(
    ${PROJECT_NAME}
    ${LIST_Qt5_SUPPORT_COMPONENTS_EX}
    )
endif()

if(DEPEND_ON_OPENCV)
  target_link_libraries(
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    )
endif()

include(CMakePackageConfigHelpers)
# 介于很多人可能对此部分不太熟悉，所以我还是讲一下这里
# configure_package_config_file这个函数可以用来生成-config.cmake文件。我们知道CMake通过查找-config.cmake
# 来支持find_package函数，然而如果我们把文件写死，那么就会导致我们在做路径相关的操作会很繁琐。这个函数可
# 以通过我们指定的一个模板(也就是上文中提到的test.in.cmake文件啦)来生成配置文件。
# 参数1：输入文件的路径
# 参数2：生成的位置，这里具体放在哪不重要，但是自己要能找到，后面还有用
# 参数3：安装位置，这个参数测试下来似乎并没有什么实际意义，但是这个参数必须给
# 关于工作路径：此处的工作路径在${VCPKG_ROOT}/buildtrees/test/x64-windows-(dbg|rel)下(这里我采用的编译器
# 为msvc,因此是x64-windows,其他编译器就在对应的目录下)，即CMake变量 ${CMAKE_CURRENT_BINARY_DIR}，因此不要
# 想着一步到位,将输出路径改为你想要的安装路径，否则你会遇到很多坑
configure_package_config_file(cmake/template/${PROJECT_NAME}.in.cmake
        ${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ./)


# write_basic_package_version_file 这个函数可以帮你生成一个-config-version.cmake函数，可以让你的库在执行
# find_package时支持指定版本，例如find_package(test 1.0.1 REQUIRED)
# 参数1：生成的路径
# 参数2：版本字符串，必须是 x.x.x 格式， 这里图省事，我就直接打上去了，正常来说应该要设置个变量的
# 参数3：这个自己去查文档啦，主要是设置你的库是否向下兼容或者主版本兼容。
# 重要的事：不要想着一步到位，直接把第二个参数设置为安装地址
# 重要的事：不要想着一步到位，直接把第二个参数设置为安装地址
# 重要的事：不要想着一步到位，直接把第二个参数设置为安装地址
# 原因同上
write_basic_package_version_file(
        ${PROJECT_NAME}-config-version.cmake
        VERSION 1.0.0
        COMPATIBILITY ExactVersion)
#
# 这里的工作路径变化为 D:\vcpkg\packages\test_x64-windows
# 这里把两个生成的文件安装到目标位置
# 这里需要注意的是，这个目标路径尽量不要改，就用share/项目名来生成，这个路径是vcpkg_cmake_config_fixup的默认路径
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION share/${PROJECT_NAME})
#
# 常规安装， 将生成目标分门别类放置到指定的目录下，并且导出相关配置文件
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
# 安装头文件，这里没什么好说的
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
# 安装导出文件，这一步会生成test-targets.cmake文件，用于帮助cmake链接你的库文件，后续要用。
install(EXPORT ${PROJECT_NAME}-targets NAMESPACE ${PROJECT_NAME}:: DESTINATION share/${PROJECT_NAME})

## 这部分代码用于运行创建gtest测试程序
## 在可执行目录下生成一个可执行文件
## cxx_library和cxx_executable为gtest提供的函数，因此必须包含gtest
#cxx_library(gtest_test "${cxx_strict}" gtest/main.cpp)
#target_link_libraries(gtest_test gtest ${PROJECT_NAME} -lgcov)
##gtest/tpg_main/中的文件需要以.cc为后缀
#cxx_executable(tpg_main gtest/tpg_main/ gtest_test)
##该步骤用于执行 需要运行make test后进行运行测试程序.Qt设置构建步骤后可以自动运行
## gtest若测试未通过,会停止并报错
#add_test(${PROJECT_NAME}_test tpg_main)
## 该语句用于设置测试程序的输出
## gtest可以不设置输出
##set_tests_properties(${PROJECT_NAME}_test PROPERTIES PASS_REGULAR_EXPRESSION "5")
## 采用 cmake --build . --target test 可以在编译时自动进行单元测试


#enable_testing()


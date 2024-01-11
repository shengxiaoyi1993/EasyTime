# 就得这么写，规定
@PACKAGE_INIT@

# 引入find_dependency函数，和find_package类似, 不过这个专门用于包的定义文件。具体可以参阅cmake文档
include(CMakeFindDependencyMacro)
# 这里将当前的目录添加到CMake查找模块的目录中
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# 上文提到假设test库引用了boost-locale
#find_dependency(Boost REQUIRED COMPONENTS locale)

# 这里需要说明的是，将CMake为我们生成的test-targets.cmake文件引入进来，这样CMake就可以帮助我们把我们的头文件和
# 库文件自动引入，因此这一步是关键步骤
include(${CMAKE_CURRENT_LIST_DIR}/EasyTime-targets.cmake)
# 这里将Boost的头文件也加进来
#if(@Boost_FOUND@)
#  include_directories(${Boost_INCLUDE_DIRS})
#endif()

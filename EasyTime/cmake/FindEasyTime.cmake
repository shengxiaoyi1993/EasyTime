
set(LIST_DEPENDENT_LIB
	cppgc
  )



find_path(toml_INCLUDE_DIR toml.h
  ${CMAKE_CURRENT_LIST_DIR}/../include/toml )

set(toml_INCLUDE_DIR ${toml_INCLUDE_DIR}/../)

find_library(toml_LIBRARY NAMES toml PATHS
  ${CMAKE_CURRENT_LIST_DIR}/../lib/)

FOREACH(MYSUBLIB ${LIST_DEPENDENT_LIB})
if(NOT TARGET ${MYSUBLIB})
  find_package(${MYSUBLIB} REQUIRED)
  if(${MYSUBLIB}_FOUND)
  	list(APPEND toml_LIBRARY ${${MYSUBLIB}_LIBRARY})
  	list(APPEND toml_INCLUDE_DIR ${${MYSUBLIB}_INCLUDE_DIR})
  endif()

endif()
ENDFOREACH(MYSUBLIB)


if(toml_INCLUDE_DIR AND toml_LIBRARY)
    set(toml_FOUND TRUE)
endif(toml_INCLUDE_DIR AND toml_LIBRARY)

if(toml_FOUND)
    if(NOT toml_FIND_QUIETLY)
        message(STATUS "found toml: ${toml_LIBRARY}")
    endif(NOT toml_FIND_QUIETLY)
else(toml_FOUND)
    if(toml_FIND_REQUIRED)
        message(FATAL_ERROR "could not find toml library")
    endif(toml_FIND_REQUIRED)
endif(toml_FOUND)

message(STATUS "run Findtoml.cmake success!")
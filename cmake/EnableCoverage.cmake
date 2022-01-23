if(__enable_coverage)
  return()
endif()
set(__enable_coverage YES)

macro(_enable_coverage_flags)
  set(_compile_options)
  set(_link_options)
  set(_link_libs)
  if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(_compile_options -fprofile-arcs -ftest-coverage)
    set(_link_options --coverage)
    set(_link_libs gcov)
  elseif(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang"
         OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
    set(_compile_options -fprofile-arcs -ftest-coverage)
    set(_link_options --coverage)
    set(_link_libs gcov)
  endif()
endmacro()

function(enable_coverage _target)
  _enable_coverage_flags()
  target_compile_options(${_target} PRIVATE ${_compile_options})
  target_link_libraries(${_target} PRIVATE ${_link_libs})
  target_link_options(${_target} PRIVATE ${_link_options})
endfunction()

function(globally_enable_coverage)
  _enable_coverage_flags()
  set(CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} ${_compile_options}"
      PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} ${_compile_options}"
      PARENT_SCOPE)
  link_libraries(${_link_libs})
  add_link_options(${_link_options})
endfunction()

file(GLOB koans_list *)

foreach(koan_dir ${koans_list})
  if(IS_DIRECTORY ${koan_dir})
    message(STATUS "Adding koan: ${koan_dir}")
    add_subdirectory(${koan_dir})
  endif()
endforeach()

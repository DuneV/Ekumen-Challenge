include(GoogleTest)

get_filename_component(target_name ${CMAKE_CURRENT_LIST_DIR} NAME)

add_test(
  NAME ${target_name}
  COMMAND ${PYTHON_EXECUTABLE} koan.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

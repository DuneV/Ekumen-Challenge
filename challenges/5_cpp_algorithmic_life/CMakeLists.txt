include(GoogleTest)

get_filename_component(target_name ${CMAKE_CURRENT_LIST_DIR} NAME)

add_executable(${target_name} koan.cpp)

target_compile_features(${target_name} PUBLIC cxx_std_17)

target_link_libraries(${target_name} gmock_main)

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_test(NAME ${target_name} COMMAND ${target_name})

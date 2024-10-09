
add_library(i2c i2c.cpp)

target_link_libraries(i2c PRIVATE pico_stdlib hardware_gpio hardware_i2c interface_i2c )

target_include_directories(i2c PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

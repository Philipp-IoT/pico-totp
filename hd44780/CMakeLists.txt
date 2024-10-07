
add_library(hd44780 hd44780.cpp)

pico_generate_pio_header(hd44780 ${CMAKE_CURRENT_LIST_DIR}/hd44780.pio)

target_link_libraries(hd44780 PRIVATE pico_stdlib hardware_pio)

target_include_directories(hd44780 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

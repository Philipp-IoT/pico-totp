
add_library(ds1307 ds1307.cpp)

target_link_libraries(ds1307 PRIVATE pico_stdlib hardware_pio interface_i2c interface_rtc)

target_include_directories(ds1307 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

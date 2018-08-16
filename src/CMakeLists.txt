
file(GLOB YART_SRCS "*.cpp")

message(STATUS ${YART_SRCS})

add_executable(yart-cpp.exe ${YART_SRCS})

set_target_properties(yart-cpp.exe PROPERTIES
    COMPILE_FLAGS "-std=c++14 -O3"
)

target_link_libraries(yart-cpp.exe -lpthread -lfreeimage)

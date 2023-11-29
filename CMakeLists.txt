cmake_minimum_required(VERSION 3.00..3.27)

project(learn-c-vulkan-api-01 C)

add_executable(main main.c)
target_link_libraries(main /usr/lib/libvulkan.so /usr/lib/libglfw.so
                      /usr/lib/libm.so)

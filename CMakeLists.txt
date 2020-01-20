set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

project(LKVector)

add_library(LKVector STATIC Vector.c)

project(run-tests)

add_executable(run-tests tests.c)
target_link_libraries(run-tests LKVector)


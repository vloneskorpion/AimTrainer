add_library(glad_lib STATIC
        glad.c
        glad.h)

# TODO check this why it was set
target_include_directories(glad_lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(${PROJECT_NAME} glad_lib)
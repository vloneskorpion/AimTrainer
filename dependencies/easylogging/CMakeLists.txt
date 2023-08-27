add_library(logging_lib STATIC
        easylogging++.cc
        easylogging++.h)

target_link_libraries(${PROJECT_NAME} logging_lib)
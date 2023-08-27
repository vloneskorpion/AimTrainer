add_library(imgui_lib STATIC
        imconfig.h
        imgui.h
        imgui_internal.h
        stb_rect_pack.h
        stb_textedit.h
        stb_truetype.h

        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp

        imgui_impl_glfw_gl3.h
        imgui_impl_glfw_gl3.cpp)

target_include_directories(imgui_lib PRIVATE
        ${CMAKE_SOURCE_DIR}/dependencies
        ${CMAKE_SOURCE_DIR}/dependencies/glad/)

target_link_libraries(imgui_lib glfw ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} imgui_lib)
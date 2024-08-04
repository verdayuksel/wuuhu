cmake_minimum_required(VERSION 3.10)
project(damn)

# C++11 standardını kullan
set(CMAKE_CXX_STANDARD 11)

# Homebrew kütüphanelerinin yollarını ayarla
set(CMAKE_PREFIX_PATH
        /opt/homebrew/opt/sdl2/2.30.5
        /opt/homebrew/opt/glfw/3.3.8
        /opt/homebrew/Cellar/glew/2.2.0_1
)

# GLFW, OpenGL, SDL2 ve GLEW bul
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

# ImGui dizinini ayarla
set(IMGUI_DIR /Users/verdayuksel/CLionProjects/damn/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)

# ImGui kaynak dosyalarını ekle
file(GLOB IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

# Uygulamanın kaynak dosyalarını ekle
set(SOURCES
        CameraInterface.cpp
        ${IMGUI_SOURCES}
)

# İlgili başlık dosyalarını ekle
set(HEADERS
        CameraInterface.h
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_DIR}/imgui_internal.h
        ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.h
        ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.h
)

# Yürütülebilir dosyayı oluştur
add_executable(damn ${SOURCES})

# İmgui, GLFW ve OpenGL kütüphanelerini ekle
target_include_directories(damn PRIVATE
        ${IMGUI_DIR}
        ${IMGUI_BACKENDS_DIR}
        /opt/homebrew/opt/sdl2/2.30.5/include
        /opt/homebrew/opt/glfw/3.3.8/include
        /opt/homebrew/Cellar/glew/2.2.0_1/include
)

# SDL2 kütüphanelerini bağla (eğer kullanıyorsanız)
target_link_libraries(damn PRIVATE
        SDL2::SDL2
        ${OPENGL_LIBRARIES}
        GLEW::GLEW
        glfw
        ${CMAKE_DL_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
)

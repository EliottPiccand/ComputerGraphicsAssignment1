cmake_minimum_required(VERSION 3.20)

project(ComputerGraphicsProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Src/*.cpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_SOURCE_DIR}/Include"
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            "${CMAKE_SOURCE_DIR}/Lib/glfw3.lib"
            "${CMAKE_SOURCE_DIR}/Lib/glew32.lib"
            opengl32
            user32
            gdi32
            shell32
            glu32
    )
elseif(APPLE)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(GLEW REQUIRED)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            OpenGL::GL
            glfw
            GLEW::GLEW
    )
else()
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(GLEW REQUIRED)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            OpenGL::GL
            glfw
            GLEW::GLEW
    )
endif()

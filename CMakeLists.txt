cmake_minimum_required(VERSION 3.0.0)
project(msd_projekt VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)


add_executable(msd_projekt 
   src/Camera.cpp
   src/Cube.cpp
   src/Grid3d.cpp
   src/IndexBuffer.cpp
   src/main.cpp
   src/Shader.cpp
   src/VertexArray.cpp
   src/VertexBuffer.cpp
)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(msd_projekt GLEW::GLEW)
target_link_libraries(msd_projekt glfw OpenGL::GL)



cmake_minimum_required(VERSION 3.1.0)
project(V-Engine2D VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  file(GLOB SDL2_SHARED_BINARIES
    # "deps/SDL2*/lib/x64/*.dll"
  )
  # file(COPY ${SDL2_SHARED_BINARIES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
  # file(COPY "res" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
  find_package(SFML REQUIRED network audio graphics window system)
endif()

add_definitions(
  -DSFML_STATIC
)


file(GLOB_RECURSE SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(V-Engine2D "${SRC}")

target_include_directories(V-Engine2D PRIVATE
  src
  src/apps
  src/core
  src/interfaces
  src/screens
  src/primitives
  src/throwmacros
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_include_directories(${PROJECT_NAME} PRIVATE
    deps/SFML/include
  )
  target_link_directories(${PROJECT_NAME} PRIVATE
    deps/SFML/lib
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE
    FLAC
    freetype
    ogg
    sfml-audio-s
    sfml-graphics-s
    sfml-network-s
    sfml-window-s
    sfml-system-s
    vorbis
    vorbisenc
    vorbisfile
    opengl32
    winmm
    gdi32
    freetype
  )
else() 
  target_include_directories(${PROJECT_NAME} PRIVATE
    ${SFML_INCLUDE_DIR}
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SFML_LIBRARIES}
  )
endif()
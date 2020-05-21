cmake_minimum_required(VERSION 3.1.0)
project(V-Engine2D VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Copy DLLs from SDL2 libraries into build output
  file(GLOB SDL2_SHARED_BINARIES
    "deps/SDL2*/lib/x64/*.dll"
  )
  file(COPY ${SDL2_SHARED_BINARIES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
  file(COPY "res" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_mixer REQUIRED)
  find_package(SDL2_net REQUIRED)
  find_package(SDL2_ttf REQUIRED)
endif()

file(GLOB_RECURSE SRC
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

add_executable(V-Engine2D "${SRC}")

target_include_directories(V-Engine2D PRIVATE
  src
  src/apps
  src/core
  src/interfaces
  src/screens
  src/throwmacros
  deps/SDL_FontCache
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")

  target_include_directories(${PROJECT_NAME} PRIVATE
    deps/SDL2/include
    deps/SDL2_image/include
    deps/SDL2_mixer/include
    deps/SDL2_net/include
    deps/SDL2_ttf/include
  )
  target_link_directories(${PROJECT_NAME} PRIVATE
    deps/SDL2/lib/x64
    deps/SDL2_image/lib/x64
    deps/SDL2_mixer/lib/x64
    deps/SDL2_net/lib/x64
    deps/SDL2_ttf/lib/x64
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2
    SDL2main
    SDL2_image
    SDL2_mixer
    SDL2_net
    SDL2_ttf
  )
  else()
  target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${SDL2_NET_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_NET_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
  )
endif()


if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_include_directories(${PROJECT_NAME} PRIVATE
    deps/SDL2/include
    deps/SDL2_net/include
  )
  target_link_directories(${PROJECT_NAME} PRIVATE
    deps/SDL2/lib/x64
    deps/SDL2_net/lib/x64
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2
    SDL2_net
  )
else()  
  target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${SDL2_NET_INCLUDE_DIR}
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2_NET_LIBRARIES}
  )
endif()
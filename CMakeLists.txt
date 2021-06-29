cmake_minimum_required(VERSION 3.1.0)
project(Saffron VERSION 0.1.0)

### FLAGS ###
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(THREADS_PREFER_PTHREAD_FLAG ON)

### DIRECTORIES ###
set(BuildDir "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(ResourceDir "res")
set(LibraryDir "lib")
set(IncludeDir "src")

set(LibSuffix "")

### EXPORTED DIRECTORIES ###
set(SaffronResourceDir ${PROJECT_SOURCE_DIR}/${ResourceDir} PARENT_SCOPE)

### VARIABLES ###
set(PLATFORM_MACRO SE_PLATFORM)
if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(BuildType Debug)
elseif (${CMAKE_BUILD_TYPE} MATCHES Release)
    set(BuildType Release)
elseif (${CMAKE_BUILD_TYPE} MATCHES MinSizeRel)
    set(BuildType Dist)
else ()
    message(FATAL_ERROR "Bad configuration selected")
endif ()

### COMPILER SPECIFIC ###
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set(CompilerLibDir "msvc")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/w44244)
    add_compile_options(/w44267)

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CompilerLibDir "mingw")
else ()
    message(FATAL_ERROR "Bad compiler selected. Supported is GNU and MSVC")
endif ()

### BUILD TYPE SPECIFIC ###
if (BuildType STREQUAL Debug)
    set(OutputDir ${BuildDir})
    add_definitions(
            -DSE_DEBUG
    )
    set(LibSubFolder Debug)
    set(LibSuffix "-d")

elseif (BuildType STREQUAL Release)
    set(OutputDir ${BuildDir})
    add_definitions(
            -DSE_RELEASE
    )
    set(LibSubFolder Release)
elseif (BuildType STREQUAL Dist)
    set(OutputDir ${BuildDir})
    set(LibSubFolder Release)
    add_definitions(
            -DSE_DIST
    )
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(Platform windows)
    add_definitions(-D${PLATFORM_MACRO}_WINDOWS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(Platform Linux)
    add_definitions(-D${PLATFORM_MACRO}_LINUX)
else ()
    message(FATAL_ERROR "Unknown OS. Only Windows and Linux are supported.")
endif ()

### OS SPECIFIC ###
if (Platform STREQUAL windows)
    file(GLOB SharedBinaries
            "${LibraryDir}/Box2D/bin/${CompilerLibDir}/${LibSubFolder}/*.dll"
            "${LibraryDir}/SFML/bin/${CompilerLibDir}/${LibSubFolder}/*.dll"
            )

    ### EXTERNAL INCLUDES AND LIBRARIES ###
    file(GLOB IncludesExt
            ${LibraryDir}/Box2D/include
            ${LibraryDir}/Glad/include
            ${LibraryDir}/entt/include
            ${LibraryDir}/imgui/include
            ${LibraryDir}/jcv/include
            ${LibraryDir}/SFML/include
            ${LibraryDir}/spdlog/include
            PARENT_SCOPE
            )


    ### EXPORT ###
    set(SaffronIncludeDirsExt ${IncludesExt} PARENT_SCOPE)
    set(SaffronIncludeDirsExt ${IncludesExt})
    set(SaffronSharedBinaries ${SharedBinaries} PARENT_SCOPE)

elseif (Platform STREQUAL windows)
    find_package(SFML REQUIRED network audio graphics window system)
    find_package(Box2D REQUIRED)
endif ()

find_package(Threads REQUIRED)


### DEFINITIONS ###
add_definitions()

### EXECUTABLE ###
file(GLOB_RECURSE allSources "${CMAKE_SOURCE_DIR}/*.cpp")
add_library(Saffron "${allSources}")
target_precompile_headers(Saffron PUBLIC "src/SaffronPCH.cpp")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

### LOCAL INCLUDES ###
file(GLOB includes ${IncludeDir} PARENT_SCOPE)
set(SaffronIncludeDirs ${includes})
set(SaffronIncludeDirs ${includes} PARENT_SCOPE)
target_include_directories(Saffron PRIVATE ${SaffronIncludeDirs})

if (Platform STREQUAL windows)
    target_include_directories(Saffron PUBLIC ${SaffronIncludeDirsExt})
    target_link_directories(Saffron PUBLIC
            ${LibraryDir}/Box2D/lib
            ${LibraryDir}/imgui/lib/${CompilerLibDir}
            ${LibraryDir}/SFML/lib/${CompilerLibDir}
            ${LibraryDir}/Glad/lib
            )
    target_link_libraries(Saffron PUBLIC
            ## Box2D
            Box2D

            ## SFML
            sfml-graphics${LibSuffix}
            sfml-window${LibSuffix}
            sfml-audio${LibSuffix}
            sfml-network${LibSuffix}
            sfml-system${LibSuffix}
            opengl32
            freetype
            winmm
            gdi32
            openal32
            flac
            vorbisenc
            vorbisfile
            vorbis
            ogg
            ws2_32

            ## imgui
            imgui${LibSuffix}
            
			## Glad
			Glad

            )
else ()
    target_include_directories(Saffron PRIVATE
            ${BOX2D_INCLUDEDIR}
            ${SFML_INCLUDEDIR}
            ${LibraryDir}/entt/include
            ${LibraryDir}/imgui/include
            ${LibraryDir}/jcv/include
            ${LibraryDir}/spdlog/include
            )
    target_link_directories(Saffron PRIVATE
            )
    target_link_libraries(Saffron PRIVATE
            ${BOX2D_LIBRARY}
            ${SFML_LIBRARIES}
            Threads::Threads
            imgui
            )
endif ()

### OUTPUT DIRECTORIES ###
set_target_properties(Saffron PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${OutputDir}
        LIBRARY_OUTPUT_DIRECTORY ${OutputDir}
        RUNTIME_OUTPUT_DIRECTORY ${OutputDir}
        )
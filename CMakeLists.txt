cmake_minimum_required(VERSION 3.10.0)
project(akg_proj VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Определение платформы (добавлено)
if(APPLE)
    set(MACOS TRUE)
elseif(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Подключаем модуль OpenGLSomethingFrameDisplayerEVO
add_subdirectory(OpenGLSomethingFrameDisplayerEVO)

# Проверяем что модуль создан
if(NOT TARGET OpenGLSomethingFrameDisplayerEVO)
    message(FATAL_ERROR "Module OpenGLSomethingFrameDisplayerEVO not found")
endif()

find_package(PkgConfig REQUIRED)

# Найти библиотеки через pkg-config
pkg_check_modules(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)

# Убраны зависимости OpenGL/glfw/GLEW - теперь они в модуле

file(GLOB_RECURSE SOURCE_FILES
    "my_glm/*.hpp"
    "core/*.cpp"
    "renderer/*.cpp"
    "world/*.cpp"
    "imgui/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES}  main.cpp Global.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Линкуем только с модулем и SDL2 - OpenGL зависимости теперь в модуле
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGLSomethingFrameDisplayerEVO  # Модуль содержит glfw, GLEW, OpenGL
    ${SDL2_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

# macOS специфичные зависимости теперь в модуле

# Копируем ресурсы модуля (если функция экспортирована)
if(COMMAND copy_opengl_resources)
    copy_opengl_resources(${PROJECT_NAME})
else()
    message(WARNING "Function copy_opengl_resources not found. Resources won't be copied.")
endif()

# Флаги компиляции для разных платформ
if(LINUX)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -ffast-math")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -ffast-math -march=native -mtune=native")
elseif(MACOS)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -ffast-math")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -ffast-math")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:RelWithDebInfo>:-O3 -g -ffast-math>
    $<$<CONFIG:Release>:-O3 -DNDEBUG -ffast-math>
)


if(LINUX)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-march=native -mtune=native>
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/Porsche_911_GT2.obj
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/model.obj
    COMMENT "Copying test model to executable directory"
)


if(EXISTS ${CMAKE_SOURCE_DIR}/shaders)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
        COMMENT "Copying shaders to executable directory"
    )
endif()
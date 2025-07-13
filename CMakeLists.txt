cmake_minimum_required(VERSION 3.20)
project(xbox_controller_api VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg integration
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

find_package(SDL3 REQUIRED)

include_directories(include)

set(SOURCES
    src/main.cpp
    src/gamepad_controller.cpp
    src/input_simulator.cpp
    src/media_controller.cpp
    src/config_manager.cpp
)

set(HEADERS
    include/gamepad_controller.h
    include/input_simulator.h
    include/media_controller.h
    include/config_manager.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} user32)
elseif(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    find_library(XTST_LIBRARY Xtst REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} ${XTST_LIBRARY})
elseif(APPLE)
    find_library(CARBON_LIBRARY Carbon)
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    target_link_libraries(${PROJECT_NAME} ${CARBON_LIBRARY} ${COREGRAPHICS_LIBRARY})
endif()

# Install configuration (only for Linux and macOS)
if(UNIX)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        COMPONENT Runtime)

    # CPack configuration for packaging
    set(CPACK_PACKAGE_NAME "xbox-controller-api")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Xbox Controller Desktop Bridge")
    set(CPACK_PACKAGE_VENDOR "Xbox Controller API")
    set(CPACK_GENERATOR "TGZ")

    if(APPLE)
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
            set(CPACK_PACKAGE_FILE_NAME "xbox-controller-api-macos-arm64")
        else()
            set(CPACK_PACKAGE_FILE_NAME "xbox-controller-api-macos-x64")
        endif()
    else()
        # Linux
        set(CPACK_PACKAGE_FILE_NAME "xbox-controller-api-linux-x64")
    endif()

    include(CPack)
endif()
cmake_minimum_required(VERSION 3.20)
project(Pbl2Book LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Widgets Charts REQUIRED)

# Qt6 helper (>= 6.3)
qt_standard_project_setup()


# Source files
file(GLOB_RECURSE SOURCES "scoure/*.cpp" "core/*.cpp" "ui/*.cpp")
file(GLOB_RECURSE HEADERS "header/*.h" "core/*.h" "ui/*.h")

qt_add_resources(ICON_RESOURCES
    ui/resources/icons/icons.qrc
)

add_executable(pbl2_book ${SOURCES} ${HEADERS} ${ICON_RESOURCES}
        header/service/BaseService.h
        scoure/service/BaseService.cpp)

# Ensure the target uses C++20 (in addition to the global setting)
if (TARGET pbl2_book)
    target_compile_features(pbl2_book PRIVATE cxx_std_20)
endif()

target_link_libraries(pbl2_book PRIVATE Qt6::Widgets Qt6::Charts)

target_include_directories(pbl2_book PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/header
    ${CMAKE_CURRENT_SOURCE_DIR}/header/service
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Test login executable removed - use debug_login.cpp instead

install(TARGETS pbl2_book)

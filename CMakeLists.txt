cmake_minimum_required(VERSION 3.14..3.30)
project(gdi VERSION 0.1 LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use libc++ on macOS for better C++23 support
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()

#################################################################################
# Dependencies
#################################################################################
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

find_package(OpenSSL REQUIRED)

# Fetch glaze from GitHub
FetchContent_Declare(glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG v7.7.0
)
FetchContent_MakeAvailable(glaze)

#################################################################################
# Executables
#################################################################################

add_executable(main "src/main.cpp")
target_link_libraries(main PUBLIC )
target_include_directories(main PUBLIC ${glaze_SOURCE_DIR}/include)



cmake_minimum_required (VERSION 3.8)

project ("WebCrawlerCpp")

set(PROJECT_MAIN_NAME WebCrawlerCpp)
add_executable (${PROJECT_MAIN_NAME} "WebCrawlerCpp.cpp")

# set cmake options -Wno-dev
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_C_STANDARD 17)
set (CMAKE_EXE_LINKER_FLAGS -static)

    ######VCPKG
set (CMAKE_TOOLCHAIN_FILE "c:/src/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake") #-DCMAKE_TOOLCHAIN_FILE=C:/src/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake
#no use: #set(VCPKG_INCLUDE_DIRS "C:/src/vcpkg/vcpkg/installed/x86-windows/include")
#no use: #include_directories(${VCPKG_INCLUDE_DIRS})
    # use the 64 bit version, (check why 86 not works)
find_package(CURL CONFIG REQUIRED)
target_link_libraries(${PROJECT_MAIN_NAME} PRIVATE CURL::libcurl)

find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT_MAIN_NAME} PRIVATE nlohmann_json::nlohmann_json)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)


    #########Boost
#set Boost_ROOT="C:/local/boost_1_80_0"    #env
#set Boost_INCLUDE_DIR="C:/local/boost_1_80_0" #env (Boost_INCLUDEDIR, not for this version)
#set Boost_LIBRARYDIR="C:/local/boost_1_80_0/lib64-msvc-14.1"  #env (Boost_LIBRARY_DIR, BOOST_LIBRARY_DIR , not for this version)
#set (Boost_DEBUG 1)    #too long
set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set (Boost_USE_MULTITHREAD ON)  # enable multithreading

find_package(Boost) #Boost 1.80.0     #Boost_ROOT env ! ?
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})   #env Boost_INCLUDE_DIR!
else()
    message(FATAL_ERROR "~~~ ! Boost Fail ! ~~~")
endif()


message(STATUS "~~~ CMake Starts, Compile starts ~~~")

# sub projects
# https://www.jetbrains.com/help/clion/2018.2/quick-cmake-tutorial.html#d156825e282

#example of include libs
#include_directories( ${MY_SOURCE_DIR}/src )
#include_directories( BEFORE ${MY_SOURCE_DIR}/src )
#include_directories( AFTER ${MY_SOURCE_DIR}/src )
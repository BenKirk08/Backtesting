cmake_minimum_required(VERSION 3.10)
project(BackTester)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#all your project files
add_executable(BackTester
    main.cpp
    DataLoader.cpp
    Strategy.cpp
    Portfolio.cpp
    Menu.cpp
    Cointegration.cpp
    Kalman.cpp
)

target_include_directories(BackTester PRIVATE /opt/homebrew/include/eigen3)

#C&P -> dont need to know how works
#copy and paste to terminal when new things added
#cmake -B build && cmake --build build
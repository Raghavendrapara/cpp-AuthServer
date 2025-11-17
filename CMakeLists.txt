cmake_minimum_required(VERSION 3.10)
project(AuthServer) 

set(CMAKE_CXX_STANDARD 17)

# 1. Find Libraries
find_package(cryptopp CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

# 2. Create Server Executable (THE BAKE STEP)
add_executable(AuthServer 
    src/main.cpp 
    src/User.cpp
)

# 3. Include Directories
target_include_directories(AuthServer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 4. Link Libraries (THE DECORATE STEP)
target_link_libraries(AuthServer PRIVATE  # <--- Name 3 (Must match Name 2)
    Crow::Crow 
    cryptopp::cryptopp
    ws2_32
    mswsock 
)
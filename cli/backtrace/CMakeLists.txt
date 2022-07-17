include(FetchContent)
FetchContent_Declare(
  capstone
  GIT_REPOSITORY https://github.com/capstone-engine/capstone
  GIT_TAG        1d230532840a37ac032c6ab80128238fc930c6c1 # 4.0.2
)

option(CAPSTONE_ARCHITECTURE_DEFAULT "" OFF)
option(CAPSTONE_ARM_SUPPORT "" ON)
option(CAPSTONE_BUILD_TESTS "" OFF)
option(CAPSTONE_BUILD_STATIC "" ON)
option(CAPSTONE_BUILD_SHARED "" OFF)
option(CAPSTONE_BUILD_CSTOOL "" OFF)


FetchContent_MakeAvailable(capstone)

add_executable(backtrace dwarf/header.cpp dwarf/state_machine.cpp elf_analysis.cpp main.cpp)
target_include_directories(backtrace PRIVATE ${capstone_SOURCE_DIR}/include)
target_link_libraries(backtrace PRIVATE vcp::vcp z capstone-static)

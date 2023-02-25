# Set cmake version requirement
cmake_minimum_required(VERSION 3.14)

project(CANTool)

# Compiler options
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-pthread")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")
include_directories("$ENV{LIBRARIES_PATH}/gtest/include")
link_directories("$ENV{LIBRARIES_PATH}/gtest/lib")

# -------------------------------------------------
# CAN Interfaces
# -------------------------------------------------
set(SOURCES_INTERFACES
	# Generic CAN Interface
	source/interfaces/include/ICANInterface.h

	# CAN Socket
	source/interfaces/include/CANSocket.h
	source/interfaces/src/CANSocket.cpp
)

# -------------------------------------------------
# General CAN utilities, including protocols
# -------------------------------------------------
set(SOURCES_CAN_UTILITY
	# CAN Message
	source/can/include/Message.h
	source/can/src/Message.cpp

	# CANOpen protocol
	source/can/include/canopen.h
	source/can/src/canopen.cpp
)

# -------------------------------------------------
# Sources for targets
# -------------------------------------------------
set(SOURCES_TARGET_CANLIB
	${SOURCES_INTERFACES}
	${SOURCES_CAN_UTILITY}
)

set(SOURCES_TARGET_CANTOOL
	# Main entry point
	source/main.cpp
)

# -------------------------------------------------
# Tests
# -------------------------------------------------
set(SOURCES_TARGET_TESTS
	tests/test_main.cpp
	tests/canopen/canopen_tests.cpp
)

# -------------------------------------------------
# Build targets
# -------------------------------------------------
add_library(canlib STATIC ${SOURCES_TARGET_CANLIB})
add_executable(cantool ${SOURCES_TARGET_CANTOOL})
target_link_libraries(cantool canlib)

# The tests
add_executable(tests ${SOURCES_TARGET_TESTS})
target_link_libraries(tests gtest canlib)

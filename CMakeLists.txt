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

	# Interface factory
	source/interfaces/include/connection_factory.h
	source/interfaces/src/connection_factory.cpp

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
)

# -------------------------------------------------
# Other utilities
# -------------------------------------------------
set(SOURCES_UTILITY
	# Commandline arguments parser
	source/utility/include/cmdargs_parser.h
	source/utility/src/cmdargs_parser.cpp
)

# -------------------------------------------------
# Sources for targets
# -------------------------------------------------
set(SOURCES_TARGET_CANLIB
	${SOURCES_INTERFACES}
	${SOURCES_CAN_UTILITY}

	# Include the utilities here, although they are not directly CAN-related
	${SOURCES_UTILITY}
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
	tests/connection_factory_tests.cpp
	tests/cmdargs_parser_tests.cpp
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

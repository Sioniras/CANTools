# Set cmake version requirement
cmake_minimum_required(VERSION 3.14)

project(CANTool)

# Compiler options
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-pthread")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")

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
# General CAN utilities
# -------------------------------------------------
set(SOURCES_CAN_UTILITY
	# CAN Message
	source/can/include/Message.h
	source/can/src/Message.cpp
)

# -------------------------------------------------
# Sources for target
# -------------------------------------------------
set(SOURCES_TARGET_CANTOOL
	# Main entry point
	source/main.cpp

	${SOURCES_INTERFACES}
	${SOURCES_CAN_UTILITY}
)

# -------------------------------------------------
# Build targets
# -------------------------------------------------
add_executable(cantool ${SOURCES_TARGET_CANTOOL})


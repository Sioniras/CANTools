///////////////////////////////////////////////////////////////////////
// Tests for the connection factory
//
// The connection factory should produce a CAN Interface connection
// based on string input (to be used from commandline).
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <interfaces/include/connection_factory.h>
#include <interfaces/include/CANSocket.h>

TEST(connection_factory, string_bad_interface_specification_returns_nullptr)
{
	auto interface = can::interfaces::connection_factory::create("wrong");
	EXPECT_TRUE(interface == nullptr);
}

TEST(connection_factory, string_create_can_socket)
{
	auto interface = can::interfaces::connection_factory::create("can");
	EXPECT_TRUE(interface != nullptr);
	EXPECT_TRUE(dynamic_cast<can::interfaces::CANSocket*>(interface.get()) != nullptr);
}

TEST(connection_factory, enum_bad_interface_specification_returns_nullptr)
{
	// "200" should not be part of the interface_type enum
	auto interface = can::interfaces::connection_factory::create(static_cast<can::interfaces::interface_type>(200));
	EXPECT_TRUE(interface == nullptr);
}

TEST(connection_factory, enum_create_can_socket)
{
	auto interface = can::interfaces::connection_factory::create(can::interfaces::interface_type::socket_can);
	EXPECT_TRUE(interface != nullptr);
	EXPECT_TRUE(dynamic_cast<can::interfaces::CANSocket*>(interface.get()) != nullptr);
}

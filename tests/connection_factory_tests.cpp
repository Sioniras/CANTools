///////////////////////////////////////////////////////////////////////
// Tests for the connection factory
//
// The connection factory should produce a CAN Interface connection
// based on string input (to be used from commandline).
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <memory>

#include <interfaces/include/CANSocket.h>

enum class interface_type
{
	socket_can,	// Using the SocketCAN interface
};

// Class is stored here during initial (TDD) development and then moved elsewhere once needed
class connection_factory
{
	public:
		connection_factory() = delete;

		static std::unique_ptr<can::interfaces::ICANInterface> create(const std::string& type)
		{
			if(type.compare("can") == 0)
				return std::make_unique<can::interfaces::CANSocket>();
			return nullptr;
		}

		static std::unique_ptr<can::interfaces::ICANInterface> create(interface_type type)
		{
			if(type == interface_type::socket_can)
				return std::make_unique<can::interfaces::CANSocket>();
			return nullptr;
		}
};

TEST(connection_factory, string_bad_interface_specification_returns_nullptr)
{
	auto interface = connection_factory::create("wrong");
	EXPECT_TRUE(interface == nullptr);
}

TEST(connection_factory, string_create_can_socket)
{
	auto interface = connection_factory::create("can");
	EXPECT_TRUE(interface != nullptr);
	EXPECT_TRUE(dynamic_cast<can::interfaces::CANSocket*>(interface.get()) != nullptr);
}

TEST(connection_factory, enum_bad_interface_specification_returns_nullptr)
{
	auto interface = connection_factory::create(static_cast<interface_type>(200));	// "200" should not be part of the interface_type enum
	EXPECT_TRUE(interface == nullptr);
}

TEST(connection_factory, enum_create_can_socket)
{
	auto interface = connection_factory::create(interface_type::socket_can);
	EXPECT_TRUE(interface != nullptr);
	EXPECT_TRUE(dynamic_cast<can::interfaces::CANSocket*>(interface.get()) != nullptr);
}

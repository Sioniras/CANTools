///////////////////////////////////////////////////////////////////////
// Connection factory implementation
//
// The connection factory should produce a CAN Interface connection.
///////////////////////////////////////////////////////////////////////
#include <interfaces/include/connection_factory.h>
#include <interfaces/include/CANSocket.h>

namespace can::interfaces
{
	std::unique_ptr<ICANInterface> connection_factory::create(const std::string& type)
	{
		if(type.compare("can") == 0)
			return std::make_unique<CANSocket>();
		return nullptr;
	}

	std::unique_ptr<ICANInterface> connection_factory::create(interface_type type)
	{
		if(type == interface_type::socket_can)
			return std::make_unique<CANSocket>();
		return nullptr;
	}
}

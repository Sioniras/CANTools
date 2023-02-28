///////////////////////////////////////////////////////////////////////
// Connection factory
//
// The connection factory should produce a CAN Interface connection.
///////////////////////////////////////////////////////////////////////
#pragma once

#include <memory>

#include <interfaces/include/ICANInterface.h>

namespace can::interfaces
{
	enum class interface_type
	{
		socket_can,	// Using the SocketCAN interface
	};

	class connection_factory
	{
		public:
			// Static-only class
			connection_factory() = delete;

			// Public static interface
			static std::unique_ptr<ICANInterface> create(const std::string& type);
			static std::unique_ptr<ICANInterface> create(interface_type type);
	};
}

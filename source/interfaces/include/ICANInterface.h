///////////////////////////////////////////////////////////////////////
// Generic CAN Interface
///////////////////////////////////////////////////////////////////////
#pragma once

#include <can/include/Message.h>

namespace can::interfaces
{
	class ICANInterface
	{
		public:
			// Messages
			virtual bool SendMessage(const can::Message& message) = 0;
			virtual bool RequestMessage(can::Message& message) = 0;

			// Connection
			virtual bool Connect(const std::string& interfaceName) = 0;
			virtual void Disconnect() = 0;
			virtual bool IsReady() const = 0;

			// Other methods
			virtual void SetTimeout(int timeout) = 0;
			virtual void SetBlockingMode(bool blocking) = 0;

			// Destructor
			virtual ~ICANInterface() {}
	};
}

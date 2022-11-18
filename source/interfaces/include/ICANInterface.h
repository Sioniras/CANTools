///////////////////////////////////////////////////////////////////////
// Generic CAN Interface
///////////////////////////////////////////////////////////////////////
#pragma once

#include <can/include/Message.h>

namespace CAN::Interfaces
{
	class ICANInterface
	{
		public:
			virtual void Disconnect() = 0;
			virtual bool SendMessage(const CAN::Message& message) = 0;
			virtual bool RequestMessage(CAN::Message& message) = 0;

			virtual ~ICANInterface() {}
	};
}

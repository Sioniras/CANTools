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
			virtual bool SendMessage(const can::Message& message) = 0;
			virtual bool RequestMessage(can::Message& message) = 0;

			virtual ~ICANInterface() {}
	};
}

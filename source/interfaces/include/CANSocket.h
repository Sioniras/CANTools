///////////////////////////////////////////////////////////////////////
// CAN Socket Interface
//
// Uses a socket connection to communicate with a directly attached
// CAN bus.
//
// Note: see https://www.kernel.org/doc/html/v5.11/networking/can.html
///////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include <interfaces/include/ICANInterface.h>

namespace can::interfaces
{
	class CANSocket : public ICANInterface
	{
		private:
			int _socket;
			std::string _interfaceName;
			int _interfaceIndex;
			int _pollTimeout;
			bool _blocking;

			bool PollSocket(int timeout);	// Timeout is in milliseconds

		public:
			// Constructor / destructor
			CANSocket();
			~CANSocket();

			// Public methods
			bool Connect(const std::string& interfaceName);
			void Disconnect();
			void SetTimeout(int timeout);
			void SetBlockingMode(bool blocking);
			constexpr bool IsReady() const;
			constexpr bool InterfaceIsAny() const;

			// ICANInterface interface
			bool SendMessage(const can::Message &message) override;
			bool RequestMessage(can::Message &message) override;
	};
}

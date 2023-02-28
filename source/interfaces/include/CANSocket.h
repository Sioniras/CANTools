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
			constexpr bool InterfaceIsAny() const;

			// ICANInterface interface
			bool SendMessage(const can::Message &message) override;
			bool RequestMessage(can::Message &message) override;
			bool Connect(const std::string& interfaceName) override;
			void Disconnect() override;
			void SetTimeout(int timeout) override;
			void SetBlockingMode(bool blocking) override;
			bool IsReady() const override;
	};
}

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

namespace CAN::Interfaces
{
	class CANSocket : public ICANInterface
	{
		private:
			int _socket;
			std::string _interfaceName;
			int _interfaceIndex;
			bool _blocking;

			bool PollSocket(int timeout = 200);	// Timeout is in milliseconds

		public:
			// Constructor / destructor
			CANSocket();
			~CANSocket();

			// Public methods
			bool Connect(const std::string& interfaceName);
			void SetBlockingMode(bool blocking);
			constexpr bool IsReady() const;
			constexpr bool InterfaceIsAny() const;

			// ICANInterface interface
			void Disconnect() override;
			bool SendMessage(const CAN::Message &message) override;
			bool RequestMessage(CAN::Message &message) override;
	};
}

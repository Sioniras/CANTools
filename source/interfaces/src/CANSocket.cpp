///////////////////////////////////////////////////////////////////////
// CAN Socket Interface
//
// Uses a socket connection to communicate with a directly attached
// CAN bus.
///////////////////////////////////////////////////////////////////////
#include <interfaces/include/CANSocket.h>

#include <cstring>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/sockios.h>
#include <poll.h>
#include <fcntl.h>
//#include <linux/can/raw.h>

// --------------------------------------------------------------------
// Constructors / destructor
// --------------------------------------------------------------------
// Constructor
can::interfaces::CANSocket::CANSocket() :
	_socket(0),
	_interfaceName("any"),
	_interfaceIndex(0),
	_pollTimeout(200),
	_blocking(true)
{
}

// Destructor
can::interfaces::CANSocket::~CANSocket()
{
	Disconnect();
}

// --------------------------------------------------------------------
// Private methods
// --------------------------------------------------------------------
// Uses the poll function to check whether any data is available on the socket
bool can::interfaces::CANSocket::PollSocket(int timeout)
{
	// Setup the polling data structure
	pollfd p;
	p.fd = _socket;
	p.events = POLLIN;

	// Poll
	auto result = poll(&p, 1, timeout);

	// Check whether any data is ready for reading
	if(result > 0 && (p.revents & POLLIN))
		return true;

	return false;
}

// --------------------------------------------------------------------
// Public methods
// --------------------------------------------------------------------
// Connect method, based on: https://www.kernel.org/doc/html/v5.11/networking/can.html
bool can::interfaces::CANSocket::Connect(const std::string& interfaceName)
{
	// Check whether a connection is already active
	if(_socket > 0)
		return false;

	// Make sure that the interfaceName does not cause a buffer overflow
	if(interfaceName.size() >= IFNAMSIZ)
		return false;

	// Create the socket
	_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	// Validate the socket
	if(_socket == -1)
		return false;

	// Prepare address structure
	sockaddr_can address;
	address.can_family = AF_CAN;

	// Get interface index
	_interfaceName = interfaceName;
	if(interfaceName.compare("any") == 0)
	{
		// Use index 0 in order to bind to all CAN interfaces
		address.can_ifindex = 0;
		_interfaceIndex = 0;
	}
	else
	{
		// Translate interface name to an interface index
		ifreq ifr;
		std::strcpy(ifr.ifr_name, interfaceName.c_str());
		if(ioctl(_socket, SIOCGIFINDEX, &ifr) < 0)
			return false;

		// Set the index
		address.can_ifindex = ifr.ifr_ifindex;
		_interfaceIndex = ifr.ifr_ifindex;
	}

	// Bind the socket
	bind(_socket, reinterpret_cast<sockaddr*>(&address), sizeof(address));

	return false;
}

// Disconnect method
void can::interfaces::CANSocket::Disconnect()
{
	// Check whether the socket is open
	if(_socket > 0)
	{
		// "close" returns 0 on success, and -1 on failure
		if(close(_socket) == 0)
			_socket = 0;	// Reset _socket to 0 if successfully closed
	}
}

// Sets the timeout used for reading from the socket
void can::interfaces::CANSocket::SetTimeout(int timeout)
{
	_pollTimeout = timeout;
}

// Sets the socket into blocking or non-blocking mode
void can::interfaces::CANSocket::SetBlockingMode(bool blocking)
{
	int flags = fcntl(_socket, F_GETFL, 0);
	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	fcntl(_socket, F_SETFL, flags);
	_blocking = blocking;
}

// Checks whether the CAN socket is open and valid
constexpr bool can::interfaces::CANSocket::IsReady() const
{
	return (_socket > 0);
}

// Checks whether the interface index is set to "any"
constexpr bool can::interfaces::CANSocket::InterfaceIsAny() const
{
	return (_interfaceIndex == 0);
}

// --------------------------------------------------------------------
// ICANInterface interface
// --------------------------------------------------------------------
// Attempt to send a message
bool can::interfaces::CANSocket::SendMessage(const can::Message& message)
{
	// Ensure that the socket is connected
	if(!IsReady())
		return false;

	// TODO: Handle the "any" case - send to all?
	if(InterfaceIsAny())
		return false;

	// Get the internal frame structure
	const can_frame& frame = message.get_frame();

	// Write frame to the CAN socket
	auto count = write(_socket, &frame, sizeof(can_frame));

	// Check whether the write was successful
	return (count == sizeof(can_frame));
}

// Requests a message from the CAN bus
bool can::interfaces::CANSocket::RequestMessage(can::Message& message)
{
	// Ensure that the socket is connected
	if(!IsReady())
		return false;

	// If the socket is not in blocking mode, poll the socket to see if data is available
	if(!_blocking && !PollSocket(_pollTimeout))
		return false;

	// Get message reference
	can_frame& frame = message.get_frame();
	ssize_t count = 0;

	// Check whether the socket is setup for "any" or a specific interface
	if(_interfaceIndex == 0)
	{
		// Receive from any CAN interface
		sockaddr_can address;
		socklen_t len = sizeof(address);
		count = recvfrom(_socket, &frame, sizeof(can_frame), 0, reinterpret_cast<sockaddr*>(&address), &len);

		// Get the name of the interface that received the message
		if(count > 0)
		{
			ifreq ifr;
			ifr.ifr_ifindex = address.can_ifindex;
			ioctl(_socket, SIOCGIFNAME, &ifr);
			message.set_interface(ifr.ifr_name);
		}
		else
		{
			message.set_interface("unknown");
		}
	}
	else
	{
		// Just read message from the specified interface
		count = read(_socket, &frame, sizeof(can_frame));
		message.set_interface(_interfaceName);
	}

	// Get the timestamp
	if(count > 0)
		ioctl(_socket, SIOCGSTAMP, &message.get_timestamp());

	return (count == CAN_MTU);
}

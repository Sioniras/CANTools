///////////////////////////////////////////////////////////////////////
// CAN Message
//
// Represents a CAN message, independent of the underlying structure.
///////////////////////////////////////////////////////////////////////
#include <can/include/Message.h>

// --------------------------------------------------------------------
// Constructor
// --------------------------------------------------------------------
CAN::Message::Message(can_frame& message) :
	_message(message),
	_interface("")
{
}

// --------------------------------------------------------------------
// Operator overloads
// --------------------------------------------------------------------
// Indexing operator
uint8_t& CAN::Message::operator[](int index)
{
	// TODO: Handle out-of-bounds properly
	if(index < 0)
		return _message.data[0];
	else if(index > 7)
		return _message.data[7];

	return _message.data[index];
}

// --------------------------------------------------------------------
// Public properties
// --------------------------------------------------------------------
// Returns the full ID
uint32_t CAN::Message::id() const
{
	return _message.can_id;
}

// Returns ID as a CANOpen ID
uint8_t CAN::Message::id_short() const
{
	return static_cast<uint8_t>(_message.can_id & 0x7F);
}

// Returns the number of data bytes
uint8_t CAN::Message::size() const
{
	return _message.len;
}

// Sets the CAN ID
void CAN::Message::set_id(uint32_t id)
{
	_message.can_id = id;
}

// Sets the CAN ID
void CAN::Message::set_id_short(uint8_t id)
{
	_message.can_id = static_cast<uint32_t>(id) & 0x7F;
}

// Sets the number of data bytes
void CAN::Message::set_size(uint8_t size)
{
	_message.len = size;
}

// --------------------------------------------------------------------
// Access to internal
// --------------------------------------------------------------------
// Provides access to internal CAN frame structure
can_frame& CAN::Message::get_frame()
{
	return _message;
}

// Const-version
const can_frame& CAN::Message::get_frame() const
{
	return _message;
}

std::string CAN::Message::get_interface() const
{
	return _interface;
}

void CAN::Message::set_interface(const std::string& interface)
{
	_interface = interface;
}

timeval& CAN::Message::get_timestamp()
{
	return _timestamp;
}

const timeval& CAN::Message::get_timestamp() const
{
	return _timestamp;
}

// --------------------------------------------------------------------

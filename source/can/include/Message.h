///////////////////////////////////////////////////////////////////////
// CAN Message
//
// Represents a CAN message, independent of the underlying structure.
///////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <linux/can.h>	// can_frame definition
#include <cstdint>	// uintX_t definitions

namespace CAN
{
	class Message
	{
		private:
			can_frame _message;
			std::string _interface;
			timeval _timestamp;

		public:
			// Constructor
			Message(can_frame& message);

			// Operators
			uint8_t& operator[](int index);

			// Getters
			uint32_t id() const;
			uint8_t id_short() const;
			uint8_t size() const;

			// Setters
			void set_id(uint32_t id);
			void set_id_short(uint8_t id);
			void set_size(uint8_t size);

			// Access to internal data, as required by interfaces
			can_frame& get_frame();
			const can_frame& get_frame() const;
			std::string get_interface() const;
			void set_interface(const std::string& interface);
			timeval& get_timestamp();
			const timeval& get_timestamp() const;
	};
}

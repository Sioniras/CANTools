///////////////////////////////////////////////////////////////////////
// CANOpen protocol
//
//
///////////////////////////////////////////////////////////////////////
#pragma once

#include <can/include/Message.h>

#include <array>
#include <type_traits>

namespace canopen
{
	// --------------------------------------------------------------------
	// Data types
	// --------------------------------------------------------------------
	using data_type = uint8_t;
	using id_type = uint8_t;
	using index_type = uint16_t;
	using subindex_type = uint8_t;

	enum class sdo_type
	{
		unknown = 0,
		read = 0x40,
		write_1byte = 0x2f,
		write_2bytes = 0x2b,
		write_4bytes = 0x23,
	};

	enum class nmt_type
	{
		unknown = 0xDD,

		// Control commands
		command_operational = 0x01,
		command_stopped = 0x02,
		command_preoperational = 0x80,
		command_reset_node = 0x81,
		command_reset_communication = 0x82,

		// Heartbeat states
		state_boot_up = 0x00,
		state_stopped = 0x04,
		state_operational = 0x05,
		state_preoperational = 0x7F,
	};

	// --------------------------------------------------------------------
	// Utility
	// --------------------------------------------------------------------
	template <typename T>
	constexpr data_type as_data(T data)
	{
		static_assert(std::is_enum<T>::value || sizeof(T) == sizeof(data_type));
		return static_cast<data_type>(data);
	}

	template <std::size_t data_length, typename T>
	constexpr auto map_to_data(T value) -> std::array<data_type, data_length>
	{
		static_assert(sizeof(T) >= data_length);

		std::array<data_type, data_length> result;
		for(std::size_t i = 0; i < data_length; i++)
			result[i] = (value >> 8*i) & 0xFF;

		return result;
	}

	template <typename T, std::size_t data_length>
	constexpr auto map_from_data(std::array<data_type, data_length> data) -> T
	{
		static_assert(sizeof(T) >= data_length);
		static_assert(std::is_integral<T>::value);
		T result{ 0 };
		for(std::size_t i = 0; i < data_length; i++)
			result += (static_cast<T>(data[i]) >> i*8);
		return result;
	}

	template <typename T, std::size_t l1, std::size_t l2>
	constexpr auto operator|(const std::array<T,l1>& a1, const std::array<T,l2>& a2)
	{
		std::array<T,l1+l2> result;
		for(std::size_t i = 0; i < l1+l2; i++)
			result[i] = (i < l1) ? a1[i] : a2[i - l1];

		return result;
	}

	template <typename T, std::size_t l>
	constexpr auto operator|(data_type d, const std::array<T,l>& a)
	{
		std::array<T,l+1> result;
		for(std::size_t i = 0; i < l+1; i++)
			result[i] = (i < 1) ? d : a[i - 1];

		return result;
	}

	template <typename T, std::size_t l>
	constexpr auto operator|(const std::array<T,l>& a, data_type d)
	{
		std::array<T,l+1> result;
		for(std::size_t i = 0; i < l+1; i++)
			result[i] = (i < l) ? a[i] : d;

		return result;
	}

	// --------------------------------------------------------------------
	// Message construction
	// --------------------------------------------------------------------
	template <std::size_t data_length>
	constexpr auto message(id_type id, std::array<data_type,data_length> data) -> can_frame
	{
		static_assert(data_length > 0 && data_length <= 8);

		can_frame result{id, {data_length}, 0, 0, 0, {0,0,0,0,0,0,0,0}};
		for(std::size_t i = 0; i < data_length; i++)
			result.data[i] = data[i];

		return result;
	}

	template <sdo_type T>
	constexpr auto message_sdo(id_type id, index_type cobid, subindex_type subindex) -> can_frame
	{
		return message(id, as_data(T) | map_to_data<2>(cobid) | map_to_data<1>(subindex) | 0 | 0 | 0 | 0);
	}

	template <nmt_type T>
	constexpr auto message_nmt(id_type id) -> can_frame
	{
		// Only commands can be sent
		static_assert( T == nmt_type::command_operational
					|| T == nmt_type::command_preoperational
					|| T == nmt_type::command_reset_communication
					|| T == nmt_type::command_reset_node
					|| T == nmt_type::command_stopped);

		return message(0, as_data(T) | map_to_data<1>(id));
	}

	// --------------------------------------------------------------------
	// Message parsing
	// --------------------------------------------------------------------
	constexpr auto get_id(const can_frame& msg) -> id_type
	{
		return msg.can_id & 0x7F;
	}

	template <canid_t mask, canid_t value>
	constexpr auto has_masked_value(const can_frame& msg)
	{
		return ((msg.can_id & mask) == value);
	}

	constexpr auto is_nmt(const can_frame& msg)
	{
		return has_masked_value<0xFFF,0x000>(msg) || has_masked_value<0xF80,0x700>(msg);
	}

	constexpr auto is_emcy(const can_frame& msg)
	{
		return has_masked_value<0xF80,0x080>(msg);
	}

	constexpr auto is_lss(const can_frame& msg)
	{
		return has_masked_value<0xFFF,0x7E4>(msg) || has_masked_value<0xFFF,0x7E5>(msg);
	}

	template <int tpdo_number>
	constexpr auto is_tpdo(const can_frame& msg)
	{
		static_assert(tpdo_number >= 1 && tpdo_number <= 4);
		switch(tpdo_number)
		{
			case 1: return has_masked_value<0xF80,0x180>(msg);
			case 2: return has_masked_value<0xF80,0x280>(msg);
			case 3: return has_masked_value<0xF80,0x380>(msg);
			case 4: return has_masked_value<0xF80,0x480>(msg);
		}
		return false;
	}

	template <int rpdo_number>
	constexpr auto is_rpdo(const can_frame& msg)
	{
		static_assert(rpdo_number >= 1 && rpdo_number <= 4);
		switch(rpdo_number)
		{
			case 1: return has_masked_value<0xF80,0x200>(msg);
			case 2: return has_masked_value<0xF80,0x300>(msg);
			case 3: return has_masked_value<0xF80,0x400>(msg);
			case 4: return has_masked_value<0xF80,0x500>(msg);
		}
		return false;
	}

	constexpr auto is_sdo_request(const can_frame& msg)
	{
		return has_masked_value<0xF80,0x600>(msg);
	}

	constexpr auto is_sdo_response(const can_frame& msg)
	{
		return has_masked_value<0xF80,0x580>(msg);
	}

	constexpr auto is_sdo(const can_frame& msg)
	{
		return (is_sdo_request(msg) || is_sdo_response(msg));
	}

	constexpr auto get_sdo_function_code(const can_frame& msg) -> sdo_type
	{
		auto result = sdo_type::unknown;

		if(msg.len > 0)
		{
			switch(as_data(msg.data[0]))
			{
				case as_data(sdo_type::read):
				case as_data(sdo_type::write_1byte):
				case as_data(sdo_type::write_2bytes):
				case as_data(sdo_type::write_4bytes):
					result = static_cast<sdo_type>(msg.data[0]);
					break;
			}
		}

		return result;
	}

	constexpr auto get_sdo_cobid(const can_frame& msg) -> index_type
	{
		if(msg.len < 3)
			return 0;

		return map_from_data<index_type>(std::array<data_type,2>{{ as_data(msg.data[1]), as_data(msg.data[2]) }});
	}

	constexpr auto get_sdo_subindex(const can_frame& msg) -> subindex_type
	{
		if(msg.len < 4)
			return 0;

		return static_cast<subindex_type>(msg.data[3]);
	}
}

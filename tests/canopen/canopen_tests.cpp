///////////////////////////////////////////////////////////////////////
// Tests for the CANOpen protocol implementation
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <can/include/canopen.h>

TEST(CANOpen, generate_message)
{
	const decltype(can_frame::can_id) id{ 0x12 };
	const std::size_t length = 6;

	auto msg = canopen::message<length>(id, {{0, 1, 2, 3, 4, 5 }});

	EXPECT_EQ(msg.can_id, id);
	EXPECT_EQ(msg.len, length);
	for(std::size_t i = 0; i < length; i++)
		EXPECT_EQ(msg.data[i], i);
}

TEST(CANOpen, generate_sdo_read)
{
	const decltype(can_frame::can_id) id{ 0x12 };
	const auto index = 0x3456;
	const auto subindex = 0x78;

	auto msg = canopen::message_sdo<canopen::sdo_type::read>(id, index, subindex);

	EXPECT_EQ(msg.can_id, id);
	EXPECT_EQ(msg.len, 8);
	EXPECT_EQ(msg.data[0], 0x40);
	EXPECT_EQ(msg.data[1], 0x56);
	EXPECT_EQ(msg.data[2], 0x34);
	EXPECT_EQ(msg.data[3], 0x78);
	EXPECT_EQ(msg.data[4], 0);
	EXPECT_EQ(msg.data[5], 0);
	EXPECT_EQ(msg.data[6], 0);
	EXPECT_EQ(msg.data[7], 0);
}

TEST(CANOpen, generate_sdo_write_1byte)
{
	const decltype(can_frame::can_id) id{ 0x12 };
	const auto index = 0x3456;
	const auto subindex = 0x78;

	auto msg = canopen::message_sdo<canopen::sdo_type::write_1byte>(id, index, subindex);

	EXPECT_EQ(msg.can_id, id);
	EXPECT_EQ(msg.len, 8);
	EXPECT_EQ(msg.data[0], 0x2f);
	EXPECT_EQ(msg.data[1], 0x56);
	EXPECT_EQ(msg.data[2], 0x34);
	EXPECT_EQ(msg.data[3], 0x78);
	EXPECT_EQ(msg.data[4], 0);
	EXPECT_EQ(msg.data[5], 0);
	EXPECT_EQ(msg.data[6], 0);
	EXPECT_EQ(msg.data[7], 0);
}

TEST(CANOpen, generate_sdo_write_2bytes)
{
	const decltype(can_frame::can_id) id{ 0x12 };
	const auto index = 0x3456;
	const auto subindex = 0x78;

	auto msg = canopen::message_sdo<canopen::sdo_type::write_2bytes>(id, index, subindex);

	EXPECT_EQ(msg.can_id, id);
	EXPECT_EQ(msg.len, 8);
	EXPECT_EQ(msg.data[0], 0x2b);
	EXPECT_EQ(msg.data[1], 0x56);
	EXPECT_EQ(msg.data[2], 0x34);
	EXPECT_EQ(msg.data[3], 0x78);
	EXPECT_EQ(msg.data[4], 0);
	EXPECT_EQ(msg.data[5], 0);
	EXPECT_EQ(msg.data[6], 0);
	EXPECT_EQ(msg.data[7], 0);
}

TEST(CANOpen, generate_sdo_write_4bytes)
{
	const decltype(can_frame::can_id) id{ 0x12 };
	const auto index = 0x3456;
	const auto subindex = 0x78;

	auto msg = canopen::message_sdo<canopen::sdo_type::write_4bytes>(id, index, subindex);

	EXPECT_EQ(msg.can_id, id);
	EXPECT_EQ(msg.len, 8);
	EXPECT_EQ(msg.data[0], 0x23);
	EXPECT_EQ(msg.data[1], 0x56);
	EXPECT_EQ(msg.data[2], 0x34);
	EXPECT_EQ(msg.data[3], 0x78);
	EXPECT_EQ(msg.data[4], 0);
	EXPECT_EQ(msg.data[5], 0);
	EXPECT_EQ(msg.data[6], 0);
	EXPECT_EQ(msg.data[7], 0);
}

TEST(CANOpen, generate_nmt_operational)
{
	const decltype(can_frame::can_id) id{ 0x12 };

	auto msg = canopen::message_nmt<canopen::nmt_type::command_operational>(id);

	EXPECT_EQ(msg.can_id, 0x000);
	EXPECT_EQ(msg.len, 2);
	EXPECT_EQ(msg.data[0], 0x01);
	EXPECT_EQ(msg.data[1], id);
}

TEST(CANOpen, generate_nmt_reset)
{
	const decltype(can_frame::can_id) id{ 0x12 };

	auto msg = canopen::message_nmt<canopen::nmt_type::command_reset_node>(id);

	EXPECT_EQ(msg.can_id, 0x000);
	EXPECT_EQ(msg.len, 2);
	EXPECT_EQ(msg.data[0], 0x81);
	EXPECT_EQ(msg.data[1], id);
}

#include <iostream>
#include <memory>
#include <interfaces/include/CANSocket.h>

/*
For testing:
sudo ip link add dev vcan0 type vcan
sudo ifconfig vcan0 up
*/

int main()
{
	auto interface = std::make_unique<CAN::Interfaces::CANSocket>();
	interface->Connect("vcan0");
	interface->SetBlockingMode(false);
	can_frame frame;
	frame.can_id = 0x182;
	frame.data[0] = 0xDE;
	frame.data[1] = 0xAD;
	frame.data[2] = 0xBE;
	frame.data[3] = 0xEF;
	frame.len = 4;
	CAN::Message msg(frame);
	interface->SendMessage(msg);

	bool status = interface->RequestMessage(msg);
	if(status)
	{
		std::cout << "Recieved message from 0x" << std::hex << msg.id() << " (0x" << static_cast<int>(msg.id_short()) << ") ["
				  << static_cast<int>(msg.size()) << "]:";
		for(int i = 0; i < msg.size(); i++)
			std::cout << " " << std::hex << static_cast<int>(msg[i]);
		std::cout << " (interface: " << msg.get_interface() << ")";
		std::cout << std::endl;
	}
	else
	{
		std::cout << "No message received." << std::endl;
	}

	return 0;
}

#include <ting/Socket.hpp>

int main() {
	try {
		ting::SocketLib socketsLib;
		ting::UDPSocket sendSock;
		ting::IPAddress ip("127.0.0.1", 5060);
		sendSock.Open();
		unsigned char data[1024];
		printf("sizeof(data)=%i\n",sizeof(data));
		memcpy(data,"This is a super cool test",25);
		sendSock.Send(data, sizeof(data), ip);
		printf("sent '%s' to %i:%i\n", data, ip.host, ip.port);
	}catch(ting::Socket::Exc &e){
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

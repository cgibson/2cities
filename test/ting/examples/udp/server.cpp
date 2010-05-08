#include <stdio.h>
#include <ting/Socket.hpp>
#include <ting/Buffer.hpp>

int main(int argc, char** argv) {
	printf("starting server\n");
	try {
		ting::SocketLib socketsLib;
		ting::UDPSocket rcvSock;
		ting::IPAddress ip("127.0.0.1", 5060);
		rcvSock.Open(5060);
		while(1){
			ting::Buffer<ting::u8> buf(new ting::u8[2000], 2000);
			rcvSock.Recv(buf, ip);
			printf("'%s'\n", buf.Begin());
		}
	}catch(ting::Socket::Exc &e){
		std::cout << "Network error: " << e.What() << std::endl;
	}
}

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

void main() {
	//Server Ip
	std::string ipAddress = "127.0.0.1";
	//Server Port
	int port = 54000;

	//Initialize Winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0 ) {
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		WSACleanup();
	}

	//Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) {
		std::cerr << "Can't create socket,Err #" << WSAGetLastError << std::endl;
	}
	//Fill in hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		//connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
	}

	//do while look to sedn.recieve data
	char buf[4095];
	std::string userInput;

	do {
		//Prompt the user for some text
		std::cout << ">";
		getline(std::cin, userInput);

		if (userInput.size() > 0) {
			//send text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			
			if (sendResult != SOCKET_ERROR) {
				//wait for response
				ZeroMemory(buf, 4096);
				int bytesRecieved = recv(sock, buf, 4096, 0);
				if (bytesRecieved > 0) {
					//Echo response to console
					std::cout << "SERVER> " << std::string(buf, 0, bytesRecieved) << std::endl;
				}
			}
		}
	} while (userInput.size() > 0);
	//close resources
	closesocket(sock);
	WSACleanup();
}
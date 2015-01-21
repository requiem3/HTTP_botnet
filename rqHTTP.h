//credit for original concept: http://www.cplusplus.com/forum/windows/17264/
#ifdef useHTTP

#ifndef RQHTTP_H
#define RQHTTP_H
class rqHTTP {

private:
	WSADATA wsaData;

public:
	rqHTTP(){};

	int start() {
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cout << "WSAStartup failed.\n";
			system("pause");
			return 1;
		}

		SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		struct hostent *host;
		host = gethostbyname("www.themp3wiz.com");

		SOCKADDR_IN SockAddr;
		SockAddr.sin_port = htons(80);
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

		std::cout << "Connecting...\n";
		if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0){
			std::cout << "Could not connect";
			system("pause");
			return 1;
		}
		std::cout << "Connected.\n";

		send(Socket, "GET / HTTP/1.1\r\nHost: www.themp3wiz.com\r\nConnection: close\r\n\r\n", strlen("GET / HTTP/1.1\r\nHost: www.themp3wiz.com\r\nConnection: close\r\n\r\n"), 0);
		char buffer[10000];

		int nDataLength;
		while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0){
			int i = 0;
			while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
				std::cout << buffer[i];
				i += 1;
			}
		}

		closesocket(Socket);
		WSACleanup();

		return 0;
	}
};
#endif
#endif
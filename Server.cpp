#include<iostream>
#include<winsock.h>

#define PORT 8080

using namespace std;

struct sockaddr_in srv;  //contain details about family,port.ip adrress and some other info.

fd_set fr, fw, fe;   //socket descriptors(ready to read, ready to write to network, exception thowing errors)

int nmaxFd;

int main()
{
	int nRet = 0;

	WSADATA ws; //wsa responsible for all the socket api working.

	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
	{
		cout <<endl<<"WSA failed to initialize." << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "WSA is initialized." << endl;
	}
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //socket descriptor id.
	if (nSocket < 0)
	{
		cout << "The Socket is not opened." << endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else 
	{
		cout << "The Socket is opened successfully." << endl;
	}
	
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	//srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv.sin_addr.s_addr = INADDR_ANY; // assigning the this machine address (ip) bcz we will running this server on local host
	memset(&(srv.sin_zero), 0, 8);
	/*
	//setsockopt
	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);
	if o success adn not zero failure;
	*/

	// blocking socket design
	u_long optval = 0;
	nRet = ioctlsocket(nSocket, FIONBIO, &optval);
	if (nRet != 0)
	{
		cout << endl << "fail blocking socket ioctl call";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		
		cout << endl << "pass blocking socket ioctl call";
	}

	// binding the socket to local port
	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0)
	{
		cout << "Fail to bind to Local port";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << " Successfully able to bind to local port";
	}
	//listen the request from client(queues the request);

	nRet = listen(nSocket, 5); // how many request can be there in the server queue(backlog ,5)

	if (nRet < 0)
	{
		cout << endl << "Fail to start to listen to Local port";
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << endl << " Start Successfully able to listening to local port";
	}
	/*

	int n = 100;

	for (int ni = 0; ni < 64; ni++)
	{
		FD_SET(n, &fr);
		n++;
	}

	cout << endl << fr.fd_count << endl;

	for (int ni = 0; ni < 64; ni++)
	{
		cout<<fr.fd_array[ni]<<endl;
	}

	*/

	//keep waiting for new request and proceed as per the request
	// select is as system call for socket descriptors

	nmaxFd = nSocket;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while (1)
	{

		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);

		cout << endl << "Before select call :" << fr.fd_count;

		nRet = select(nmaxFd + 1, &fr, &fw, &fe, &tv);

		if (nRet > 0)
		{
			// when someone connect or coomunicate over a dedicated msg

		}
		else if (nRet == 0)
		{
			// no connection request
			//none of the socket descriptors are ready
			cout << endl << "nothing on the port :\t" << PORT;
		}
		else
		{
			//select function fails
			cout << endl << "failed select call";
			WSACleanup();
			exit(EXIT_FAILURE);
			
		
		}

		cout << endl << "After the select call :" << fr.fd_count;
		Sleep(2000);
	}

	return 0;


}
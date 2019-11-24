#pragma once

#define SECURITY_WIN32

#include <iostream>
#include <windows.h>
#include <schannel.h>
#include <sspi.h>
#include <fstream>
#include <regex>
#include <chrono>



#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "secur32.lib")
#pragma comment(lib, "crypt32.lib")

class ReqPicture
{
private:

	//variables used to acquire credentials and create a security package handle - GetSChannelHandle()
	SCHANNEL_CRED credData;
	TimeStamp lifeTime;
	CredHandle clientCred; // used for both acquiring credentials and establishing a handshake
	SECURITY_STATUS secStatus; // used in almost every function to check the return status of sspi functions

	//variables used to connect to a socket - ConnectSocket()
	std::string url;
	int port;
	SOCKET sock;
	SOCKADDR_IN sockAddr;
	WSADATA wsaData;
	struct hostent* host;

	//variables used to establish a handshake - DoHandshake()
	SecBuffer buffersI[1];
	SecBufferDesc bufferDescI;
	CtxtHandle contHandle;
	TimeStamp ttlReq;
	unsigned long iscIFlags;

	//variables used to retrieve a certificate just incase the server needs it - GetCertificate()
	PCCERT_CONTEXT aCertContext = NULL;
	unsigned long pkgLen;
	PSecPkgInfo pkgInfo;

	//variables used to dump image binary to file
	std::ofstream file;
	std::string img = "";
public:
	ReqPicture(std::string address);
	bool GetSChannelHandle();
	bool ConnectSocket();
	bool DoHandshake();
	bool GetCertificate();
	std::string Request(std::string request, bool getUrl, std::string fileName="");
	void CloseConnection();
	std::string GetImgURL(DWORD size, PBYTE msg);
	std::string GetImgBinary(DWORD size, PBYTE msg);
	void DecodeJPG(std::string img, std::string name);
	~ReqPicture();
};


static bool FetchPicture(std::string imageName, std::string fileName)
{
	if (imageName.find(" ") != std::string::npos)
	{
		std::replace(imageName.begin(), imageName.end(), ' ', '+');
	}


	//Keep track of how many tries before breaking completely
	int cnt = 0;


	while (cnt < 5)
	{
		cnt++;

		//Create a ReqPicture class variable with the servers name as an argument
		ReqPicture reqPicUrl("google.com");


		//Aquire security context credentials handle
		if (!reqPicUrl.GetSChannelHandle())
		{
			printf("Failed to get credentials, trying again...\n");
			continue;
		}
		printf("Created a security package handler!\n");


		//Connect to the servers socket on port 443
		if (!reqPicUrl.ConnectSocket())
		{
			printf("Failed to connect to server, trying again...\n");
			continue;
		}
		printf("Connected to the server!\n");


		//Establish an SSL handshake
		if (!reqPicUrl.DoHandshake())
		{
			printf("Failed to handshake, trying again...\n");
			continue;
		}
		printf("Established a handshake with server!\n");


		//Fetch picture url, notice how we set the getUrl argument to true
		std::string picUrl = reqPicUrl.Request("GET /search?q=" + imageName + "&tbm=isch HTTP/1.1\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/43.0.2357.134 Safari/537.36\r\nConnection: close\r\n\r\n",
			true);
		if (picUrl.empty())
		{
			printf("Failed to retrieve the picture website URL\n");
			continue;
		}
		printf("Fetched picture website URL\n");


		//Fix up the url
		std::regex reg("\/.+");
		std::smatch m;
		std::regex_search(picUrl, m, reg);
		std::string urlPath = m.str();
		picUrl = std::regex_replace(picUrl, reg, "");


		//Keep track of how many tries to connect to pictures web address before breaking
		int cnt2 = 0;


		while (cnt2 < 3)
		{
			cnt2++;

			//Initialize the ReqPicture class variable to the pictures url
			ReqPicture reqPic(picUrl);


			//Print the pictures url
			std::cout << "Picture url: " + picUrl + urlPath + "\n";


			//The above steps all over again but this time we fetch the actual picture binary and dump it to a file
			//////////////////////////////////////////////////////
			if (!reqPic.GetSChannelHandle())
			{
				continue;
			}
			printf("Created a security package handler!\n");
			if (!reqPic.ConnectSocket())
			{
				continue;
			}
			printf("Connected to the server!\n");
			if (!reqPic.DoHandshake())
			{
				printf("Failed to handshake, trying again...\n");
				continue;
			}
			printf("Established a handshake with server!\n\n");
			//////////////////////////////////////////////////////

			//Here we make a get request to the pictures url to retrieve the image binary,
			//notice how getUrl argument is false so it knows to dump the image binary to the file
			//notice how also a 3rd argument is given which is the file name you chose.
			std::string picFilePath = reqPic.Request("GET " + urlPath + " HTTP/1.1\r\nHost: " + picUrl + "\r\nConnection: close\r\n\r\n",
				false,
				fileName);
			if (!picFilePath.empty())
			{
				printf("Fetched and downloaded an image successfully!");
				return true;
			}
		}
	}
	return false;
}

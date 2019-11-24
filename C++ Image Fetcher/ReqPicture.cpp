#include "ReqPicture.h"
#include "base64.h"


ReqPicture::ReqPicture(std::string address):url(address){}

ReqPicture::~ReqPicture()
{
	CloseConnection();
}

bool ReqPicture::ConnectSocket()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("failed to start WSA\n");
		return false;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("failed to create stream socket\n");
		return false;
	}
	if ((host = gethostbyname(url.c_str())) == NULL) {
		printf("failed to get host name\n");
		return false;
	}
	sockAddr.sin_addr.s_addr = *(unsigned int*)host->h_addr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(443);
	if (connect(sock, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		printf("Failed to connect to server\n");
		return false;
	}

	return true;
}



void ReqPicture::CloseConnection()
{
	DWORD dwType = SCHANNEL_SHUTDOWN;

	buffersI[0].BufferType = SECBUFFER_TOKEN;
	buffersI[0].pvBuffer = &dwType;
	buffersI[0].cbBuffer = sizeof(dwType);

	bufferDescI.cBuffers = 1;
	bufferDescI.pBuffers = buffersI;
	bufferDescI.ulVersion = SECBUFFER_VERSION;

	secStatus = ApplyControlToken(&contHandle, &bufferDescI);

	if (secStatus != SEC_E_OK)
	{
		printf("Failed to send shutdown request to server\n");
		return;
	}

	buffersI[0].pvBuffer = NULL;
	buffersI[0].BufferType = SECBUFFER_TOKEN;
	buffersI[0].cbBuffer = 0;

	bufferDescI.cBuffers = 1;
	bufferDescI.pBuffers = buffersI;
	bufferDescI.ulVersion = SECBUFFER_VERSION;
	secStatus = SEC_I_CONTINUE_NEEDED;
	while (secStatus != SEC_E_OK && secStatus != SEC_E_CONTEXT_EXPIRED)
	{
		secStatus = InitializeSecurityContext(&clientCred, &contHandle, NULL, iscIFlags, 0, 0, NULL,
			0, NULL, &bufferDescI, &iscIFlags, NULL);
	}
	DeleteSecurityContext(&contHandle);
	FreeContextBuffer(buffersI[0].pvBuffer);
	FreeCredentialsHandle(&clientCred);
}

bool ReqPicture::GetSChannelHandle()
{
	ZeroMemory(&credData, sizeof(credData));

	//Use schannel SSPI
	credData.dwVersion = SCHANNEL_CRED_VERSION;

	//Use RSA algorithm for encryption
	ALG_ID supportedAlgs[1] = { CALG_RSA_KEYX };
	credData.cSupportedAlgs = 1;
	credData.palgSupportedAlgs = supportedAlgs;

	//Use the TLS ver 1.0 protocol tunneling
	credData.grbitEnabledProtocols = SP_PROT_TLS1;

	//Acquire a security package handler that uses the principals declared above
	secStatus = AcquireCredentialsHandle(
		NULL,                  // default principal
		(char*)UNISP_NAME,            // name of the SSP
		SECPKG_CRED_OUTBOUND,  // client will use the credentials
		NULL,                  // use the current LOGON id
		&credData,             // protocol-specific data
		NULL,                  // default
		NULL,                  // default
		&clientCred,                // Global private variable clientCred is later passed as an argument to InitializeSecurityContext()
		&lifeTime              // receives the credential time limit
	);

	//If acquiring a handle fails
	if (secStatus != SEC_E_OK)
	{
		printf("Failed to create credentials ~~ Client credentials status: 0x%x\n", secStatus);
		FreeCredentialsHandle(&clientCred);
		return false;
	}

	return true;
}

bool ReqPicture::DoHandshake()
{
	//flags to ask the server to use, if the server declines some of the flags it will change this global private variable itself.
	iscIFlags = 
		ISC_REQ_ALLOCATE_MEMORY;

	//declare security buffer output, the security buffer input is declared in the private section of this class.
	SecBufferDesc bufferDescO;
	SecBuffer buffersO[1];

	//variables to use for receiving handshake data from server with WinSock.recv() function
	int dataLength = 0;
	char recvBuffer[65536];

	//Initialize bufferDesc output and input to receive token from server
	buffersO[0].BufferType = SECBUFFER_TOKEN;
	buffersO[0].cbBuffer = 0;
	buffersO[0].pvBuffer = NULL;
	bufferDescO.ulVersion = SECBUFFER_VERSION;
	bufferDescO.cBuffers = 1;
	bufferDescO.pBuffers = buffersO;

	//Initialize bufferDesc input now instead of later
	bufferDescI.cBuffers = 1;
	bufferDescI.pBuffers = buffersI;
	bufferDescI.ulVersion = SECBUFFER_VERSION;

	//Do this while the server asks us to continue sending information
	do {
		//This happens only after the 'else' statement, when we call IntializeSecurityContext() for
		//the first time. Which is why buffersO[0].pvBuffer isn't NULL anymore.
		if(buffersO[0].pvBuffer != NULL){
			//Send the server data contained in the output buffer that was received from the
			//first call to IntializeSecurityContext()
			if (send(sock, (char*)buffersO[0].pvBuffer, buffersO[0].cbBuffer, 0) == SOCKET_ERROR)
			{
				printf("Failed to send handshake data!\n");
				secStatus = SEC_E_INTERNAL_ERROR;
				goto cleanUp;
			}
			printf("Handshake data sent %i\n", buffersO[0].cbBuffer);

			//Receive data, which might be the token, from the server,
			//so we can set it to the input buffer in the next step
			ZeroMemory(recvBuffer, sizeof(recvBuffer));
			dataLength = recv(sock, recvBuffer, sizeof(recvBuffer), 0);
			if (dataLength == 0 || dataLength == -1)
			{
				printf("No data was received from server!\n");
				secStatus = SEC_E_INTERNAL_ERROR;
				goto cleanUp;
			}
			printf("Handshake data receivied %i\n", dataLength);

			//Set the input buffer to the data received, which might be the token,
			//so we can send it to the server in the next step.
			buffersI[0].pvBuffer = recvBuffer;
			buffersI[0].cbBuffer = dataLength;
			buffersI[0].BufferType = SECBUFFER_TOKEN;

			//Send the input buffer, which might be the token, to the server.
																							 //After the first call we switch the 2nd argument and the 9th argument
			secStatus = InitializeSecurityContext(&clientCred, &contHandle, NULL,             //to send the contHandle received from the first call.
				iscIFlags, 0, 0, &bufferDescI, 0, NULL, &bufferDescO, &iscIFlags, &ttlReq);  //And in the 7th argument we add the buffer input,
																							 //to send the token received by the server
		}
		//First call to IntializeSecurityContext()
		else
		{
			//This is called first and sends a Client hello message to the server
			//telling it what security principals it has available

			//The security principals available are set in the
			//handler 'clientCred' in the function GetSChannelHandle() earlier
			secStatus = InitializeSecurityContextA(&clientCred, NULL, (SEC_CHAR*)url.c_str(), //In the first call 2nd and 7th arguments are
				iscIFlags, 0, 0, NULL, 0, &contHandle, &bufferDescO, &iscIFlags, &ttlReq);   //NULL, to only receive and not send.
		}

		if (secStatus == SEC_E_INCOMPLETE_CREDENTIALS)
		{
			std::cout << "\n\nhaha\n\n";
		}
		//if the security status returned was a status other than OK or CONTINUE then break and return false
		if (secStatus != SEC_E_OK && secStatus != SEC_I_CONTINUE_NEEDED)
		{
			goto cleanUp;
		}
	} while (secStatus == SEC_I_CONTINUE_NEEDED);

cleanUp:

	FreeContextBuffer(buffersO[0].pvBuffer);
	if (secStatus == SEC_E_OK)
		return true;
	return false;
}

bool ReqPicture::GetCertificate()
{
	HCERTSTORE	hMyCertStore = NULL;

	hMyCertStore = CertOpenSystemStore(0, "MY");

	if (hMyCertStore == NULL)
	{
		printf("Failed to open certificate store");
		return false;
	}
	
	aCertContext = CertFindCertificateInStore(hMyCertStore, X509_ASN_ENCODING, 0, CERT_FIND_SUBJECT_STR_A, NULL, NULL);

	if (aCertContext == NULL)
	{
		printf("Failed to retrieve certificate from store");
		return false;
	}

	CertCloseStore(hMyCertStore, 0);
	
	return true;
}

std::string ReqPicture::Request(std::string request, bool getUrl, std::string fileName)
{
	SecBufferDesc descBufferMsg;
	SecBuffer buffersMsg[4];
	SecPkgContext_StreamSizes sizes;
	PBYTE pbHead;
	PBYTE pbData;
	int dataLen = 0;

	secStatus = QueryContextAttributes(&contHandle,
		SECPKG_ATTR_STREAM_SIZES,
		&sizes);

	if (secStatus != SEC_E_OK)
	{
		printf("Failed to query context attributes");
		return "";
	}

	int packetLength = sizes.cbHeader + sizes.cbTrailer + sizes.cbMaximumMessage;

	pbHead = (PBYTE)malloc(sizeof(PBYTE) * packetLength);

	pbData = pbHead + sizes.cbHeader;
	for (int i = 0; i < request.size(); i++)
	{
		pbData[i] = request[i];
	}
	pbData[request.size()] = '\0';

	dataLen = strlen((char*)pbData);

	buffersMsg[0].BufferType = SECBUFFER_STREAM_HEADER;
	buffersMsg[0].cbBuffer = sizes.cbHeader;
	buffersMsg[0].pvBuffer = pbHead;

	buffersMsg[1].BufferType = SECBUFFER_DATA;
	buffersMsg[1].cbBuffer = dataLen;
	buffersMsg[1].pvBuffer = pbData;

	buffersMsg[2].BufferType = SECBUFFER_STREAM_TRAILER;
	buffersMsg[2].cbBuffer = sizes.cbTrailer;
	buffersMsg[2].pvBuffer = pbData + dataLen;

	buffersMsg[3].BufferType = SECBUFFER_EMPTY;
	buffersMsg[3].cbBuffer = 0;
	buffersMsg[3].pvBuffer = NULL;

	descBufferMsg.cBuffers = 4;
	descBufferMsg.pBuffers = buffersMsg;
	descBufferMsg.ulVersion = SECBUFFER_VERSION;

	secStatus = EncryptMessage(&contHandle, 0, &descBufferMsg, 0);

	if (secStatus != SEC_E_OK)
	{
		printf("Failed to encrypt message");
		return "";
	}

	dataLen = buffersMsg[0].cbBuffer + buffersMsg[1].cbBuffer + buffersMsg[2].cbBuffer;

	if (send(sock, (char*)pbHead, dataLen, 0) == SOCKET_ERROR)
	{
		printf("Failed to send data");
		return "";
	}

	printf("%i bytes of data sent\n\n", dataLen);

	int dataLength = 0;
	dataLen = 0;

	descBufferMsg.ulVersion = SECBUFFER_VERSION;
	descBufferMsg.cBuffers = 4;
	descBufferMsg.pBuffers = buffersMsg;


	//open global private variable file to keep writing and closing it after the loop.
	if(!fileName.empty())
		file.open(fileName, std::ios::binary);


	while (true)
	{
		if (secStatus == SEC_E_INCOMPLETE_MESSAGE || dataLen == 0)
		{
			dataLength = recv(sock, (char*)(pbHead + dataLen), packetLength - dataLen, 0);
			if (dataLength == SOCKET_ERROR)
			{
				printf("Failed to recv data");
				return "";
			}
			if (dataLength == 0)
			{
				break;
			}

			dataLen += dataLength;
		}

		buffersMsg[0].pvBuffer = pbHead;
		buffersMsg[0].cbBuffer = dataLen;
		buffersMsg[0].BufferType = SECBUFFER_DATA;

		buffersMsg[1].BufferType = SECBUFFER_EMPTY;
		buffersMsg[2].BufferType = SECBUFFER_EMPTY;
		buffersMsg[3].BufferType = SECBUFFER_EMPTY;

		secStatus = DecryptMessage(&contHandle, &descBufferMsg, 0, NULL);
		if (secStatus == SEC_E_INCOMPLETE_MESSAGE)
		{
			continue;
		}

		dataLen = 0;
		int cnt = 0;
		for (int i = 1; i < 4; i++)
		{
			if (buffersMsg[i].BufferType == SECBUFFER_EXTRA && dataLen == 0)
			{
				MoveMemory(pbHead, buffersMsg[i].pvBuffer, buffersMsg[i].cbBuffer);
				dataLen = buffersMsg[i].cbBuffer;
			}
			if (buffersMsg[i].BufferType == SECBUFFER_DATA && cnt == 0)
			{
				if (getUrl) {
					std::string URL = GetImgURL(buffersMsg[i].cbBuffer, (PBYTE)buffersMsg[i].pvBuffer);
					if (!URL.empty())
					{
						free(pbHead);
						return URL;
					}
				}
				else
				{
					DecodeJPG(GetImgBinary(buffersMsg[i].cbBuffer, (PBYTE)buffersMsg[i].pvBuffer), fileName);
				}
				cnt++;
			}
		}
	}
	if (!getUrl)
	{
		file.close();
		return "done";
	}
	return "";
}

std::string ReqPicture::GetImgURL(DWORD size, PBYTE msg)
{
	std::string sMsg = "";


	for (int i = 0; i < size; i++)
	{
		if (sMsg == "https://" || !img.empty())
		{
			if (img.size() > 200)
			{
				img = "";
				sMsg = msg[i];
			}
			else if (img.find("https://") != std::string::npos)
			{
				img = "";
				sMsg = msg[i];
			}
			else if (img.find(".jpg") != std::string::npos)
			{
				if (msg[i] == 34)
				{
					return img;
				}
				else
				{
					img = "";
				}
			}
			else if (msg[i] > 31 && msg[i] < 127 && msg[i] != '%')
			{
				img += msg[i];
			}
		}
		else {
			if (msg[i] > 31 && msg[i] < 127 && msg[i] != '%')
			{
				sMsg += msg[i];
			}
			else if (msg[i] == '\r' || msg[i] == '\n')
			{
				sMsg += msg[i];
			}
			else
			{
				sMsg += ' ';
			}
		}
		if (sMsg.size() > 8)
		{
			sMsg = "";
		}
	}

	//Try this later

	/*std::regex reg("https:\/\/.+\.jpg\"");
	std::smatch m;
	if (std::regex_search(sMsg, m, reg))
	{
		return m.str();
	}*/ 

	return "";
}

std::string ReqPicture::GetImgBinary(DWORD size, PBYTE msg)
{
	std::string sMsg = "";

	for (int i = 0; i < size; i++)
	{
		if (!img.empty())
		{
			sMsg += msg[i];
		}
		else if((msg[i] < 32 || msg[i] > 126) && (msg[i] != '\r' && msg[i] != '\n'))
		{
			sMsg += msg[i];
			img = "start";
		}
	}

	return sMsg;
}

void ReqPicture::DecodeJPG(std::string imgBinary, std::string fileName)
{
	file.write(imgBinary.c_str(), imgBinary.size());
}

//bool ReqPicture::Renegotiate()
//{
//	SecBufferDesc bufferDescO;
//	SecBuffer buffersO[1];
//	
//	buffersO[0].BufferType = SECBUFFER_TOKEN;
//	buffersO[0].cbBuffer = 0;
//	buffersO[0].pvBuffer = NULL;
//	bufferDescO.ulVersion = SECBUFFER_VERSION;
//	bufferDescO.cBuffers = 1;
//	bufferDescO.pBuffers = buffersO;
//
//	secStatus = InitializeSecurityContext(&clientCred, &contHandle, NULL,
//		iscIFlags, 0, 0, &bufferDescI, 0, NULL, &bufferDescO, &iscIFlags, &ttlReq);
//
//	if (secStatus != SEC_E_OK)
//	{
//		printf("Failed to renegotiate with the server.");
//		return false;
//	}
//
//	return true;
//}
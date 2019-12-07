//
// Copyright (C) 2003-2004 Trevor Hogan
//

#ifndef CLIENT_H
 #define CLIENT_H

#define COMPRESS_NONE		0
#define COMPRESS_DEFLATE	1
#define COMPRESS_GZIP		2

#define CS_RECVHEADERS		0
#define CS_PROCESSHEADERS	1
#define CS_RECVBODY			2
#define CS_MAKERESPONSE		3
#define CS_SEND				4

#define GPBUF_SIZE			8192

extern char gpBuf[GPBUF_SIZE];

class CClient
{
public:
	CClient( SOCKET &sckClient, struct sockaddr_in &sinAddress, const unsigned int &cuiTimeOut, const char &ccCompression );
	virtual ~CClient( );
 
	bool Update( );

	void Reset( );

private:
	SOCKET m_sckClient;

	unsigned char m_ucState;
	unsigned int m_uiTimeOut;
	char m_cCompression;
	string m_strReceiveBuf;
	string m_strSendBuf;
	struct request_t rqst;
	struct response_t rsp;
	bool m_bKeepAlive;
	unsigned long m_ulLast;
	bool m_bReset;
};

#endif

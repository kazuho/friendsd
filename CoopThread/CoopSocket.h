/**
 * CoopSocket.h
 */

#ifndef COOPSOCKET_H
#define COOPSOCKET_H

#include "CoopSleeper.h"
#include "CoopRunnable.h"
#include "SockUtils.h"
#include "sstring.h"
extern "C" {
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
}

class CoopSocket : public CoopThread {
public:
	typedef CoopSocket MyType;
	typedef CoopThread super;
protected:
	int mSock;
	enum EState {
		active, write_block, read_block, inactive
	} mState;
	bool mSuspended; // overrides state of the socket
public:
	CoopSocket();
	virtual ~CoopSocket();
	virtual bool Activate();
	virtual bool Suspend();
	virtual bool Unsuspend();
	bool IsSuspended() const { return mSuspended; }
	bool Close();
	int GetSock() const { return mSock; }
	bool IsValidSock() const { return (mSock != -1); }
protected:
	MyType* Init(CoopThreadMgr* inMgr, int inSock);
private:
	static int StateModifier(ThreadSet&, int inWaitTimeMillis);
};

class CoopSocketConnector : public CoopSocket {
public:
	typedef CoopSocketConnector MyType;
	typedef CoopSocket super;
	typedef CoopRunnable<CoopSleeper, MyType> TimeoutChecker;
protected:
	int mTimeoutSec;
	int mRawSock;
	sockaddr_in mSockaddr;
	CoopThread* mTimeoutChecker;
public:
	CoopSocketConnector();
	virtual ~CoopSocketConnector();
	MyType* Init(CoopThreadMgr* inMgr, unsigned long inAddr, int inPort, int inTimeoutSec = 10);
	MyType* Init(CoopThreadMgr* inMgr, const sstring& inHost, int inPort, int inTimeoutSec = 10);
	virtual bool Deactivate();
	bool IsConnecting();
private:
	bool OnTimeout(TimeoutChecker* inChecker);
public:
	static int sTimeoutSec;
};

class CoopSocketListener : public CoopSocket {
public:
	typedef CoopSocketListener MyType;
	typedef CoopSocket super;
public:
	CoopSocketListener();
	virtual ~CoopSocketListener();
	MyType* Init(CoopThreadMgr* inMgr, int inPort);
	virtual bool Deactivate();
	int Accept(sockaddr_in &outSockaddr);
	int Accept();
};

class CoopSocketReader : public CoopSocket {
public:
	typedef CoopSocketReader MyType;
	typedef CoopSocket super;
public:
	CoopSocketReader();
	virtual ~CoopSocketReader();
	MyType* Init(CoopThreadMgr* inMgr, int inSock);
	virtual bool Deactivate();
	int Read(char* const outBuffer, int inMaxSize);
};

class CoopSocketWriter : public CoopSocket {
public:
	typedef CoopSocketWriter MyType;
	typedef CoopSocket super;
public:
	CoopSocketWriter();
	virtual ~CoopSocketWriter();
	MyType* Init(CoopThreadMgr* inMgr, int inSock);
	virtual bool Deactivate();
	int Write(const char* inBuffer, int inMaxSize);
};

#endif

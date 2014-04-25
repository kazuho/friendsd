/**
 * CoopFinger.h
 *
 * Broadcasts(pull model) when all data have been received.
 */

#ifndef COOPFINGER_H
#define COOPFINGER_H

#include <vector.h>
#include "CoopRunnable.h"
#include "CoopSocket.h"
#include "Subject.h"
#include "Buffer.h"

class CoopFinger : public Subject {
public:
	typedef CoopFinger MyType;
	typedef CoopRunnable<CoopSocketConnector, MyType> Connector;
	typedef CoopRunnable<CoopSocketWriter, MyType> Requester;
	typedef CoopRunnable<CoopSocketReader, MyType> Receiver;
	enum { port = 79 };
	enum { trying, succeed, error };
protected:
	unsigned long mHostAddr;
	int mState;
	Buffer mBuffer;
	CoopSocket* mCurrentThread, * mOldThread;
public:
	CoopFinger(CoopThreadMgr *inMgr, unsigned long inAddr, int inTimeoutSec);
	virtual ~CoopFinger();
	bool IsCompleted() const { return mState != trying; }
	bool IsValid() const { return (mState == succeed); }
	int GetState() const { return mState; }
	const Buffer& GetBuffer() const { return mBuffer; }
private:
	bool WhileConnecting(Connector*);
	bool WhileRequesting(Requester*);
	bool WhileReceiving(Receiver*);
};

#endif


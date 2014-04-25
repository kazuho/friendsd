/**
 * CoopFinger.cc
 */

extern "C" {
#include <assert.h>
#include <fcntl.h>
}
#include "CoopFinger.h"
#include "SockUtils.h"

//#define DEBUG_MSG

CoopFinger::CoopFinger(CoopThreadMgr *inMgr, unsigned long inAddr, int inTimeoutSec)
: mHostAddr(inAddr), mState(trying), mBuffer()
{
	mCurrentThread = NEW(Connector, (this, MyType::WhileConnecting))
		->Init(inMgr, inAddr, port, inTimeoutSec);
	mOldThread = 0;
#ifdef DEBUG_MSG
	cout << "CoopFinger::CoopFinger" << mCurrentThread << mOldThread << endl;
#endif
}

CoopFinger::~CoopFinger()
{
	if (mOldThread != 0) {
		DELETE(mOldThread);
	}
	if (mCurrentThread != 0) {
		DELETE(mCurrentThread);
	}
}

bool
CoopFinger::WhileConnecting(Connector* ioConnector)
{
#ifdef DEBUG_MSG
	cout << "CoopFinger::WhileConnecting(" << ioConnector << ')' << flush;
#endif
	assert(mCurrentThread == ioConnector);
	
	if (mOldThread != 0) {
		DELETE(mOldThread);
		mOldThread = 0;
	}
	
	if (! ioConnector->IsConnecting()) {
		if (ioConnector->IsValidSock()) {
			goto ON_NEXT;
		} else {
			goto ON_END;
		}
	}
#ifdef DEBUG_MSG
	cout << " connecting" << endl;
#endif
	return true;
 ON_END:
#ifdef DEBUG_MSG
	cout << " error" << endl;
#endif
	mCurrentThread = 0;
	mOldThread = ioConnector;
	mState = error;
	NotifyAll(0);
	return false;
 ON_NEXT:
#ifdef DEBUG_MSG
	cout << " connected" << endl;
#endif
	mCurrentThread = NEW(Requester, (this, MyType::WhileRequesting))
		->Init(ioConnector->GetThreadMgr(), ioConnector->GetSock());
	mOldThread = ioConnector;
	return false;
}

bool
CoopFinger::WhileRequesting(Requester *ioRequester)
{
#ifdef DEBUG_MSG
	cout << "WhileRequesting" << endl;
#endif
	assert(ioRequester == mCurrentThread);
	if (mOldThread != 0) {
		DELETE(mOldThread);
		mOldThread = 0;
	}
	
	switch (ioRequester->Write("\n", 1)) {
	case -1:
		goto ON_ERROR;
	case 0:
		break;
	default:
		goto ON_NEXT;
	}
	return true;
 ON_ERROR:
	ioRequester->Close();
	mCurrentThread = 0;
	mOldThread = ioRequester;
	mState = error;
	NotifyAll(0);
	return false;
 ON_NEXT:
	mCurrentThread = NEW(Receiver, (this, MyType::WhileReceiving))
		->Init(ioRequester->GetThreadMgr(), ioRequester->GetSock());
	mOldThread = ioRequester;
	return false;
}

bool
CoopFinger::WhileReceiving(Receiver *ioReceiver)
{
#ifdef DEBUG_MSG
	cout << "WhileReceiving" << endl;
#endif
	static char sBuf[1024];
	
	assert(mCurrentThread == ioReceiver);
	if (mOldThread != 0) {
		DELETE(mOldThread);
		mOldThread = 0;
	}
	
	int aResult = ioReceiver->Read(sBuf, sizeof(sBuf) - 1);
	switch (aResult) {
	case -1: // on close
		mCurrentThread = 0;
		mOldThread = ioReceiver;
		mState = succeed;
		goto ON_END;
	case 0: // on block
		break;
	default:
		mBuffer.Push(sBuf, aResult);
		break;
	}
	return true;
 ON_END:
	mState = succeed;
	ioReceiver->Close();
	NotifyAll(0);
	return false;
}

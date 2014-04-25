/**
 * FPTalker.cc
 */

extern "C" {
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
}

#include "FPTalker.h"
#include "FingerContainer.h"
#include "sstring.h"

//#define DEBUG_MSG

FPTalker::FPTalker(FPFactory *inFactory, int inSock)
: super(inFactory), mWriteBufferIndex(0), mLeftRequests(0)
{
	mCurrentThread = NEW(Reader, (this, MyType::ReadRequest))
		->Init(mFactory->GetThreadMgr(), inSock);
	mOldThread = 0;
	
	StartTimeout();
}

FPTalker::~FPTalker()
{
#ifdef DEBUG_MSG
	cout << "FPTalker::~FPTalker" << endl;
#endif
	if (mCurrentThread != 0) {
		mCurrentThread->Close();
		DELETE(mCurrentThread);
	}
	if (mOldThread != 0) {
		DELETE(mOldThread);
	}
}

bool
FPTalker::IsCompleted() const
{
	return (mCurrentThread == 0 && mLeftRequests == 0);
}

bool
FPTalker::ReadRequest(Reader *ioReader)
{
	assert(ioReader == mCurrentThread);
	
	static char sBuf[1024];
	int aResult = ioReader->Read(sBuf, sizeof(sBuf) - 1);
#ifdef DEBUG_MSG
	cout << "FPTalker::ReadRequest(Reader*(" << ioReader->GetSock() << ")) : " << aResult << endl;
#endif
	switch (aResult) {
	case -1:
		goto ON_ERROR;
	case 0:
		goto ON_CONTINUE;
	default:
		StartTimeout(); // restart timeout
		for (int j = 0; j < aResult; j++) {
			switch (sBuf[j]) {
			case '\n':
			case '\r':
				goto ON_COMPLETE;
			default:
				mReadBuffer.Push(sBuf[j]);
				break;
			}
		}
		goto ON_CONTINUE;
	}
 ON_CONTINUE:
	return true;
 ON_ERROR:
	ioReader->Close();
	mCurrentThread = 0;
	mOldThread = ioReader;
	return false;
 ON_COMPLETE:
	mCurrentThread = 0;
	if (ParseRequest() != 0) {
		mCurrentThread = NEW(Writer, (this, MyType::Write))
			->Init(ioReader->GetThreadMgr(), ioReader->GetSock());
		mOldThread = ioReader;
		mReadBuffer.Clear();
		StopTimeout();
	} else {
		ioReader->Close();
		mOldThread = ioReader;
	}
	return false;
}

bool
FPTalker::Write(Writer *ioWriter)
{
#ifdef DEBUG_MSG
	cout << "FPTalker::Write(Writer*(" << ioWriter->GetSock() << "))" << endl;
#endif
	assert(ioWriter == mCurrentThread);
	if (mOldThread != 0) {
		DELETE(mOldThread);
		mOldThread = 0;
	}
	
	if (mWriteBuffer.GetSize() > 0) {
		int aResult = ioWriter->Write(mWriteBuffer.Begin(), mWriteBuffer.GetSize());
		switch (aResult) {
		case -1:
			goto ON_CLOSE;
		case 0:
			goto ON_CONTINUE;
		default:
			StartTimeout();
			mWriteBuffer.Pop(aResult);
			if (mWriteBuffer.GetSize() == 0 && mLeftRequests == 0) {
				goto ON_COMPLETE;
			} else {
				goto ON_CONTINUE;
			}
		}
	} else {
		if (mLeftRequests == 0) {
			goto ON_COMPLETE;
		} else {
			StopTimeout();
			ioWriter->Suspend();
			goto ON_CONTINUE;
		}
	}
 ON_CONTINUE:
	return true;
 ON_COMPLETE:
 ON_CLOSE:
	ioWriter->Close();
	mOldThread = ioWriter;
	mCurrentThread = 0;
	return false;
}

void
FPTalker::OnTimeout()
{
	if (mCurrentThread != 0) {
		mCurrentThread->Close();
		DELETE(mCurrentThread);
		mCurrentThread = 0;
	}
	if (mOldThread != 0) {
		DELETE(mOldThread);
		mOldThread = 0;
	}
}

void
FPTalker::Update(OneTimeSubject* inFrom, void*)
{
	FingerContainer* aContainer = (FingerContainer*)inFrom;
#ifdef DEBUG_MSG
	cout << "FPTalker::Update(" << aContainer->GetHostname() << ",): " << mLeftRequests << endl;
#endif
		
	mWriteBuffer.Push('+');
	mWriteBuffer.PushStr(aContainer->GetHostname());
	mWriteBuffer.Push(' ');
	
	if (aContainer->IsError()) {
		mWriteBuffer.PushStr("1 0\n"); // result code ' ' size
	} else {
		static char sBuf[64];
		::sprintf(sBuf, "0 %u\n", aContainer->GetBuffer().GetSize()); // result code \s size
		mWriteBuffer.PushStr(sBuf);
		
		mWriteBuffer.Push(aContainer->GetBuffer());
	}
	
	mLeftRequests--;
	if (mCurrentThread != 0 && mCurrentThread->IsValidSock()) {
		mCurrentThread->Unsuspend(); // i.e. ioWriter->Unsuspend()
		StartTimeout();
	}
}

int
FPTalker::ParseRequest()
{
	int aCount = 0;
	char aBuf[256];
	int aBufIndex = 0;
	
	for (char* aIt = mReadBuffer.Begin();
			 aIt != mReadBuffer.End();
			 ++aIt) {
		if (isspace(*aIt)) {
			if (aBufIndex != 0) {
				aBuf[aBufIndex] = '\0';
				RequestFinger(aBuf); // request
				aCount++;
				aBufIndex = 0;
			} else {
				// do nothing
			}
		} else {
			aBuf[aBufIndex++] = *aIt;
		}
	}
	if (aBufIndex != 0) {
		aBuf[aBufIndex] = '\0';
		RequestFinger(aBuf); // request
		aCount++;
	}
	
	return aCount;
}

void
FPTalker::RequestFinger(const char* aHostname)
{
#ifdef DEBUG_MSG
	cout << "FPTalker::RequestFinger(" << aHostname << ')' << endl;
#endif
	mLeftRequests++;
	mFactory->GetFingerContainer(aHostname)->Attach(this);
}

/**
 * FriendsdTalker.cc
 */

#include "FriendsdTalker.h"
#include "sstring.h"

//#define DEBUG_MSG

#define BLOCK_OPCODE(x) (((IOHeader*)(x).Begin())->GetOpcode())
#define BLOCK_LENGTH(x) (((IOHeader*)(x).Begin())->GetLength())
#define READ_BLOCK(x) ((x).GetSize() >= BLOCK_LENGTH(x) + sizeof(IOHeader))

#define READ_SHORT(x) ((x).GetSize() >= sizeof(unsigned short))
#define GET_OPCODE(x) (ntohs(*(unsigned short*)(x).Begin()))

FriendsdTalker::FriendsdTalker(FPFactory* inFactory, int inSock)
:	super(inFactory),
	mSleeper(this, OnWakeup),
	mIsAttached(false)
{
	// keep it deactivated until req_wait is requested
	mSleeper.Init(mFactory->GetThreadMgr());
	mSleeper.Deactivate();
	
	mCurrentThread = NEW(Reader, (this, OnReceiveUserID))
			->Init(mFactory->GetThreadMgr(), inSock);
	mOldThread = 0;
	
	mLMofContainer = mFactory->GetFriendsdContainer()->GetLastModified() - 1;
		// assure both values are different
	
	StartTimeout();
}

FriendsdTalker::~FriendsdTalker()
{
	if (mCurrentThread != 0) {
		mCurrentThread->Close();
		DELETE(mCurrentThread);
	}
	if (mOldThread != 0) {
		DELETE(mOldThread);
	}
}

void
FriendsdTalker::OnTimeout()
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

bool
FriendsdTalker::IsCompleted() const
{
	return mCurrentThread == 0 && (! mIsAttached);
}

bool
FriendsdTalker::OnReceiveUserID(Reader* ioReader)
{
#ifdef DEBUG_MSG
	cout << "FriendsdTalker::OnReceiveUserID(Reader*): " << mReadBuffer.GetSize() << endl;
#endif
	assert(ioReader == mCurrentThread);
	
	if (mOldThread != 0) {
		DELETE(mOldThread);
		mOldThread = 0;
	}
	
	if (READ_BLOCK(mReadBuffer)) {
		goto ON_NEXT;
	}
	if (Read(ioReader)) {
		if (READ_BLOCK(mReadBuffer)) {
			goto ON_NEXT;
		} else {
			goto ON_CONTINUE;
		}
	}
// ON_CLOSE:
	ioReader->Close();
	mOldThread = mCurrentThread;
	mCurrentThread = 0;
	StopTimeout();
	return false;
 ON_NEXT:
#ifdef DEBUG_MSG
	cout << "Popping: " << BLOCK_LENGTH(mReadBuffer) + sizeof(IOHeader) << endl;
#endif
	mReadBuffer.Pop(BLOCK_LENGTH(mReadBuffer) + sizeof(IOHeader));
	mOldThread = mCurrentThread;
	mCurrentThread = NEW(Reader, (this, OnReceiveCmd))
			->Init(mFactory->GetThreadMgr(), ioReader->GetSock());
	StartTimeout();
	return false;
 ON_CONTINUE:
	return true;
}

bool
FriendsdTalker::OnReceiveCmd(Reader* ioReader)
{
#ifdef DEBUG_MSG
	cout << "FriendsdTalker::OnReceiveCmd(Reader*): " << mReadBuffer.GetSize() << endl;
#endif
	assert(ioReader == mCurrentThread);
	
	if (mOldThread != 0) {
		DELETE(mOldThread);
		mOldThread = 0;
	}
	
	if (READ_SHORT(mReadBuffer)) {
		goto ON_NEXT;
	}
	if (Read(ioReader)) {
		if (READ_SHORT(mReadBuffer)) {
			goto ON_NEXT;
		} else {
			goto ON_CONTINUE;
		}
	}
// ON_CLOSE:
	ioReader->Close();
	mOldThread = mCurrentThread;
	mCurrentThread = 0;
	StopTimeout();
	return false;
 ON_CONTINUE:
	return true;
 ON_NEXT:
	mCmd = GET_OPCODE(mReadBuffer);
#ifdef DEBUG_MSG
	cout << "Command: " << mCmd << endl;
#endif
	mReadBuffer.Pop(sizeof(unsigned short));
	switch (mCmd) {
	case req_wait:
		mOldThread = mCurrentThread;
		mCurrentThread = NEW(Writer, (this, OnWrite))
				->Init(mFactory->GetThreadMgr(), ioReader->GetSock());
		mCurrentThread->Suspend();
		StopTimeout();
		{
			FriendsdContainer* aContainer = mFactory->GetFriendsdContainer();
			if (mLMofContainer != aContainer->GetLastModified()) {
				mIsAttached = true;
				aContainer->Attach(this);
			} else {
				mSleeper.Sleep(eWakeupSec);
			}
		}
		break;
	case req_all:
	case req_err:
		mOldThread = mCurrentThread;
		mCurrentThread = NEW(Writer, (this, OnWrite))
				->Init(mFactory->GetThreadMgr(), ioReader->GetSock());
		mCurrentThread->Suspend();
		StopTimeout();
		mIsAttached = true;
		mFactory->GetFriendsdContainer()->Attach(this);
		break;
	case req_sv:
		mOldThread = mCurrentThread;
		mCurrentThread = NEW(Writer, (this, OnWrite))
				->Init(mFactory->GetThreadMgr(), ioReader->GetSock());
		SetServersToWrite(mFactory->GetFriendsdContainer()->GetServers());
		StartTimeout();
		break;
	case close_connection:
	default:
		ioReader->Close();
		mOldThread = mCurrentThread;
		mCurrentThread = 0;
		StopTimeout();
		break;
	}
	return false;
}

bool
FriendsdTalker::Read(Reader* ioReader)
{
	static char sBuf[2048];
	int aReadResult = ioReader->Read(sBuf, sizeof(sBuf));
	
	switch (aReadResult) {
	case -1: // error
		goto ON_CLOSE;
	case 0: // block
		break;
	default:
		StartTimeout();
		mReadBuffer.Push(sBuf, aReadResult);
		break;
	}
#ifdef DEBUG_MSG
	cout << "FriendsdTalker::Read: ";
	for (int j = 0; j < mReadBuffer.GetSize(); j++) {
		char aBuf[4];
		::sprintf(aBuf, "%02X ", mReadBuffer.Begin()[j]);
		cout << aBuf;
	}
	cout << endl;
#endif
	return true;
 ON_CLOSE:
	return false;
}

bool
FriendsdTalker::OnWrite(Writer* ioWriter)
{
#ifdef DEBUG_MSG
	cout << "FriendsdTalker::OnWrite(Writer*): " << mWriteBuffer.GetSize() << endl;
#endif
	assert(ioWriter == mCurrentThread);
	
	if (mOldThread != 0) {
		DELETE(mOldThread);
		mOldThread = 0;
	}
	
	int aWriteResult;
	
	if (mWriteBuffer.GetSize() == 0) {
		goto ON_NEXT;
	}
	
	aWriteResult = ioWriter->Write(mWriteBuffer.Begin(), mWriteBuffer.GetSize());
	switch(aWriteResult) {
	case -1:
		goto ON_CLOSE;
	case 0:
		break;
	default:
		StartTimeout();
		mWriteBuffer.Pop(aWriteResult);
		break;
	}
	
// ON_CONTINUE:
	return true;
 ON_CLOSE:
	ioWriter->Close();
	mOldThread = mCurrentThread;
	mCurrentThread = 0;
	StopTimeout();
	return false;
 ON_NEXT:
	mOldThread = mCurrentThread;
	mCurrentThread = NEW(Reader, (this, OnReceiveCmd))
			->Init(mFactory->GetThreadMgr(), ioWriter->GetSock());
	StartTimeout();
	return false;
}

void
FriendsdTalker::Update(OneTimeSubject* inFrom, void*)
{
#ifdef DEBUG_MSG
	cout << "FriendsdTalker::Update(OneTimeSubject*, void*)" << endl;
#endif
	FriendsdContainer* aContainer = (FriendsdContainer*)inFrom;
	
	switch (mCmd) {
	case req_wait:
	case req_all:
		mLMofContainer = aContainer->GetLastModified();
		SetUsersToWrite(aContainer->GetUsers());
		break;
	case req_err:
		SetServersToWrite(aContainer->GetErrorServers());
		break;
	}
	mCurrentThread->Unsuspend();
	StartTimeout();
	mIsAttached = false;
}

bool
FriendsdTalker::OnWakeup(Sleeper* ioSleeper)
{
#ifdef DEBUG_MSG
	cout << "FriendsdTalker::OnWakeup(Reader*)" << endl;
#endif
	assert(ioSleeper == &mSleeper);
	
	FriendsdContainer* aContainer = mFactory->GetFriendsdContainer();
	if (mLMofContainer != aContainer->GetLastModified() || // if updated
			(! aContainer->IsValid())) { // or is obsolete
		ioSleeper->Deactivate();
		mIsAttached = true;
		aContainer->Attach(this);
	} else {
		ioSleeper->Sleep(eWakeupSec);
	}
	return true; // should not throw mSleeper away
}

void
FriendsdTalker::SetUsersToWrite(const UserVect& inUsers)
{
	// write count
	{
		unsigned short aCount = htons(inUsers.size());
		mWriteBuffer.Push((char*)(&aCount), sizeof(aCount));
	}
	
	// write each element
	for (UserVect::const_iterator aUserIt = inUsers.begin();
			aUserIt != inUsers.end();
			++aUserIt) {
		IOHeader aHeader;
		aHeader.SetOpcode(ack);
		aHeader.SetLength(sizeof(User));
		mWriteBuffer.Push(aHeader, sizeof(IOHeader));
		mWriteBuffer.Push((char*)(&*aUserIt), sizeof(User));
	}
}

void
FriendsdTalker::SetServersToWrite(const ServerVect& inServers)
{
	// write count
	{
		unsigned short aCount = htons(inServers.size());
		mWriteBuffer.Push((char*)(&aCount), sizeof(aCount));
	}
	
	// write each element
	for (ServerVect::const_iterator aServerIt = inServers.begin();
			aServerIt != inServers.end();
			++aServerIt) {
		IOHeader aHeader;
		aHeader.SetOpcode(ack);
		aHeader.SetLength(sizeof(Server));
		mWriteBuffer.Push(aHeader, sizeof(IOHeader));
		mWriteBuffer.Push((char*)(&*aServerIt), sizeof(Server));
	}
}

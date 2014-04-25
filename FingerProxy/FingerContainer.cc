/**
 * FingerContainer.cc
 */

#include "FingerContainer.h"
#include "FPTalker.h"
extern "C" {
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
}

//#define DEBUG_MSG

int FingerContainer::sValidSec = 90;

FingerContainer::FingerContainer(FPFactory* inFactory, const sstring& inHostname)
: mFactory(inFactory), mCurrentFinger(0), mTimer(0), mHostname(inHostname), mAliveUntil(0)
{
	if ((mHostaddr = SockUtils::GetHostByName(inHostname)) == 0xffffffff) {
		goto ON_ERROR;
	}
	
	ReloadFinger();
	return;
 ON_ERROR:
	mCurrentFinger = 0;
	mAliveUntil = mFactory->GetAliveUntil();
	return;
}

FingerContainer::~FingerContainer()
{
	NotifyAll(0);
	if (mCurrentFinger != 0) {
		DELETE(mCurrentFinger);
	}
	if (mTimer != 0) {
		DELETE(mTimer);
	}
}

const sstring&
FingerContainer::GetHostname() const
{
	return mHostname;
}

const Buffer&
FingerContainer::GetBuffer() const
{
	return mCurrentFinger->GetBuffer();
}

bool
FingerContainer::IsError() const
{
	return (mCurrentFinger == 0 ||
					mCurrentFinger->GetState() == CoopFinger::error);
}

int
FingerContainer::GetAliveUntil() const
{
	return (IsValid() ? mAliveUntil : 0x7fffffff);
}

bool
FingerContainer::IsObsolete() const
{
	return (GetAliveUntil() < ::time(NULL));
}

bool
FingerContainer::IsValid() const
{
#ifdef DEBUG_MSG
	cout << "FingerContainer::IsValid" << endl;
#endif
	return ((mCurrentFinger == 0 && mTimer == 0) || (mCurrentFinger != 0 && mCurrentFinger->IsCompleted()));
}

bool
FingerContainer::Attach(OneTimeObserver* inTarget)
{
	OneTimeSubject::Attach(inTarget);
	
	if (IsValid()) {
		if (IsObsolete()) {
			goto ON_RELOAD;
		} else {
			goto ON_IMMEDIATE;
		}
	} else if (mTimer != 0) {
		goto ON_WAIT;
	} else if (mCurrentFinger != 0) {
		goto ON_WAIT;
	} else {
		goto ON_IMMEDIATE;
	}
 ON_RELOAD:
	ReloadFinger();
	return true;
 ON_IMMEDIATE:
	NotifyAll(0);
	return true;
 ON_WAIT:
	return true;
}

void
FingerContainer::Update(Subject* inFrom, void*)
{
	assert(inFrom == mCurrentFinger);
	if (mTimer != 0) {
		DELETE(mTimer);
		mTimer = 0;
	}
	mAliveUntil = mFactory->GetAliveUntil();
	NotifyAll(0);
}

bool
FingerContainer::ReloadFinger()
{
	bool aResult;
	int aLoad = (int)mFactory->GetThreadMgr()->GetLoad();
	
	if (aLoad > enter_load) {
		if (mTimer == 0) {
			(mTimer = NEW(Timer, (this, FingerContainer::OnTimer)))
				->Init(mFactory->GetThreadMgr(), aLoad * default_wait_secs / enter_load);
		} else {
			if (mTimer->IsActive()) {
				mTimer->Sleep(aLoad * default_wait_secs / enter_load);
			}
		}
		aResult = true;
	} else {
		if (mCurrentFinger != 0) {
			DELETE(mCurrentFinger);
			mCurrentFinger = 0;
		}
		
		mFactory->OnReload(this);
		
		{
			int aConnectSecs = (int)mFactory->GetPropertyValue("FINGER_CONNECT_SEC");
			if (aConnectSecs <= 0) {
				aConnectSecs = default_connect_secs;
			}
			mCurrentFinger = NEW(CoopFinger, (mFactory->GetThreadMgr(), mHostaddr, aConnectSecs));
			mCurrentFinger->Attach(this);
		}
		aResult = false;
	}
	return aResult;
}

bool
FingerContainer::OnTimer(Timer* ioTimer)
{
	assert(ioTimer == mTimer);
	return ReloadFinger();
}

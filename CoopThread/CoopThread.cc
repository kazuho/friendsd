/**
 * CoopThread.cc
 */

#include "CoopThread.h"
#include "CoopThreadMgr.h"

//#define DEBUG_MSG

CoopThread::CoopThread(StateModifierFunc inStateModifierFunc)
: mMgr(0), mIsActive(true), mStateModifierFunc(inStateModifierFunc)
{
#ifdef DEBUG_MSG
	cout << "CoopThread::CoopThread(" << (void*)inStateModifierFunc << ')' << endl;
#endif
}

CoopThread::~CoopThread()
{
	if (mMgr != 0) {
		mMgr->RemoveThread(this);
	}
#ifdef DEBUG_MSG
	cout << "CoopThread::~CoopThread" << endl;
#endif
}

CoopThread::MyType*
CoopThread::Init(CoopThreadMgr* inMgr)
{
	mMgr = inMgr;
	if (mMgr != 0) {
		mMgr->AddThread(this);
		mIsActive = true;
	}
	return this;
}

bool
CoopThread::Activate()
{
	bool aResult;
	
	if (mMgr != 0) {
		mIsActive = true;
		aResult = true;
	} else {
		aResult = false;
	}
	return aResult;
}

bool
CoopThread::Deactivate()
{
	mIsActive = false;
	return true;
}

bool
CoopThread::IsAlive() const
{
	return (mMgr != 0) ? mMgr->IsAliveThread(this) : false;
}


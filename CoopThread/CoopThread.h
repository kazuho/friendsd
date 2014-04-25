/**
 * CoopThread.h
 */

#ifndef COOPTHREAD_H
#define COOPTHREAD_H

#include "CoopThreadMgr.h"

class CoopThread {
public:
	typedef CoopThread MyType;
	typedef CoopThreadMgr::ThreadSet ThreadSet;
	typedef CoopThreadMgr::StateModifierFunc StateModifierFunc;
protected:
	CoopThreadMgr* mMgr;
	bool mIsActive;
	StateModifierFunc mStateModifierFunc;
public:
	// constructs a new thread
	CoopThread(StateModifierFunc inStateModifierFunc = 0);
	// destructs a thread
	virtual ~CoopThread();
	// calculation.  return false when a thread can be destructed.
	virtual bool Run() = 0;
	virtual bool Activate();
	virtual bool Deactivate();
	bool IsActive() const { return mIsActive; }
	bool IsAlive() const;
	CoopThreadMgr* GetThreadMgr() { return mMgr; }
	StateModifierFunc GetStateModifierFunc() const { return mStateModifierFunc; }
protected:
	MyType* Init(CoopThreadMgr* inMgr);
private:
	CoopThread(const MyType&);
	MyType& operator=(const MyType&);
public:
	struct ptr_hashcode {
		int operator()(const CoopThread*& x) const { return ((int)(const void*)x) / sizeof(CoopThread); }
	};
};

#endif

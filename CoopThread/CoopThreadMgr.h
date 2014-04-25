/**
 * CoopThreadMgr.h
 */

#ifndef COOPTHREADMGR_H
#define COOPTHREADMGR_H

#include "hashset.h"
#include "hashmap.h"

#include "AllocDebug.h"

class CoopThreadMgr {
friend class CoopThread;
public:
	typedef CoopThreadMgr MyType;
	typedef hashset<CoopThread*, ptr_hashcode<CoopThread*> > ThreadSet;
	// returns minimul waitMillitm for inThreadPts, -1 if is infinite
	typedef int (*StateModifierFunc)(ThreadSet& inThreadPts,
																	 int inMaxWaitMillis);
private:
	typedef hashmap<StateModifierFunc, ThreadSet, ptr_hashcode<StateModifierFunc> > StateModifierMap;
protected:
	ThreadSet mThreads;
	CoopThread *mCurrentThread;
	StateModifierMap mStateModifierMap;
	bool mReiterateRun;
public:
	CoopThreadMgr();
	virtual ~CoopThreadMgr();
	// returns false if no thread exists
	bool Run();
	double GetLoad() const;
	int GetActiveThreadCount() const;
	int GetSleepingThreadCount() const;
protected:
private:
	CoopThreadMgr(const MyType&);
	MyType& operator=(const MyType&);
	void CheckActiveThreads();
	// these functions may only be called from CoopThread
	void AddThread(CoopThread*);
	void RemoveThread(CoopThread*);
	bool IsAliveThread(const CoopThread*) const;
public:
	static int sMaxWaitMillis;
};

#endif

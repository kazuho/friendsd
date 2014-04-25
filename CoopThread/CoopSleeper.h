/**
 * CoopSleeper.h
 */

#ifndef COOPSLEEPER_H
#define COOPSLEEPER_H

#include "CoopThread.h"

class CoopSleeper : public CoopThread
{
public:
	typedef CoopSleeper MyType;
	typedef CoopThread super;
protected:
	int mWakeUpSec; // time(2) format, -1 when is not used
public:
	CoopSleeper();
	virtual ~CoopSleeper();
	MyType* Init(CoopThreadMgr* inMgr, int inWakeUpSec = -1);
	virtual bool Activate();
	virtual bool Deactivate();
	virtual bool Sleep(int inSec);
	virtual bool IsSleeping() { return (mWakeUpSec != -1); }
private:
	static int StateModifier(ThreadSet& inThreads, int inMaxWaitMillis);
};

#endif

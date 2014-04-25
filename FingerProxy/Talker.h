/**
 * Talker.h
 */

#ifndef TALKER_H
#define TALKER_H

#include "CoopSleeper.h"
#include "CoopRunnable.h"

class FPFactory;
class Talker {
public:
	typedef Talker MyType;
	typedef CoopRunnable<CoopSleeper, MyType> Sleeper;
protected:
	FPFactory* const mFactory;
	Sleeper* mSleeper;
public:
	Talker(FPFactory* inFactory);
	virtual ~Talker();
	virtual bool IsCompleted() const = 0;
	FPFactory* GetFactory() const;
protected:
	bool StartTimeout();
	bool StopTimeout();
	virtual void OnTimeout() {}
private:
	bool OnWakeup(Sleeper* ioSleeper);
};

#endif

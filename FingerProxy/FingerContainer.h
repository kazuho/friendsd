/**
 * FingerContainer.h
 */

#ifndef FINGERCONTAINER_H
#define FINGERCONTAINER_H

#include "CoopFinger.h"
#include "Observer.h"
#include "OneTimeSubject.h"
#include "FPFactory.h"
#include "sstring.h"

class FingerContainer : public Observer, public OneTimeSubject {
public:
	typedef FingerContainer MyType;
	// no "super" class is defined since Observer & OneTimeSubject is a mix-in class
	typedef CoopRunnable<CoopSleeper, MyType> Timer;
	enum { default_wait_secs = 3, default_connect_secs = 5, enter_load = 30 };
protected:
	FPFactory* const mFactory;
	CoopFinger* mCurrentFinger;
	Timer* mTimer;
	sstring mHostname;
	unsigned long mHostaddr;
	int mAliveUntil;
public:
	FingerContainer(FPFactory* inFactory, const sstring& inHostname);
	virtual ~FingerContainer();
	const sstring& GetHostname() const;
	const Buffer& GetBuffer() const;
	bool IsError() const;
	int GetAliveUntil() const;
	bool IsObsolete() const;
	bool IsValid() const;
	virtual bool Attach(OneTimeObserver* inTarget);
protected:
	virtual void Update(Subject* inFrom, void*);
private:
	bool ReloadFinger();
	bool OnTimer(Timer* ioTimer);
public:
	static int sValidSec;
};

#endif

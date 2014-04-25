/**
 * OneTimeSubject.h
 */

#ifndef ONETIMESUBJECT_H
#define ONETIMESUBJECT_H

#include <list.h>

class OneTimeObserver;
class OneTimeSubject {
public:
	typedef list<OneTimeObserver*> ObserverList;
protected:
	ObserverList mObserverList;
public:
	OneTimeSubject() : mObserverList() {}
	virtual ~OneTimeSubject() {}
	virtual bool Attach(OneTimeObserver *inTarget);
	virtual bool Detach(OneTimeObserver *inTarget);
	bool IsAttached(OneTimeObserver* inTarget) const;
protected:
	void NotifyAll(void* inArg);
};

#endif

/**
 * Subject.h
 */

#ifndef SUBJECT_H
#define SUBJECT_H

#include <list.h>

class Observer;
class Subject {
public:
	typedef list<Observer*> ObserverList;
protected:
	ObserverList mObserverList;
public:
	Subject() : mObserverList() {}
	virtual ~Subject() {}
	virtual bool Attach(Observer* inTarget);
	virtual bool Detach(Observer* inTarget);
	bool IsAttached(Observer* inTarget) const;
protected:
	void NotifyAll(void* inArg);
};

#endif

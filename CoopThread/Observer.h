/**
 * Observer.h
 */

#ifndef OBSERVER_H
#define OBSERVER_H

class Observer {
friend class Subject;
protected:
	virtual void Update(Subject* mFrom, void* inArgs) = 0;
};

#endif

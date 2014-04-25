/**
 * Subject.cc
 */

#include <algo.h>
#include "Subject.h"
#include "Observer.h"

// #define DENY_MULTIPLE_OBSERVATION

#define FIND(c, i) (find(c.begin(), c.end(), i))

bool
Subject::Attach(Observer *inTarget)
{
#ifdef DENY_MULTIPLE_OBSERVATION
	if (FIND(mObserverList, inTarget) == mObserverList.end()) {
		mObserverList.push_back(inTarget);
		return true;
	} else {
		return false;
	}
#else
	mObserverList.push_back(inTarget);
	return true;
#endif
}

bool
Subject::Detach(Observer *inTarget)
{
	ObserverList::iterator aIt = FIND(mObserverList, inTarget);
	if (aIt != mObserverList.end()) {
		mObserverList.erase(aIt);
		return true;
	} else {
		return false;
	}
}

bool
Subject::IsAttached(Observer* inTarget) const
{
	return (FIND(mObserverList, inTarget) != mObserverList.end());
}

void
Subject::NotifyAll(void* inArg)
{
	for (ObserverList::iterator aIt = mObserverList.begin();
			 aIt != mObserverList.end();
			 ++aIt) {
		(*aIt)->Update(this, inArg);
	}
}

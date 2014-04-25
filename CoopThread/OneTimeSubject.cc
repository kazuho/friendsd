/**
 * OneTimeSubject.cc
 */

#include <algo.h>
#include "OneTimeSubject.h"
#include "OneTimeObserver.h"

// #define DENY_MULTIPLE_OBSERVATION

#define FIND(c, i) (find(c.begin(), c.end(), i))

bool
OneTimeSubject::Attach(OneTimeObserver *inTarget)
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
OneTimeSubject::Detach(OneTimeObserver *inTarget)
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
OneTimeSubject::IsAttached(OneTimeObserver* inTarget) const
{
	return (FIND(mObserverList, inTarget) != mObserverList.end());
}

void
OneTimeSubject::NotifyAll(void* inArg)
{
	for (ObserverList::iterator aIt = mObserverList.begin();
			 aIt != mObserverList.end();
			 ++aIt) {
		(*aIt)->Update(this, inArg);
	}
	mObserverList.erase(mObserverList.begin(), mObserverList.end());
}

/**
 * CoopThreadMgr.cc
 */

extern "C" {
#include <assert.h>
#include <unistd.h>
}
#include <algo.h>
#include "CoopThreadMgr.h"
#include "CoopThread.h"

//#define DEBUG_MSG

CoopThreadMgr::CoopThreadMgr()
: mThreads(), mCurrentThread(0), mStateModifierMap()
{
	mReiterateRun = false;
}

CoopThreadMgr::~CoopThreadMgr()
{
}

bool
CoopThreadMgr::Run()
{
	CheckActiveThreads();
	
#ifdef DEBUG_MSG
	cout << "CoopThreadMgr::Run(" << GetActiveThreadCount() << ':'
		<< GetSleepingThreadCount() << ')' << endl;
#endif
	// iterate CoopThread::Run()
	for (ThreadSet::iterator aIt = mThreads.begin(); aIt != mThreads.end(); ) {
		CoopThread* aCurrentThread = *aIt;
		if (aCurrentThread->IsActive()) {
			mCurrentThread = aCurrentThread;
			if (aCurrentThread->Run()) {
				mCurrentThread = 0;
				++aIt;
			} else {
				mCurrentThread = 0;
				RemoveThread(aCurrentThread);
			}
		} else {
			++aIt;
		}
		if (mReiterateRun) {
			mReiterateRun = false;
			break;
		}
	}
	mCurrentThread = 0;
	
#ifdef DEBUG_MSG
	cout << endl;
#endif  
	
	return (mThreads.size() != 0);
}

double
CoopThreadMgr::GetLoad() const
{
	double aResult = 0;
	for (ThreadSet::const_iterator aIt = mThreads.begin();
			 aIt != mThreads.end();
			 ++aIt) {
		if ((*aIt)->IsActive()) {
			aResult += 1.0;
		} else {
			aResult += 0.1;
		}
	}
	return aResult;
}

int
CoopThreadMgr::GetActiveThreadCount() const
{
	int aResult = 0;
	for (ThreadSet::const_iterator aIt = mThreads.begin();
			 aIt != mThreads.end();
			 ++aIt) {
		if ((*aIt)->IsActive()) {
			aResult++;
		}
	}
	return aResult;
}

int
CoopThreadMgr::GetSleepingThreadCount() const
{
	int aResult = 0;
	for (ThreadSet::const_iterator aIt = mThreads.begin();
			 aIt != mThreads.end();
			 ++aIt) {
		if (! (*aIt)->IsActive()) {
			aResult++;
		}
	}
	return aResult;
}

void
CoopThreadMgr::CheckActiveThreads()
{
#ifdef DEBUG_MSG
	cout << "CheckActiveThreads(" << mStateModifierMap.size()
		<< ':' << GetActiveThreadCount() << ':' << GetSleepingThreadCount()
			<< ')' << flush;
#endif
	
	switch (mStateModifierMap.size()) {
	case 0:
		break;
	case 1:
		{
			StateModifierMap::iterator aModifierIt = mStateModifierMap.begin();
			
			int aResult = (*(*aModifierIt).first)((*aModifierIt).second,
																			(GetActiveThreadCount() != 0) ? 0 : sMaxWaitMillis);
			if (aResult == -1) {
				::usleep(sMaxWaitMillis * 1000);
			} else {
				::usleep(aResult * 1000);
			}
		}
		break;
	default:
		{
			StateModifierMap::iterator aShortestIt;
			int aShortestResult = sMaxWaitMillis,
			aSecondShortestResult = sMaxWaitMillis;
			
			for (StateModifierMap::iterator aModifierIt = mStateModifierMap.begin();
					 aModifierIt != mStateModifierMap.end();
					 ++aModifierIt) {
#ifdef DEBUG_MSG
				cout << '(' << (void*)(*aModifierIt).first << flush;
#endif
				int aResult = (*(*aModifierIt).first)((*aModifierIt).second, 0);
#ifdef DEBUG_MSG
				cout << ')' << flush;
#endif
				if (aResult == -1) {
					aResult = sMaxWaitMillis;
				} else if (aResult < aShortestResult) {
					aSecondShortestResult = aShortestResult;
					aShortestResult = aResult;
					aShortestIt = aModifierIt;
				} else if (aResult < aSecondShortestResult) {
					aSecondShortestResult = aResult;
				}
			}
			
#ifdef DEBUG_MSG
			cout << GetActiveThreadCount() << flush;
#endif
			if (GetActiveThreadCount() == 0) {
				(*(*aShortestIt).first)((*aShortestIt).second, aSecondShortestResult);
				
				for (StateModifierMap::iterator aModifierIt = mStateModifierMap.begin();
						 aModifierIt != mStateModifierMap.end();
						 ++aModifierIt) {
					if (aShortestIt != aModifierIt) {
						(*(*aModifierIt).first)((*aModifierIt).second, 0);
					}
				}
			}
		}
		break;
	}	
	
#ifdef DEBUG_MSG
	cout << endl;
#endif
}

void
CoopThreadMgr::AddThread(CoopThread* inThread)
{
#ifdef DEBUG_MSG
	cout << "AddThread(" << inThread << ')' << endl;
#endif
	assert(mThreads.find(inThread) == mThreads.end());
	
	mThreads.insert(inThread);
	StateModifierFunc aModifierFunc = inThread->GetStateModifierFunc();
	if (aModifierFunc != 0) {
		StateModifierMap::iterator aModifierFuncIt = mStateModifierMap.find(aModifierFunc);
		if (aModifierFuncIt == mStateModifierMap.end()) {
			aModifierFuncIt = mStateModifierMap.insert(StateModifierMap::value_type(aModifierFunc, ThreadSet()));
		}
		(*aModifierFuncIt).second.insert(inThread);
	}
	mReiterateRun = true;
}

void
CoopThreadMgr::RemoveThread(CoopThread* inThread)
{
#ifdef DEBUG_MSG
	cout << "CoopThreadMgr::RemoveThread(" << inThread << ") " << flush;
#endif
	assert(mCurrentThread != inThread);
	
	ThreadSet::iterator aIt;
	if ((aIt = mThreads.find(inThread)) != mThreads.end()) {
#ifdef DEBUG_MSG
		cout << "-thread " << flush;
#endif
		mThreads.erase(aIt);
		StateModifierFunc aModifier = inThread->GetStateModifierFunc();
		if (aModifier != 0) {
			StateModifierMap::iterator aModifierIt = mStateModifierMap.find(aModifier);
			assert(aModifierIt != mStateModifierMap.end());
			ThreadSet& aThreadSet = (*aModifierIt).second;
			aIt = aThreadSet.find(inThread);
			assert(aIt != aThreadSet.end());
#ifdef DEBUG_MSG
			cout << "-funcptr.cnt " << flush;
#endif
			aThreadSet.erase(aIt);
			if (aThreadSet.size() == 0) {
#ifdef DEBUG_MSG
				cout << "-funcptr.entry " << flush;
#endif
				mStateModifierMap.erase(aModifierIt);
			}
		} 
	}
	
	mReiterateRun = true;
#ifdef DEBUG_MSG
	cout << endl;
#endif
}

bool
CoopThreadMgr::IsAliveThread(const CoopThread* inThread) const
{
	return (mThreads.find((CoopThread* const)inThread) != mThreads.end());
}

int CoopThreadMgr::sMaxWaitMillis = 60000;

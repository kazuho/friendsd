/**
 * CoopSleeper.h
 */

#include "CoopSleeper.h"
extern "C" {
#include <assert.h>
#include <time.h>
#include <unistd.h>
}

//#define DEBUG_MSG

CoopSleeper::CoopSleeper()
: super(StateModifier), mWakeUpSec(-1)
{
}

CoopSleeper::~CoopSleeper()
{
}

CoopSleeper::MyType*
CoopSleeper::Init(CoopThreadMgr* inMgr, int inWakeUpSec)
{
	super::Init(inMgr);
	if (inWakeUpSec != -1) {
		Sleep(inWakeUpSec);
	}
	return this;
}

bool
CoopSleeper::Activate()
{
	bool aResult = super::Activate();
	
	if (aResult) {
		mWakeUpSec = -1;
	}
	return aResult;
}

bool
CoopSleeper::Deactivate()
{
	bool aResult = super::Deactivate();
	
	if (aResult) {
		mWakeUpSec = -1;
	}
	return aResult;
}

bool
CoopSleeper::Sleep(int inSec)
{
	bool aResult = super::Deactivate();
	if (aResult) {
		mWakeUpSec = ::time(NULL) + inSec;
	}
	return aResult;
}

int
CoopSleeper::StateModifier(ThreadSet& inThreads, int inMaxWaitMillis)
{
#ifdef DEBUG_MSG
	cout << "CoopSleeper::StateModifier(" << inThreads.size() << ',' << inMaxWaitMillis << "): " << flush;
#endif
	int aMinSleepSec = 0x01000000;
	int aCurrentTime;
	
	aCurrentTime = ::time(NULL);
	for (ThreadSet::iterator aIt = inThreads.begin();
			 aIt != inThreads.end();
			 ++aIt) {
		int aWakeUpSec = ((CoopSleeper*)*aIt)->mWakeUpSec;
		if (aWakeUpSec == -1) {
			// nothing
		} else if (aWakeUpSec > aCurrentTime) {
			assert(! (*aIt)->IsActive());
			if (aWakeUpSec - aCurrentTime < aMinSleepSec) {
				aMinSleepSec = aWakeUpSec - aCurrentTime;
				assert(aMinSleepSec > 0);
			}
		} else {
			assert(! (*aIt)->IsActive());
			(*aIt)->Activate();
			aMinSleepSec = 0;
		}
	}
	
	int aSleepSec = min(aMinSleepSec, inMaxWaitMillis / 1000);
#ifdef DEBUG_MSG
	cout << "sleeping(" << aSleepSec << ')' << flush;
#endif
	
	int aResult;
	if (aSleepSec != 0) {
		::sleep(aSleepSec);
		
		aCurrentTime = ::time(NULL);
		for (ThreadSet::iterator aIt = inThreads.begin();
				 aIt != inThreads.end();
				 ++aIt) {
			int aWakeUpSec = ((CoopSleeper*)*aIt)->mWakeUpSec;
			if (aWakeUpSec == -1) {
				// nothing
			} else if (aWakeUpSec <= aCurrentTime) {
				(*aIt)->Activate();
			}
		}
		aResult = 0;
	} else {
		aResult = (aMinSleepSec * 1000 >= 0) ? aMinSleepSec * 1000 : -1;
	}

#ifdef DEBUG_MSG
	cout << " returning(" << aResult << ')' << endl;
#endif	
	return aResult;
}

#ifdef TEST

class SleepTest : public CoopSleeper {
	typedef SleepTest MyType;
	typedef CoopSleeper super;
public:
	SleepTest(CoopThreadMgr* inMgr) : super(inMgr) {}
	bool Run() { cout << "SleepTest::Run()" << endl; Sleep(1); return true; }
};

int
main(int, char**)
{
	CoopThreadMgr* aMgr = NEW(CoopThreadMgr, ());
	SleepTest* aSleepTest = NEW(SleepTest, (aMgr));
	
	while (aMgr->Run())
		;
	
	DELETE(aSleepTest);
	DELETE(aMgr);
	
	return 0;
}

#endif

/**
 * Talker.cc
 */

#include "Talker.h"
#include "FPFactory.h"
#include "sstring.h"
#include <iostream.h>

//#define DEBUG_MSG

Talker::Talker(FPFactory* inFactory)
: mFactory(inFactory), mSleeper(0)
{
}

Talker::~Talker()
{
#ifdef DEBUG_MSG
	cout << "Talker::~Talker" << endl;
#endif
	if (mSleeper != 0) {
		DELETE(mSleeper);
	}
}

FPFactory*
Talker::GetFactory() const
{
	return mFactory;
}

bool
Talker::StartTimeout()
{
	int aSleepSec = (int)mFactory->GetPropertyValue(sstring("TIMEOUT_SEC"));
	
	if (aSleepSec > 0) { // is a valid sec.
	
		if (mSleeper == 0) {
			(mSleeper = NEW(Sleeper, (this, OnWakeup)))
					->Init(mFactory->GetThreadMgr(), (int)mFactory->GetPropertyValue(sstring("TIMEOUT_SEC")));
		} else {
			mSleeper->Sleep((int)mFactory->GetPropertyValue(sstring("TIMEOUT_SEC")));
		}
	}
	
	return aSleepSec > 0;
}

bool
Talker::StopTimeout()
{
	return mSleeper != 0 && mSleeper->Deactivate();
}

bool
Talker::OnWakeup(Sleeper* ioSleeper)
{
	OnTimeout(); // call hander
	ioSleeper->Deactivate(); // sleep until StartTimeout will be called
	return true;
}

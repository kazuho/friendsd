/**
 * OneTimeObserver.h
 */

#ifndef ONETIMEOBSERVER_H
#define ONETIMEOBSERVER_H

class OneTimeObserver {
friend class OneTimeSubject;
protected:
	virtual void Update(OneTimeSubject* mFrom, void* inArgs) = 0;
};

#endif

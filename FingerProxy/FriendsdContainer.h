/**
 * FriendsdContainer.h
 */

#ifndef FRIENDSDCONTAINER_H
#define FRIENDSDCONTAINER_H

extern "C" {
#include <assert.h>
}
#include <vector.h>

#include "OneTimeObserver.h"
#include "OneTimeSubject.h"

class FPFactory;
class sstring;
class FriendsdContainer : public OneTimeObserver, public OneTimeSubject {
public:
	typedef FriendsdContainer MyType;
	struct Server {
		char id[16], name[64];
		int access;
	};
	struct User {
		char login[9], name[21], tty[4], idle[5], when[11], where[13], server[16], dummy[5];
	};
	typedef vector<Server> ServerVect;
	typedef vector<User> UserVect;
protected:
	FPFactory* const mFactory;
	ServerVect mHostnames, mErrorServers;
	UserVect mUsers;
	int mLeftUpdates;
	int mLastModified; // time(2) format
	const int mValidSec;
public:
	FriendsdContainer(FPFactory* inFactory, const sstring& inHostnames, int inValidSec = 20);
	virtual ~FriendsdContainer();
	virtual bool Attach(OneTimeObserver* inTarget);
	const ServerVect& GetServers() const;
	const ServerVect& GetErrorServers() const;
	const UserVect& GetUsers() const;
	bool IsValid() const;
	int GetLastModified() const;
protected:
	virtual void Update(OneTimeSubject* inFrom, void*);
private:
	void RestructBuffer();
private:
	static Server CreateServer(const sstring& inHostname);
	static void AdjustUser(User& ioUser);
	static void ShrinkStr(char* const ioStr, int inMaxLength);
};

#endif

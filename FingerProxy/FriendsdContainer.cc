/**
 * FriendsdContainer.cc
 */

extern "C" {
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
}

#include "FriendsdContainer.h"
#include "FPFactory.h"
#include "FingerContainer.h"
#include "sstring.h"

//#define DEBUG_MSG

FriendsdContainer::FriendsdContainer(FPFactory* inFactory, const sstring& inHostnames, int inValidSec)
: mFactory(inFactory), mLastModified(0), mValidSec(inValidSec)
{
	// parse inHostnames and initialize mHostnames
	struct {
		sstring::size_type index;
		bool isValid;
	} aHostnameHead;
	aHostnameHead.isValid = false;
	for (sstring::size_type aIndex = 0; inHostnames[aIndex] != '\0'; aIndex++) {
		if (! isspace(inHostnames[aIndex])) {
			if (! aHostnameHead.isValid) {
				aHostnameHead.index = aIndex;
				aHostnameHead.isValid = true;
			}
		} else { // isspace
			if (aHostnameHead.isValid) { // a new entry
				mHostnames.push_back(CreateServer(sstring(inHostnames.begin() + aHostnameHead.index,
						inHostnames.begin() + aIndex)));
				aHostnameHead.isValid = false;
			}
		}
	}
	if (aHostnameHead.isValid) {
		mHostnames.push_back(CreateServer(inHostnames.begin() + aHostnameHead.index));
	}
	
	mLeftUpdates = 0;
}

FriendsdContainer::~FriendsdContainer()
{
}

bool
FriendsdContainer::Attach(OneTimeObserver* inTarget)
{
#ifdef DEBUG_MSG
	cout << "FriendsdContainer::Attach(" << inTarget << ")" << endl;
#endif
	bool aResult = OneTimeSubject::Attach(inTarget);
	
	if (aResult) {
		if (IsValid()) {
			NotifyAll(0);
		} else {
			if (mLeftUpdates != 0) {
				// do nothing
			} else {
				RestructBuffer();
			}
		}
	}
	return aResult;
}

const FriendsdContainer::ServerVect&
FriendsdContainer::GetServers() const
{
#ifdef DEBUG_MSG
	cout << "FriendsdContainer::GetServers: " << mHostnames.size() << endl;
#endif
	return mHostnames;
}

const FriendsdContainer::ServerVect&
FriendsdContainer::GetErrorServers() const
{
#ifdef DEBUG_MSG
	cout << "FriendsdContainer::GetErrorServers: " << mErrorServers.size() << endl;
#endif
	return mErrorServers;
}

const FriendsdContainer::UserVect&
FriendsdContainer::GetUsers() const
{
#ifdef DEBUG_MSG
	cout << "FriendsdContainer::GetUsers: " << mUsers.size() << endl;
#endif
	return mUsers;
}

bool
FriendsdContainer::IsValid() const
{
	return (mLastModified + mValidSec >= ::time(NULL)
					&& (mLeftUpdates == 0));
}

int
FriendsdContainer::GetLastModified() const
{
#ifdef DEBUG_MSG
	cout << "FriendsdContainer::GetLastModified() const: "
		<<mLastModified << endl;
#endif
	return mLastModified;
}

void
FriendsdContainer::RestructBuffer()
{
#ifdef DEBUG_MSG
	cout << "FriendsdContainer::RequestBuffer: " << flush;
#endif
	// clear buffers
	mErrorServers.erase(mErrorServers.begin(), mErrorServers.end());
	mUsers.erase(mUsers.begin(), mUsers.end());
	
	// request
	mLeftUpdates += mHostnames.size();
	for (ServerVect::const_iterator aIt = mHostnames.begin();
			 aIt != mHostnames.end();
			 ++aIt) {
		mFactory->GetFingerContainer((*aIt).name)->Attach(this);
	}
#ifdef DEBUG_MSG
	cout << mLeftUpdates << endl;
#endif
}

void
FriendsdContainer::Update(OneTimeSubject* inFrom, void*)
{
#define HEADSTR "Login       Name              TTY Idle    When    Where"

#ifdef DEBUG_MSG
	cout << "FriendsdContainer::Update: " << flush;
#endif
	FingerContainer* aContainer = (FingerContainer*)inFrom;
	
	if (! aContainer->IsError()) { // succeed
		const Buffer& aBuffer = aContainer->GetBuffer();
		const char* aBufferIt = aBuffer.Begin();
		
		if (aBufferIt == aBuffer.End()) {
			goto ON_BUFEND;
		}
		while (::strncmp(aBufferIt, HEADSTR, sizeof(HEADSTR) - 1) != 0) {
			while (*aBufferIt != '\n' && *aBufferIt != '\r') {
				if (++aBufferIt == aBuffer.End()) {
					goto ON_BUFEND;
				}
			}
			do {
				if (++aBufferIt == aBuffer.End()) {
					goto ON_BUFEND;
				}
			} while (*aBufferIt  == '\n' || *aBufferIt == '\r');
		}
		// user exists
		while (1) { // add each line to mUsers
			User aNewUser;
			while (*aBufferIt != '\n' && *aBufferIt != '\r') {
				if (++aBufferIt == aBuffer.End()) {
					goto ON_BUFEND;
				}
			}
			do {
				if (++aBufferIt == aBuffer.End()) {
					goto ON_BUFEND;
				}
			} while (*aBufferIt  == '\n' || *aBufferIt == '\r');
			::memcpy((void*)&aNewUser, aBufferIt, sizeof(aNewUser));
			::strncpy(aNewUser.server, aContainer->GetHostname(), sizeof(aNewUser.server) - 1);
			for (char* aPt = aNewUser.server; *aPt != '\0'; aPt++) {
				if (*aPt == '.') {
					*aPt = '\0';
					break;
				}
			}
			AdjustUser(aNewUser);
			mUsers.push_back(aNewUser);
		}
		// wont reach here
	} else { // error
		mErrorServers.push_back(CreateServer(aContainer->GetHostname()));
	}
	
 ON_BUFEND:
	if (--mLeftUpdates == 0) {
		mLastModified = ::time(NULL);
		NotifyAll(0);
	}
#ifdef DEBUG_MSG
	cout << mLeftUpdates << endl;
#endif

#undef HEAD_STR
}

FriendsdContainer::Server
FriendsdContainer::CreateServer(const sstring& inHostname)
{
	Server aServer;
	fill((char*)&aServer, (char*)&aServer + sizeof(aServer), '\0');
	
	// write aServer.id, the first domain
	for (sstring::size_type aIndex = 0;
			 aIndex < sizeof(aServer.id) - 1
			 && inHostname[aIndex] != '\0'
			 && inHostname[aIndex] != '.';
			 aIndex++) {
		aServer.id[aIndex] = inHostname[aIndex];
	}
	
	// write aServer.name field
	::strncpy(aServer.name, inHostname, sizeof(aServer.name) - 1);
	
	return aServer;
}

void
FriendsdContainer::AdjustUser(User& ioUser)
{
#define SHRINKSTR(x) (ShrinkStr(x, sizeof(x) - 1))
	
	for (size_t j = 0; j < sizeof(ioUser.where); j++) {
		if (ioUser.where[j] == '.') {
			ioUser.where[j] = '\0';
		}
	}
	for (size_t j = 0; j < sizeof(ioUser.server); j++) {
		if (ioUser.server[j] == '.') {
			ioUser.server[j] = '\0';
		}
	}
	
	SHRINKSTR(ioUser.login);
	SHRINKSTR(ioUser.name);
	SHRINKSTR(ioUser.tty);
	SHRINKSTR(ioUser.idle);
	SHRINKSTR(ioUser.when);
	SHRINKSTR(ioUser.where);
	SHRINKSTR(ioUser.server);
	
	fill((char*)ioUser.dummy, (char*)ioUser.dummy + sizeof(ioUser.dummy), '\0');
	
#undef SHRINKSTR
}

void
FriendsdContainer::ShrinkStr(char* const ioStr, int inMaxLen)
{
	int aStrzIndex;

	aStrzIndex = -1;
	for (int aIndex = 0; aIndex < inMaxLen; aIndex++) {
		if (ioStr[aIndex] == '\0') {
			break;
		}
		if (isalnum(ioStr[aIndex])
				|| ioStr[aIndex] == '-'
				|| (ioStr[aIndex] & 0x80) != 0) {
			aStrzIndex = aIndex;
		}
	}
	aStrzIndex++;
	for (int aIndex = aStrzIndex; aIndex < inMaxLen + 1; aIndex++) {
		ioStr[aIndex] = '\0';
	}
}

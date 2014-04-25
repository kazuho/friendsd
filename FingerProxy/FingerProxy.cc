/**
 * FingerProxy.cc
 */

#include "FingerContainer.h"
#include "FriendsdContainer.h"
#include "FPFactory.h"
#include "FPTalker.h"
#include "FriendsdTalker.h"
#include "Property.h"
#include "SockUtils.h"
#include "sstring.h"
extern "C" {
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
}

//#define DEBUG_MSG

class FingerProxy : public FPFactory {
public:
	typedef FingerProxy MyType;
	typedef FPFactory super;
	typedef CoopRunnable<CoopSocketListener, MyType> Listener;
	typedef hashmap<sstring, FingerContainer*, sstring::hashcode> ContainerMap;
	typedef list<Talker*> TalkerList;
protected:
	CoopThreadMgr* mMgr;
	ContainerMap mContainerMap;
	FriendsdContainer* mFriendsdContainer;
	Listener* mFPListener, * mFriendsdListener;
	TalkerList mTalkerList;
	PropertyMap mProps;
public:
	FingerProxy(const sstring& inPropFile);
	virtual ~FingerProxy();
	void Run();
	virtual CoopThreadMgr* GetThreadMgr() { return mMgr; }
	virtual FingerContainer* GetFingerContainer(const sstring& inHostname);
	virtual int GetAliveUntil();
	virtual void OnReload(FingerContainer* inContainer);
	virtual FriendsdContainer* GetFriendsdContainer() { return mFriendsdContainer; }
	virtual double GetPropertyValue(const sstring& inName) const;
	virtual const sstring& GetPropertyString(const sstring& inName) const;
private:
	bool AcceptFPTalker(Listener* ioListener);
	bool AcceptFriendsdTalker(Listener* ioListener);
	void GCContainers();
	void GCTalkers();
	const char* LogHeader() const;
};

FingerProxy::FingerProxy(const sstring& inPropFile)
: mContainerMap(),
	mTalkerList(),
	mProps()
{
	if (! mProps.Init(inPropFile)) {
		::exit(1);
	}
	mFriendsdContainer = mProps.IsDefined("FRIENDSD_RESTRUCT_SEC") ?
			NEW(FriendsdContainer, (this, mProps["FRIENDSD_SERVERS"], mProps["FRIENDSD_RESTRUCT_SEC"])) :
			NEW(FriendsdContainer, (this, mProps["FRIENDSD_SERVERS"]));
	
	mMgr = NEW(CoopThreadMgr, ());
	
	cout << LogHeader() << "Booting..." << endl;
	
	mFPListener = NEW(Listener, (this, AcceptFPTalker));
	mFPListener->Init(mMgr, mProps["FINGERPROXY_PORT"]);
	if (! mFPListener->IsValidSock()) {
		cout << LogHeader() << "Could not listen to port "
			<< ((int)mProps["FINGERPROXY_PORT"]) << endl;
		::exit(1);
	}
	cout << LogHeader() << "Now listening " << ((int)mProps["FINGERPROXY_PORT"])
		<< " for " << "FPTalker." << endl;
	
	mFriendsdListener = NEW(Listener, (this, AcceptFriendsdTalker));
	mFriendsdListener->Init(mMgr, mProps["FRIENDSD_PORT"]);
	if (! mFriendsdListener->IsValidSock()) {
		cout << LogHeader() << "Could not listen to port " 
			<< ((int)mProps["FRIENDSD_PORT"]) << endl;
		::exit(1);
	}
	cout << LogHeader() << "Now listening " << ((int)mProps["FRIENDSD_PORT"])
		<< " for " << "Friendsd." << endl;
}

FingerProxy::~FingerProxy()
{
	DELETE(mFriendsdContainer);
	DELETE(mMgr);
}

void
FingerProxy::Run()
{
	bool aContinueFlag, aListenerFlag = true;
	int aCount = 0;
	
	do {
		aContinueFlag = aContinueFlag = mMgr->Run();
		
		if (aCount % 16 == 0) {
			GCContainers();
		}
		if (aCount % 16 == 0 || (! aListenerFlag)) {
			GCTalkers();
		}
		
		if (aListenerFlag && mTalkerList.size() >= mProps["MAX_CONNECTION"]) {
			cout << LogHeader() << "Suspending Listeners." << endl;
			mFPListener->Suspend();
			mFriendsdListener->Suspend();
			aListenerFlag = false;
		} else if ((! aListenerFlag) && mTalkerList.size() < mProps["MAX_CONNECTION"]) {
			cout << LogHeader() << "Resuming Listeners." << endl;
			mFPListener->Unsuspend();
			mFriendsdListener->Unsuspend();
			aListenerFlag = true;
		}
		
		aCount++;
	} while (aContinueFlag);

#if 0
	cout << "<<<ALLOC>>>" << endl;
	ShowAllocs();
	cout << "<<< END >>>" << endl;
#endif
}

FingerContainer*
FingerProxy::GetFingerContainer(const sstring& inHostname)
{
#ifdef DEBUG_MSG
	cout << "FingerProxy::GetFingerContainer(" << inHostname << ")" << endl;
#endif
	
	ContainerMap::iterator aContainerIt = mContainerMap.find(inHostname);
	if (aContainerIt == mContainerMap.end()) {
#ifdef DEBUG_MSG
		cout << LogHeader() << "Constructing new entry for " << inHostname << endl;
#endif
		aContainerIt = mContainerMap.insert(ContainerMap::value_type(inHostname,
				NEW(FingerContainer, (this, inHostname))));
	}
	
	return (*aContainerIt).second;
}

int
FingerProxy::GetAliveUntil()
{
	int aOffset = ((int)mProps["PROXY_ALIVE_SEC"]) - (::random() & 15);
#ifdef DEBUG_MSG
	cout << "FingerProxy::GetAliveUntil() returning with offset " << aOffset << endl;
#endif
	return ::time(NULL) + aOffset;
}

void
FingerProxy::OnReload(FingerContainer* inContainer)
{
	cout << LogHeader() << "Loading finger data on "
		<< inContainer->GetHostname() << endl;
}

double
FingerProxy::GetPropertyValue(const sstring& inName) const
{
	return mProps[inName];
}

const sstring&
FingerProxy::GetPropertyString(const sstring& inName) const
{
	return mProps[inName];
}

bool
FingerProxy::AcceptFPTalker(Listener *ioListener)
{
	int aNewSock;
	sockaddr_in aSockaddr;
	
	if ((aNewSock = ioListener->Accept(aSockaddr)) >= 0) {
		FPTalker* aTalker;
		aTalker = NEW(FPTalker, (this, aNewSock));
		assert(aTalker != 0);
		cout << LogHeader() << "FPTalker connected from "
			<< SockUtils::Ip2Str(htonl(*(unsigned long*)&aSockaddr.sin_addr))
				<< " @ socket " << aNewSock << " (" << aTalker << ')' << endl;
		mTalkerList.push_back(aTalker);
	}
	return true;
}

bool
FingerProxy::AcceptFriendsdTalker(Listener *ioListener)
{
	int aNewSock;
	sockaddr_in aSockaddr;
	
	if ((aNewSock = ioListener->Accept(aSockaddr)) >= 0) {
		FriendsdTalker* aTalker;
		aTalker = NEW(FriendsdTalker, (this, aNewSock));
		assert(aTalker != 0);
		cout << LogHeader() << "FriendsdTalker connected from "
			<< SockUtils::Ip2Str(htonl(*(unsigned long*)&aSockaddr.sin_addr))
				<< " @ socket " << aNewSock << " (" << aTalker << ')' << endl;
		mTalkerList.push_back(aTalker);
	}
	return true;
}

void
FingerProxy::GCContainers()
{
	int aCount = 0;
	
	ContainerMap::iterator aIt = mContainerMap.begin();
	while (aIt != mContainerMap.end()) {
		if ((*aIt).second->IsObsolete()) {
			DELETE((*aIt).second);
			mContainerMap.erase(aIt++);
			aCount++;
		} else {
			++aIt;
		}
	}
	if (aCount != 0) {
		cout << LogHeader() << "Garbage collected " << aCount
			<< " entries of Container, left " << mContainerMap.size() << '.' << endl;
	}
}

void
FingerProxy::GCTalkers()
{
	int aCount = 0;
	
	TalkerList::iterator aIt = mTalkerList.begin();
	while (aIt != mTalkerList.end()) {
		if ((*aIt)->IsCompleted()) {
			DELETE(*aIt);
			mTalkerList.erase(aIt++);
			aCount++;
		} else {
			++aIt;
		}
	}
	if (aCount != 0) {
		cout << LogHeader() << "Garbage collected " << aCount
			<< " entries of Talker, left " << mTalkerList.size() << '.' << endl;
	}
}

const char*
FingerProxy::LogHeader() const
{
	static char sBuf[64];
	
	time_t aTime;
	::time(&aTime);
	::strftime(sBuf, sizeof(sBuf) - 1, "[%m/%d:%H:%M:%S]", ::localtime(&aTime));
#ifdef ALLOCDEBUG
	::sprintf(sBuf + ::strlen(sBuf), "(%3.1f:%4d) ", mMgr->GetLoad(), _AllocCount());
#else
	::sprintf(sBuf + ::strlen(sBuf), "(%3.1f) ", mMgr->GetLoad());
#endif

	return sBuf;
}

int main(int, char**);

int
main(int argc, char** argv)
{
	SockUtils::IgnoreSigPipe();
	FingerProxy(argc > 2 ? argv[1] : "FPServer.prop").Run();
	return 0;
}

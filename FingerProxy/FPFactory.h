/**
 * FPFactory.h
 */

#ifndef FPFACTORY_H
#define FPFACTORY_H

class CoopThreadMgr;
class FingerContainer;
class FriendsdContainer;
struct sockaddr_in;
class sstring;

class FPFactory {
public:
	// thread
	virtual CoopThreadMgr* GetThreadMgr() = 0;
	// FingerContainer
	virtual FingerContainer* GetFingerContainer(const sstring& inHostname) = 0;
	virtual int GetAliveUntil() = 0;
	virtual void OnReload(FingerContainer* inContainer) = 0;
	// FriendsdContainer
	virtual FriendsdContainer* GetFriendsdContainer() = 0;
	// Misc.
	virtual double GetPropertyValue(const sstring& inName) const = 0;
	virtual const sstring& GetPropertyString(const sstring& inName) const = 0;
};

#endif

/**
 * FriendsdTalker.h
 */

#ifndef FRIENDSDTALKER_H
#define FRIENDSDTALKER_H

#include "Talker.h"
#include "CoopSocket.h"
#include "CoopSleeper.h"
#include "CoopRunnable.h"
#include "OneTimeObserver.h"
#include "FPFactory.h"
#include "FriendsdContainer.h"
#include "Buffer.h"

class FriendsdTalker : public Talker, public OneTimeObserver {
public:
	typedef FriendsdTalker MyType;
	typedef Talker super;
	typedef CoopRunnable<CoopSocketReader, MyType> Reader;
	typedef CoopRunnable<CoopSocketWriter, MyType> Writer;
	typedef CoopRunnable<CoopSleeper, MyType> Sleeper;
	typedef FriendsdContainer::Server Server;
	typedef FriendsdContainer::User User;
	typedef FriendsdContainer::ServerVect ServerVect;
	typedef FriendsdContainer::UserVect UserVect;
	struct IOHeader {
		char header[4];
		operator char*() { return header; }
		operator const char*() const { return header; }
		int GetValue() const { return header[0] * 256 + header[1]; }
		void SetValue(unsigned short v) { header[0] = v >> 8, header[1] = v; }
		unsigned short GetOpcode() const { return GetValue(); }
		void SetOpcode(unsigned short v) { SetValue(v); }
		unsigned short GetLength() const { return header[2] * 256 + header[3]; }
		void SetLength(unsigned short l) { header[2] = l >> 8, header[3] = l; }
	};
	enum {
		req_wait = 1, req_all = 2, req_err = 4, req_sv = 8, req_ifnew = 16,
		close_connection = 65535,
		ack = 1, nak = 0
		};
	enum { eWakeupSec = 10 };
protected:
	CoopSocket* mCurrentThread, * mOldThread;
	Sleeper mSleeper;
	int mLMofContainer;
	unsigned short mCmd;
	bool mIsAttached;
	Buffer mReadBuffer, mWriteBuffer;
public:
	FriendsdTalker(FPFactory* inFactory, int inSock);
	virtual ~FriendsdTalker();
	virtual bool IsCompleted() const;
	virtual void OnTimeout();
private:
	bool OnReceiveUserID(Reader* ioReader);
	bool OnReceiveCmd(Reader* ioReader);
	bool Read(Reader* ioReader);
	bool OnWrite(Writer* ioWriter);
	virtual void Update(OneTimeSubject* inFrom, void*);
	bool OnWakeup(Sleeper* ioSleeper);
	void SetUsersToWrite(const UserVect& inUsers);
	void SetServersToWrite(const ServerVect& inServers);
};

#endif

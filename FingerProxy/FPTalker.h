/**
 * FPTalker.h
 */

#ifndef FPTALKER_H
#define FPTALKER_H

#include "Talker.h"
#include "CoopRunnable.h"
#include "CoopSocket.h"
#include "OneTimeObserver.h"
#include "FPFactory.h"
#include "Buffer.h"

class FPTalker : public Talker, public OneTimeObserver {
public:
	typedef FPTalker MyType;
	typedef Talker super;
	typedef CoopRunnable<CoopSocketReader, MyType> Reader;
	typedef CoopRunnable<CoopSocketWriter, MyType> Writer;
	typedef Buffer IOBuffer;
protected:
	CoopSocket* mCurrentThread, * mOldThread;
	IOBuffer mReadBuffer;
	IOBuffer mWriteBuffer;
	int mWriteBufferIndex;
	int mLeftRequests;
public:
	FPTalker(FPFactory* inFactory, int inSock);
	virtual ~FPTalker();
	virtual bool IsCompleted() const;
	bool ReadRequest(Reader*);
	bool Write(Writer*);
protected:
	virtual void OnTimeout();
	virtual void Update(OneTimeSubject* inFrom, void*);
private:
	int ParseRequest();
	void RequestFinger(const char* inHostname);
};

#endif

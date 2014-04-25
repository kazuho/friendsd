/**
 * CoopSocket.cc:
 * 		CoopSocket
 * 		CoopSocketConnector
 * 		CoopSocketListener
 * 		CoopSocketReader
 * 		CoopSocketWriter
 */

#if defined(USE_SELECT)
// do nothing
#elif defined(USE_POLL)
// do nothing
#else
#error Please define USE_SELECT or USE_POLL
#endif

extern "C" {
#include <assert.h>
#include <netdb.h>
#ifdef USE_POLL
#include <poll.h>
#endif
#include <string.h>
#ifdef USE_POLL
#include <stropts.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
}
#include "CoopSocket.h"
#include "SockUtils.h"
#ifdef USE_POLL
#include <vector.h>
#endif

#ifndef ERRNO_ON_BLOCK
#error define ERRNO_ON_BLOCK (typically EAGAIN or EWOULDBLOCK)
#endif

//#define DEBUG_MSG
//#define CONSERV_CONNECTION

#define RW_LOOPCOUNT (4)

CoopSocket::CoopSocket()
: super(StateModifier), mState(inactive), mSuspended(true)
{
}

CoopSocket::~CoopSocket()
{
}

CoopSocket::MyType*
CoopSocket::Init(CoopThreadMgr* inMgr, int inSock)
{
	super::Init(inMgr);
	mSock = inSock;
	mState = (inSock != -1) ? active : inactive;
	mSuspended = false;
	return this;
}

bool
CoopSocket::Close()
{
#ifdef DEBUG_MSG
	cout << "CoopSocket::Close(): " << mSock << endl;
#endif
	bool aResult;
	if (mSock != -1) {
		aResult = (close(mSock) == 0);
		mSock = -1;
	} else {
		aResult = true;
	}
	return aResult;
}

bool
CoopSocket::Activate()
{
	bool aResult = super::Activate();
	
	if (aResult) {
		mState = active;
	}
	return aResult;
}

bool
CoopSocket::Suspend()
{
	bool aResult = Deactivate();
	
	if (aResult) {
		mSuspended = true;
	}
	return aResult;
}

bool
CoopSocket::Unsuspend()
{
	bool aResult = Activate();
	
	if (aResult) {
		mSuspended = false;
	}
	return aResult;
}

#ifdef USE_POLL
typedef vector<pollfd> PollFdVect;
#endif

//#define DEBUG_MSG
int
CoopSocket::StateModifier(ThreadSet& inThreads, int inMaxWaitMillis)
{
#ifdef DEBUG_MSG
	cout << "CoopSocket::StateModifier(" << inThreads.size() << ',' << inMaxWaitMillis << "):" << flush;
#endif
	
#ifdef USE_SELECT
	fd_set aReadFds, aWriteFds, aExceptionFds;
	FD_ZERO(&aReadFds);
	FD_ZERO(&aWriteFds);
	FD_ZERO(&aExceptionFds);
#endif
#ifdef USE_POLL
	PollFdVect fds;
#endif
	
	int aMaxFd = -1,
	aMaxWaitMillis = inMaxWaitMillis,
	aResult = inMaxWaitMillis;
	
	for (ThreadSet::iterator aIt = inThreads.begin();
			 aIt != inThreads.end();
			 ++aIt) {
		MyType* aThread = (MyType*)*aIt;
		if (aThread->IsSuspended()) {
#ifdef DEBUG_MSG
			cout << 's' << flush;
#endif
			// do nothing
		} else {
			int aFd = aThread->GetSock();
			if (aFd >= 0) {
				switch (aThread->mState) { // should be inactive if write_block or read_block
				case write_block:
#ifdef DEBUG_MSG
					cout << 'w' << flush;
#endif
#ifdef USE_SELECT
					FD_SET(aFd, &aWriteFds);
#endif
#ifdef USE_POLL
					{
						pollfd aNewFd;
						aNewFd.fd = aFd, aNewFd.events = POLLOUT, aNewFd.revents = 0;
						fds.push_back(aNewFd);
					}
#endif
					if (aFd > aMaxFd) {
						aMaxFd = aFd;
					}
					break;
				case read_block:
#ifdef DEBUG_MSG
					cout << 'r' << flush;
#endif
#ifdef USE_SELECT
					FD_SET(aFd, &aReadFds);
#endif
#ifdef USE_POLL
					{
						pollfd aNewFd;
						aNewFd.fd = aFd, aNewFd.events = POLLIN, aNewFd.revents = 0;
						fds.push_back(aNewFd);
					}
#endif
					if (aFd > aMaxFd) {
						aMaxFd = aFd;
					}
					break;
				case inactive:
#ifdef DEBUG_MSG
					cout << 'i' << flush;
#endif
					break;
				default:
#ifdef DEBUG_MSG
					cout << 'd' << flush;
#endif
					aMaxWaitMillis = 0;
					break;
				}
			} else {
#ifdef DEBUG_MSG
				cout << 'x' << flush;
#endif
				aMaxWaitMillis = 0;
			}
			aResult = 0;
		}
	}
	
#ifdef DEBUG_MSG
	cout << " Polling(" << aMaxWaitMillis << ')';
#endif
	int aSelectResult;
#ifdef USE_SELECT
	{
		struct timeval aTimeval;
		aTimeval.tv_sec = aMaxWaitMillis / 1000;
		aTimeval.tv_usec = (aMaxWaitMillis != 0) ? (aMaxWaitMillis % 1000) * 1000 : 1;
		if (aMaxFd >= 0) {
			aSelectResult = ::select(aMaxFd + 1, &aReadFds, &aWriteFds, &aExceptionFds, &aTimeval);
		} else {
			aSelectResult = 0;
		}
	}
#endif
#ifdef USE_POLL
	aSelectResult = (aMaxFd >= 0) ? ::poll(fds.begin(), fds.size(), aMaxWaitMillis) : 0;
#endif
	
	switch (aSelectResult) {
	case -1: // error
		for (ThreadSet::iterator aIt = inThreads.begin();
				 aIt != inThreads.end();
				 ++aIt) {
			(*aIt)->Activate();
		}
		break;
	default: // succeed
#ifdef USE_SELECT
		for (ThreadSet::iterator aIt = inThreads.begin();
				 aIt != inThreads.end();
				 ++aIt) {
			switch (((MyType*)*aIt)->mState) {
			case write_block:
				if (FD_ISSET(((MyType*)*aIt)->mSock, &aWriteFds)) {
					(*aIt)->Activate();
				} else {
					(*aIt)->Deactivate();
				}
				break;
			case read_block:
				if (FD_ISSET(((MyType*)*aIt)->mSock, &aReadFds)) {
					(*aIt)->Activate();
				} else {
					(*aIt)->Deactivate();
				}
				break;
			default:
				break;
			}
		}
#endif
#ifdef USE_POLL
		for (ThreadSet::reverse_iterator aIt = inThreads.rbegin();
				aIt != inThreads.rend();
				++aIt) {
			MyType* aSocket = (MyType*)*aIt;
			if (aSocket->IsSuspended()) {
				continue;
			}
			switch (aSocket->mState) {
			case write_block:
				assert(fds.back().fd == aSocket->mSock);
#ifdef DEBUG_MSG
				cout << '(' << fds.back().fd << ':' << fds.back().revents << ':';
#endif
				if ((fds.back().revents & (POLLOUT | POLLERR | POLLHUP)) != 0) {
#ifdef DEBUG_MSG
					cout << "a)";
#endif
					aSocket->Activate();
				} else {
#ifdef DEBUG_MSG
					cout << "d)";
#endif
					aSocket->Deactivate();
				}
				fds.pop_back();
				break;
			case read_block:
				assert(fds.back().fd == aSocket->mSock);
#ifdef DEBUG_MSG
				cout << '(' << fds.back().fd << ':' << fds.back().revents << ':';
#endif
				assert(fds.back().fd == aSocket->mSock);
				if ((fds.back().revents & (POLLIN | POLLERR | POLLHUP)) != 0) {
#ifdef DEBUG_MSG
					cout << "a)";
#endif
					aSocket->Activate();
				} else {
#ifdef DEBUG_MSG
					cout << "d)";
#endif
					aSocket->Deactivate();
				}
				fds.pop_back();
				break;
			default:
				break;
			}
		}
#endif
		break;
	}
	
#ifdef DEBUG_MSG
	cout << " returning " << aResult << endl;
#endif
	return aResult;
}
//#undef DEBUG_MSG

// CoopSocketConnector

CoopSocketConnector::CoopSocketConnector()
: super(), mRawSock(-1), mTimeoutChecker(0)
{
}

CoopSocketConnector::~CoopSocketConnector()
{
	if (mTimeoutChecker != 0) {
		DELETE(mTimeoutChecker);
	}
}

// inAddr should be in machine byte order
CoopSocketConnector::MyType*
CoopSocketConnector::Init(CoopThreadMgr* inMgr, unsigned long inAddr, int inPort, int inTimeoutSec)
{
	super::Init(inMgr, -1);
	
	*(unsigned long*)&mSockaddr.sin_addr = htonl(inAddr);
	mSockaddr.sin_family = AF_INET;
	mSockaddr.sin_port = htons(inPort);
	
	mTimeoutSec = inTimeoutSec;
	return this;
}

CoopSocketConnector::MyType*
CoopSocketConnector::Init(CoopThreadMgr* inMgr, const sstring& inHost, int inPort, int inTimeoutSec)
{
	return Init(inMgr, SockUtils::GetHostByName(inHost), inPort, inTimeoutSec);
}

bool
CoopSocketConnector::Deactivate()
{
	bool aResult = super::Deactivate();
	
	if (aResult) {
		mState = write_block;
	}
	return aResult;
}

bool
CoopSocketConnector::IsConnecting()
{
#ifdef DEBUG_MSG
	cout << "CoopSocketConnector::IsConnecting()" << endl;
#endif
	if (mSock != -1 || *(unsigned long*)&mSockaddr.sin_addr == 0xffffffff) {
		goto ON_COMPLETE;
	} else {
		if (mRawSock == -1) {
			mRawSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (mRawSock == -1) {
				goto ON_ERROR;
			}
#ifndef CONSERV_CONNECTION
			if (! SockUtils::SetNonBlockingMode(mRawSock)) {
				close(mRawSock);
				mRawSock = -1;
				goto ON_ERROR;
			}
#endif
		} else if (mRawSock == -2) {
			goto ON_ERROR;
		}
		
		if (::connect(mRawSock, (sockaddr*)&mSockaddr, sizeof(sockaddr_in)) == 0) {
#ifdef CONSERV_CONNCTION
			SockUtils::SetNonBlockingMode(mRawSock);
#endif
			mSock = mRawSock;
			mRawSock = -1;
			Activate();
			goto ON_COMPLETE;
		} else if (errno == EINPROGRESS) {
			mSock = mRawSock;
			mRawSock = -1;
			mTimeoutChecker = NEW(TimeoutChecker, (this, OnTimeout))
				->Init(GetThreadMgr(), mTimeoutSec);
			Deactivate();
		} else {
			close(mRawSock);
			mRawSock = -1;
			goto ON_ERROR;
		}
	}
	return true;
 ON_COMPLETE:
	if (mTimeoutChecker != 0) {
		DELETE(mTimeoutChecker);
		mTimeoutChecker = 0;
	}
	return false;
 ON_ERROR:
	if (mTimeoutChecker != 0) {
		DELETE(mTimeoutChecker);
		mTimeoutChecker = 0;
	}
	return false;
}

bool
CoopSocketConnector::OnTimeout(TimeoutChecker*)
{
#ifdef DEBUG_MSG
	cout << "CoopSocketConnector::OnTimeout: " << mSock << ',' << mRawSock << endl;
#endif
	if (mSock != -1) {
		Close();
		assert(mSock == -1);
		mRawSock = -2;
	}
	Activate();
	return false;
}

// CoopSocketListener

CoopSocketListener::CoopSocketListener()
: super()
{
}

CoopSocketListener::~CoopSocketListener()
{
}

CoopSocketListener::MyType*
CoopSocketListener::Init(CoopThreadMgr* inMgr, int inPort)
{
	super::Init(inMgr, -1);
	
	sockaddr_in aSockaddr;
	
	fill((char*)&aSockaddr, ((char*)&aSockaddr) + sizeof(aSockaddr), '\0');
	aSockaddr.sin_port = htons(inPort);
	aSockaddr.sin_family = AF_INET;
	aSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	mSock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (mSock == -1) {
		goto ON_OPEN_ERROR;
	}
	if ((! SockUtils::SetNonBlockingMode(mSock)) ||
			(::bind(mSock, (sockaddr*)&aSockaddr, sizeof(sockaddr)) != 0) ||
			(::listen(mSock, 5) != 0)) {
		goto ON_ERROR;
	}
	Activate();
	return this;
 ON_ERROR:
	Close();
	return this;
 ON_OPEN_ERROR:
	return this;
}

int
CoopSocketListener::Accept()
{
	sockaddr_in aSockaddr;
	return Accept(aSockaddr);
}

int
CoopSocketListener::Accept(sockaddr_in &outSockaddr)
{
#ifdef DEBUG_MSG
	cout << "CoopSocketListener::Accept(sockaddr_in&)" << endl;
#endif
	int aAddrLength, aNewSock;
	
	aAddrLength = sizeof(sockaddr_in);
	aNewSock = ::accept(mSock, (sockaddr*)&outSockaddr, &aAddrLength);
	if (aNewSock == -1) {
		Deactivate();
		goto ON_ERROR;
	} else if (! SockUtils::SetNonBlockingMode(aNewSock)) {
		close(aNewSock);
		goto ON_ERROR;
	}
	return aNewSock;
 ON_ERROR:
	return -1;
}

bool
CoopSocketListener::Deactivate()
{
	bool aResult = super::Deactivate();
	
	if (aResult) {
		mState = read_block;
	}
	return aResult;
}

// CoopSocketReader

CoopSocketReader::CoopSocketReader()
: super()
{
}

CoopSocketReader::~CoopSocketReader()
{
}

CoopSocketReader::MyType*
CoopSocketReader::Init(CoopThreadMgr* inMgr, int inSock)
{
	super::Init(inMgr, inSock);
	return this;
}

bool
CoopSocketReader::Deactivate()
{
	bool aResult = super::Deactivate();
	
	if (aResult) {
		mState = read_block;
	}
	return aResult;
}

int
CoopSocketReader::Read(char* const outBuffer, int inMaxSize)
{
#ifdef DEBUG_MSG
	cout << "CoopSocketReader::Read(char*const,int)" << endl;
#endif
	int aResult;
	
	if (mSock == -1) {
		aResult = 0;
	} else {
		aResult = 0;
		int aReadSize = inMaxSize;
		for (int j = 0; j < RW_LOOPCOUNT && aReadSize != 0; j++) {
			int aReadResult = ::read(mSock, outBuffer + aResult, aReadSize);
			switch (aReadResult) {
			case -1:
				if (errno == ERRNO_ON_BLOCK) {
					Deactivate();
				} else {
					if (aResult == 0) {
						aResult = -1;
					}
				}
				goto ON_READ_END;
			case 0:
				if (aResult == 0) {
					aResult = -1;
				}
				goto ON_READ_END;
			default:
				aResult += aReadResult;
				aReadSize -= aReadResult;
				break;
			}
		}
	ON_READ_END:
		;
	}
#ifdef DEBUG_MSG
	cout << "CoopSocketReader<T>::Read returned " << aResult << endl;
#endif
	return aResult;
}

// CoopSocketWriter

CoopSocketWriter::CoopSocketWriter()
: super()
{
}

CoopSocketWriter::~CoopSocketWriter()
{
}

CoopSocketWriter::MyType*
CoopSocketWriter::Init(CoopThreadMgr* inMgr, int inSock)
{
	super::Init(inMgr, inSock);
	return this;
}

bool
CoopSocketWriter::Deactivate()
{
	bool aResult = super::Deactivate();
	
	if (aResult) {
		mState = write_block;
	}
	return mState;
}

int
CoopSocketWriter::Write(const char* inBuffer, int inMaxSize)
{
#ifdef DEBUG_MSG
	cout << "CoopSocketWriter::Write(const char*,int)" << endl;
#endif
	int aResult;
	
	if (mSock == -1) {
		aResult = 0;
	} else {
		aResult = 0;
		int aWriteSize = inMaxSize;
		for (int j = 0; j < RW_LOOPCOUNT && aWriteSize != 0; j++) {
			int aWriteResult = ::write(mSock, inBuffer + aResult, aWriteSize);
			switch (aWriteResult) {
			case -1:
				if (errno == ERRNO_ON_BLOCK) {
					Deactivate();
				} else {
					if (aResult == 0) {
						aResult = -1;
					}
				}
				goto ON_WRITE_END;
			case 0:
				if (aResult == 0) {
					aResult = -1;
				}
				goto ON_WRITE_END;
			default:
				aResult += aWriteResult;
				aWriteSize -= aWriteResult;
				break;
			}
		}
	ON_WRITE_END:
		;
	}
#ifdef DEBUG_MSG
	cout << "CoopSocketWriter<T>::Write returned " << aResult << endl;
#endif
	return aResult;
}

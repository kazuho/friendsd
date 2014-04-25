/**
 * Buffer.h
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

class Buffer {
public:
	typedef Buffer MyType;
	typedef size_t size_type;
	enum { block_size = 1024 };
protected:
	char *mBuffer;
	char *mBegin, *mEnd;
	size_type mSize;
public:
	Buffer();
	Buffer(const MyType&);
	virtual ~Buffer();
	MyType& operator=(const MyType&);
	const char* Begin() const { return mBegin; }
	char* Begin() { return mBegin; }
	const char* End() const { return mEnd; }
	char* End() { return mEnd; }
	size_type GetSize() const { return mEnd - mBegin; }
	void Clear() { mBegin = mBuffer; mEnd = mBuffer; *mEnd = '\0'; }
	char Pop() { return *mBegin++; }
	void Pop(size_type inSize) { mBegin += inSize; }
	void Push(const MyType& x);
	void Push(char x);
	void Push(const char* inBegin, const char* inEnd);
	void Push(const char* inPtr, int inSize);
	void PushStr(const char* inStr);
private:
	void EnsureCapacity(size_type inSize);
	void FreeGarbage();
};

#endif

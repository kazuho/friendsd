/**
 * Buffer.cc
 */

#include "Buffer.h"
#include <algobase.h>
extern "C" {
#include <assert.h>
#include <stdlib.h>
#include <string.h>
}

Buffer::Buffer()
: mSize(block_size)
{
	mBuffer = (char*)::malloc(mSize);
	assert(mBuffer != 0);
	mBegin = mBuffer;
	mEnd = mBuffer;
}

Buffer::Buffer(const MyType& x)
: mSize(block_size)
{
	mBuffer = (char*)malloc(block_size);
	assert(mBuffer != 0);
	mBegin = mBuffer;
	mEnd = mBuffer;
	Push(x.Begin(), x.GetSize());
}

Buffer::~Buffer()
{
	::free(mBuffer);
}

Buffer::MyType&
Buffer::operator=(const MyType& x)
{
	if (&x != this) {
		Clear();
		Push(x.Begin(), x.GetSize());
	}
	return *this;
}

void
Buffer::Push(const MyType& x)
{
	Push(x.Begin(), x.GetSize());
}

void
Buffer::Push(char x)
{
	EnsureCapacity(GetSize() + 1);
	*mEnd++ = x;
	*mEnd = '\0';
}

void
Buffer::Push(const char* inBegin, const char* inEnd)
{
	Push(inBegin, inEnd - inBegin);
}

void
Buffer::Push(const char* inPtr, int inSize)
{
	EnsureCapacity(GetSize() + inSize);
	copy(inPtr, inPtr + inSize, mEnd);
	mEnd += inSize;
	*mEnd = '\0';
}

void
Buffer::PushStr(const char* inStr)
{
	Push(inStr, ::strlen(inStr));
}

void
Buffer::EnsureCapacity(size_type inSize)
{
	if (inSize + (mBegin - mBuffer) >= mSize) {
		FreeGarbage();
	}
	if (inSize + (mBegin - mBuffer) >= mSize) {
		do {
			mSize *= 2;
		} while (inSize + (mBegin - mBuffer) >= mSize);
		char* aNewBuffer = (char*)::realloc(mBuffer, mSize);
		assert(aNewBuffer != 0);
		if (aNewBuffer != mBuffer) {
			ptrdiff_t aDiff = aNewBuffer - mBuffer;
			mBuffer += aDiff;
			mBegin += aDiff;
			mEnd += aDiff;
		}
	}
}

void
Buffer::FreeGarbage()
{
	int aGarbageSize = mBegin - mBuffer,
			aSize = mEnd - mBegin;
	if (aGarbageSize >= aSize) {
		copy(mBegin, mEnd, mBuffer);
		mBegin -= aGarbageSize;
		mEnd -= aGarbageSize;
		*mEnd = '\0';
	}
}

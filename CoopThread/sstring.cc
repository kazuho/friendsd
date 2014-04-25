/**
 * sstring.cc
 */

#include "sstring.h"
extern "C" {
#include <string.h>
}

sstring::sstring(int inBuflen)
: len(0), buflen(inBuflen)
{
	buf = new char [buflen];
	buf[0] = '\0';
}

sstring::sstring(const sstring& x)
: len(x.len), buflen(len + 1)
{
	buf = new char [buflen];
	copy(x.buf, x.buf + buflen, buf);
}

sstring::sstring(const char* inStr)
: len(::strlen(inStr)), buflen(len + 1)
{
	buf = new char [buflen];
	copy(inStr, inStr + buflen, buf);
}

sstring::sstring(size_type n, char x)
: len(n), buflen(len + 1)
{
	buf = new char [buflen];
	fill(buf, buf + len, x);
	buf[len] = '\0';
}

sstring::sstring(const_iterator begin, const_iterator end)
: len(end - begin), buflen(len + 1)
{
	buf = new char [buflen];
	copy(begin, end, buf);
	buf[len] = '\0';
}

sstring::~sstring()
{
	delete buf;
}

sstring&
sstring::operator=(const sstring& x)
{
	if (this != &x) {
		if (buflen >= x.len + 1) {
			len = x.len;
			copy(x.buf, x.buf + x.len + 1, buf);
		} else {
			delete buf;
			len = x.len;
			buflen = len + 1;
			buf = new char [buflen];
			copy(x.buf, x.buf + buflen, buf);
		}
	}
	return *this;
}

sstring&
sstring::operator+=(const sstring& x)
{
	ensure_size(len + x.len);
	copy(x.buf, x.buf + x.len + 1, buf + len);
	len += x.len;
	return *this;
}

sstring&
sstring::operator+=(const char* x)
{
	int xlen = ::strlen(x);
	ensure_size(len + xlen);
	copy(x, x + xlen + 1, buf + len);
	len += xlen;
	return *this;
}

bool
sstring::operator==(const sstring& x) const
{
	return ::strcmp(buf, x.buf) == 0;
}

bool
sstring::operator==(const char* x) const
{
	return ::strcmp(buf, x) == 0;
}

bool
sstring::operator<(const sstring& x) const
{
	return ::strcmp(buf, x.buf) < 0;
}

bool
sstring::operator<(const char* x) const
{
	return ::strcmp(buf, x) < 0;
}

bool
sstring::operator<=(const sstring& x) const
{
	return ::strcmp(buf, x.buf) <= 0;
}

bool
sstring::operator<=(const char* x) const
{
	return ::strcmp(buf, x) <= 0;
}

void
sstring::push_front(char x)
{
	ensure_size(len + 1);
	copy_backward(buf, buf + len + 1, buf + len + 2);
	buf[0] = x;
	len++;
}

void
sstring::push_back(char x)
{
	ensure_size(len + 1);
	buf[len++] = x;
	buf[len] = '\0';
}

void
sstring::pop_front()
{
	copy(buf + 1, buf + len + 1, buf);
	len--;
}

void
sstring::pop_back()
{
	buf[--len] = '\0';
}

sstring::iterator
sstring::insert(char* position, char x)
{
	ensure_size(len + 1);
	copy_backward(position, buf + len + 1, buf + len + 2);
	*position = x;
	len++;
	return position;
}

void
sstring::insert(char* position, size_type n, char x)
{
	if (n != 0) {
		ensure_size(len + n);
		copy_backward(position, buf + len + 1, buf + len + n + 1);
		while (n--)
			*position++ = x;
		len += n;
	}
}

void
sstring::insert(char* position, const char* begin, const char* end)
{
	if (begin != end) {
		ensure_size(len + end - begin);
		copy_backward(position, buf + len + 1, buf + len + (end - begin) + 1);
		copy(begin, end, position);
		len += end - begin;
	}
}

void
sstring::erase(char* position)
{
	copy(position + 1, buf + len + 1, position);
	len--;
}

void
sstring::erase(char* begin, char* end)
{
	copy(end, buf + len + 1, begin);
	len -= end - begin;
}

void
sstring::ensure_size(int inLen)
{
	if (buflen < inLen + 1) {
		int newbuflen = buflen;
		do {
			newbuflen = (newbuflen < 32) ? 64 : newbuflen * 2;
		} while (newbuflen < inLen + 1);
		char* newbuf = new char [newbuflen];
		copy(buf, buf + len + 1, newbuf);
		delete buf;
		buf = newbuf;
		buflen = newbuflen;
	}
}

void
sstring::swap(sstring& x)
{
	::swap(buf, x.buf);
	::swap(len, x.len);
	::swap(buflen, x.buflen);
}

int
sstring::hashcode::operator()(const sstring& x) const
{
	int r = 0, g;
	for (sstring::const_iterator p = x.begin(); p != x.end(); p++) {
		r = (r << 4) + *p;
		if ((g = r & 0xf0000000) != 0)
			r ^= g >> 24;
	}
	return r;
}

sstring
operator+(const sstring& x, const sstring& y)
{
	sstring s(x);
	s += y;
	return s;
}

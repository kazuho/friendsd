OBJS=		AllocDebug.o sstring.o Buffer.o \
		CoopThreadMgr.o CoopThread.o CoopSocket.o CoopSleeper.o \
		SockUtils.o Subject.o OneTimeSubject.o
CFLAGS=		-O2 -Wall -pg
CPPFLAGS=	-DUSE_SELECT -DERRNO_ON_BLOCK=EWOULDBLOCK -DNBLOCK_TAG=FNDELAY -DALLOCDEBUG
CCC=		g++
pCC=		gcc

objs:	$(OBJS)

.cc.o:
	$(CCC) $(CPPFLAGS) $(CFLAGS) -c $<

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS)
# DO NOT DELETE

AllocDebug.o: AllocDebug.h /usr/include/assert.h /usr/include/features.h
AllocDebug.o: /usr/include/sys/cdefs.h /usr/lib/g++-include/iostream.h
AllocDebug.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
AllocDebug.o: /usr/include/_G_config.h /usr/lib/g++-include/map.h
AllocDebug.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
AllocDebug.o: /usr/lib/g++-include/std/new.h
AllocDebug.o: /usr/lib/g++-include/std/cstddef.h
AllocDebug.o: /usr/lib/g++-include/std/stddef.h /usr/include/stdlib.h
AllocDebug.o: /usr/include/errno.h /usr/include/linux/errno.h
AllocDebug.o: /usr/include/asm/errno.h /usr/include/alloca.h
AllocDebug.o: /usr/include/linux/limits.h /usr/lib/g++-include/algobase.h
AllocDebug.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/bool.h
AllocDebug.o: /usr/lib/g++-include/iterator.h /usr/lib/g++-include/function.h
AllocDebug.o: /usr/lib/g++-include/tree.h /usr/lib/g++-include/projectn.h
Buffer.o: Buffer.h /usr/lib/g++-include/std/stddef.h /usr/include/_G_config.h
Buffer.o: /usr/lib/g++-include/std/cstddef.h /usr/lib/g++-include/algobase.h
Buffer.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/bool.h
Buffer.o: /usr/lib/g++-include/iterator.h /usr/lib/g++-include/iostream.h
Buffer.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
Buffer.o: /usr/lib/g++-include/function.h /usr/include/assert.h
Buffer.o: /usr/include/features.h /usr/include/sys/cdefs.h
Buffer.o: /usr/include/stdlib.h /usr/include/errno.h
Buffer.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
Buffer.o: /usr/include/alloca.h /usr/include/linux/string.h
Buffer.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
Buffer.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
Buffer.o: /usr/include/asm/string.h
CoopSleeper.o: CoopSleeper.h CoopThread.h CoopThreadMgr.h hashset.h
CoopSleeper.o: /usr/lib/g++-include/list.h /usr/lib/g++-include/function.h
CoopSleeper.o: /usr/lib/g++-include/bool.h /usr/include/_G_config.h
CoopSleeper.o: /usr/lib/g++-include/algobase.h /usr/lib/g++-include/pair.h
CoopSleeper.o: /usr/lib/g++-include/iterator.h
CoopSleeper.o: /usr/lib/g++-include/std/stddef.h
CoopSleeper.o: /usr/lib/g++-include/std/cstddef.h
CoopSleeper.o: /usr/lib/g++-include/iostream.h
CoopSleeper.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
CoopSleeper.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
CoopSleeper.o: /usr/lib/g++-include/std/new.h /usr/include/stdlib.h
CoopSleeper.o: /usr/include/features.h /usr/include/sys/cdefs.h
CoopSleeper.o: /usr/include/errno.h /usr/include/linux/errno.h
CoopSleeper.o: /usr/include/asm/errno.h /usr/include/alloca.h
CoopSleeper.o: /usr/include/linux/limits.h hashmap.h AllocDebug.h
CoopSleeper.o: /usr/include/assert.h /usr/include/linux/time.h
CoopSleeper.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
CoopSocket.o: /usr/include/assert.h /usr/include/features.h
CoopSocket.o: /usr/include/sys/cdefs.h /usr/include/linux/string.h
CoopSocket.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
CoopSocket.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
CoopSocket.o: /usr/include/asm/string.h /usr/include/sys/time.h
CoopSocket.o: /usr/include/linux/time.h /usr/include/linux/time.h
CoopSocket.o: /usr/include/sys/types.h /usr/include/sys/bitypes.h
CoopSocket.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
CoopSocket.o: CoopSocket.h CoopSleeper.h CoopThread.h CoopThreadMgr.h
CoopSocket.o: hashset.h /usr/lib/g++-include/list.h
CoopSocket.o: /usr/lib/g++-include/function.h /usr/lib/g++-include/bool.h
CoopSocket.o: /usr/include/_G_config.h /usr/lib/g++-include/algobase.h
CoopSocket.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/iterator.h
CoopSocket.o: /usr/lib/g++-include/std/stddef.h
CoopSocket.o: /usr/lib/g++-include/std/cstddef.h
CoopSocket.o: /usr/lib/g++-include/iostream.h
CoopSocket.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
CoopSocket.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
CoopSocket.o: /usr/lib/g++-include/std/new.h /usr/include/stdlib.h
CoopSocket.o: /usr/include/errno.h /usr/include/linux/errno.h
CoopSocket.o: /usr/include/asm/errno.h /usr/include/alloca.h
CoopSocket.o: /usr/include/linux/limits.h hashmap.h AllocDebug.h
CoopSocket.o: CoopRunnable.h SockUtils.h /usr/include/netdb.h
CoopSocket.o: /usr/include/paths.h /usr/include/sys/socket.h
CoopSocket.o: /usr/include/linux/socket.h /usr/include/asm/socket.h
CoopSocket.o: /usr/include/asm/sockios.h /usr/include/linux/sockios.h
CoopSocket.o: /usr/include/linux/uio.h /usr/include/netinet/in.h
CoopSocket.o: /usr/include/linux/in.h /usr/include/asm/byteorder.h
CoopThread.o: CoopThread.h CoopThreadMgr.h hashset.h
CoopThread.o: /usr/lib/g++-include/list.h /usr/lib/g++-include/function.h
CoopThread.o: /usr/lib/g++-include/bool.h /usr/include/_G_config.h
CoopThread.o: /usr/lib/g++-include/algobase.h /usr/lib/g++-include/pair.h
CoopThread.o: /usr/lib/g++-include/iterator.h
CoopThread.o: /usr/lib/g++-include/std/stddef.h
CoopThread.o: /usr/lib/g++-include/std/cstddef.h
CoopThread.o: /usr/lib/g++-include/iostream.h
CoopThread.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
CoopThread.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
CoopThread.o: /usr/lib/g++-include/std/new.h /usr/include/stdlib.h
CoopThread.o: /usr/include/features.h /usr/include/sys/cdefs.h
CoopThread.o: /usr/include/errno.h /usr/include/linux/errno.h
CoopThread.o: /usr/include/asm/errno.h /usr/include/alloca.h
CoopThread.o: /usr/include/linux/limits.h hashmap.h AllocDebug.h
CoopThreadMgr.o: /usr/include/assert.h /usr/include/features.h
CoopThreadMgr.o: /usr/include/sys/cdefs.h /usr/include/linux/unistd.h
CoopThreadMgr.o: /usr/include/asm/unistd.h /usr/lib/g++-include/algo.h
CoopThreadMgr.o: /usr/include/stdlib.h /usr/lib/g++-include/std/stddef.h
CoopThreadMgr.o: /usr/include/_G_config.h /usr/lib/g++-include/std/cstddef.h
CoopThreadMgr.o: /usr/include/errno.h /usr/include/linux/errno.h
CoopThreadMgr.o: /usr/include/asm/errno.h /usr/include/alloca.h
CoopThreadMgr.o: /usr/lib/g++-include/bool.h /usr/lib/g++-include/pair.h
CoopThreadMgr.o: /usr/lib/g++-include/iterator.h
CoopThreadMgr.o: /usr/lib/g++-include/iostream.h
CoopThreadMgr.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
CoopThreadMgr.o: /usr/lib/g++-include/function.h
CoopThreadMgr.o: /usr/lib/g++-include/algobase.h /usr/lib/g++-include/heap.h
CoopThreadMgr.o: /usr/lib/g++-include/tempbuf.h /usr/include/linux/limits.h
CoopThreadMgr.o: CoopThreadMgr.h hashset.h /usr/lib/g++-include/list.h
CoopThreadMgr.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
CoopThreadMgr.o: /usr/lib/g++-include/std/new.h hashmap.h AllocDebug.h
CoopThreadMgr.o: CoopThread.h
OneTimeSubject.o: /usr/lib/g++-include/algo.h /usr/include/stdlib.h
OneTimeSubject.o: /usr/include/features.h /usr/include/sys/cdefs.h
OneTimeSubject.o: /usr/lib/g++-include/std/stddef.h /usr/include/_G_config.h
OneTimeSubject.o: /usr/lib/g++-include/std/cstddef.h /usr/include/errno.h
OneTimeSubject.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
OneTimeSubject.o: /usr/include/alloca.h /usr/lib/g++-include/bool.h
OneTimeSubject.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/iterator.h
OneTimeSubject.o: /usr/lib/g++-include/iostream.h
OneTimeSubject.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
OneTimeSubject.o: /usr/lib/g++-include/function.h
OneTimeSubject.o: /usr/lib/g++-include/algobase.h /usr/lib/g++-include/heap.h
OneTimeSubject.o: /usr/lib/g++-include/tempbuf.h /usr/include/linux/limits.h
OneTimeSubject.o: OneTimeSubject.h /usr/lib/g++-include/list.h
OneTimeSubject.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
OneTimeSubject.o: /usr/lib/g++-include/std/new.h OneTimeObserver.h
SockUtils.o: /usr/include/linux/fcntl.h /usr/include/asm/fcntl.h
SockUtils.o: /usr/include/linux/signal.h /usr/include/asm/signal.h
SockUtils.o: /usr/include/stdio.h /usr/include/libio.h
SockUtils.o: /usr/include/_G_config.h /usr/include/sys/types.h
SockUtils.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
SockUtils.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
SockUtils.o: /usr/include/sys/bitypes.h SockUtils.h
Subject.o: /usr/lib/g++-include/algo.h /usr/include/stdlib.h
Subject.o: /usr/include/features.h /usr/include/sys/cdefs.h
Subject.o: /usr/lib/g++-include/std/stddef.h /usr/include/_G_config.h
Subject.o: /usr/lib/g++-include/std/cstddef.h /usr/include/errno.h
Subject.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
Subject.o: /usr/include/alloca.h /usr/lib/g++-include/bool.h
Subject.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/iterator.h
Subject.o: /usr/lib/g++-include/iostream.h /usr/lib/g++-include/streambuf.h
Subject.o: /usr/include/libio.h /usr/lib/g++-include/function.h
Subject.o: /usr/lib/g++-include/algobase.h /usr/lib/g++-include/heap.h
Subject.o: /usr/lib/g++-include/tempbuf.h /usr/include/linux/limits.h
Subject.o: Subject.h /usr/lib/g++-include/list.h
Subject.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
Subject.o: /usr/lib/g++-include/std/new.h Observer.h
ThreadTest.o: /usr/lib/g++-include/iostream.h
ThreadTest.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
ThreadTest.o: /usr/include/_G_config.h CoopRunnable.h
sstring.o: sstring.h /usr/lib/g++-include/algobase.h
sstring.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/bool.h
sstring.o: /usr/include/_G_config.h /usr/lib/g++-include/iterator.h
sstring.o: /usr/lib/g++-include/std/stddef.h
sstring.o: /usr/lib/g++-include/std/cstddef.h /usr/lib/g++-include/iostream.h
sstring.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
sstring.o: /usr/lib/g++-include/function.h /usr/include/linux/limits.h
sstring.o: /usr/include/linux/string.h /usr/include/linux/types.h
sstring.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
sstring.o: /usr/include/asm/types.h /usr/include/asm/string.h

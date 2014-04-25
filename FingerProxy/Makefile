PROGNAME=	FPServer
OBJS=		Talker.o FPTalker.o FriendsdTalker.o \
		CoopFinger.o \
		FingerContainer.o FriendsdContainer.o \
		Property.o \
		FingerProxy.o
SUBDIRS=	../CoopThread
SUBOBJS=	../CoopThread/AllocDebug.o \
		../CoopThread/sstring.o \
		../CoopThread/Buffer.o \
		../CoopThread/CoopThreadMgr.o \
		../CoopThread/CoopThread.o \
		../CoopThread/CoopSocket.o \
		../CoopThread/CoopSleeper.o \
		../CoopThread/SockUtils.o \
		../CoopThread/Subject.o \
		../CoopThread/OneTimeSubject.o
CFLAGS=		-O2 -Wall -g -pg
CPPFLAGS=	-DALLOCDEBUG
LIBS=		
INCLUDES=	-I. -I../CoopThread
CCC=		g++
CC=		gcc

all:		$(PROGNAME)

$(PROGNAME):	$(OBJS) CoopThread
	$(CCC) $(CFLAGS) $(LIBS) $(SUBOBJS) $(OBJS) -o $(PROGNAME)

CoopThread:
	(cd ../CoopThread ; $(MAKE) objs) || exit 1

.cc.o:
	$(CCC) $(INCLUDES) $(CFLAGS) $(CPPFLAGS) -c $<

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $<

clean:
	@for i in $(SUBDIRS); do (cd $$i ; $(MAKE) clean) || exit 1 ; done
	rm -f $(OBJS) $(PROGNAME)
# DO NOT DELETE

CoopFinger.o: /usr/include/assert.h /usr/include/features.h
CoopFinger.o: /usr/include/sys/cdefs.h /usr/include/linux/fcntl.h
CoopFinger.o: /usr/include/asm/fcntl.h CoopFinger.h
CoopFinger.o: /usr/lib/g++-include/vector.h /usr/lib/g++-include/function.h
CoopFinger.o: /usr/lib/g++-include/bool.h /usr/include/_G_config.h
CoopFinger.o: /usr/lib/g++-include/algobase.h /usr/lib/g++-include/pair.h
CoopFinger.o: /usr/lib/g++-include/iterator.h /usr/include/linux/stddef.h
CoopFinger.o: /usr/lib/g++-include/iostream.h
CoopFinger.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
CoopFinger.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
CoopFinger.o: /usr/lib/g++-include/std/new.h
CoopFinger.o: /usr/lib/g++-include/std/cstddef.h /usr/include/stdlib.h
CoopFinger.o: /usr/include/errno.h /usr/include/linux/errno.h
CoopFinger.o: /usr/include/asm/errno.h /usr/include/alloca.h
CoopFinger.o: /usr/include/linux/limits.h ../CoopThread/CoopRunnable.h
CoopFinger.o: ../CoopThread/CoopSocket.h ../CoopThread/CoopSleeper.h
CoopFinger.o: ../CoopThread/CoopThread.h ../CoopThread/CoopThreadMgr.h
CoopFinger.o: ../CoopThread/hashset.h /usr/lib/g++-include/list.h
CoopFinger.o: ../CoopThread/hashmap.h ../CoopThread/AllocDebug.h
CoopFinger.o: ../CoopThread/SockUtils.h ../CoopThread/sstring.h
CoopFinger.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
CoopFinger.o: /usr/include/netdb.h /usr/include/paths.h
CoopFinger.o: /usr/include/sys/types.h /usr/include/linux/types.h
CoopFinger.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
CoopFinger.o: /usr/include/asm/types.h /usr/include/sys/bitypes.h
CoopFinger.o: /usr/include/sys/socket.h /usr/include/linux/socket.h
CoopFinger.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
CoopFinger.o: /usr/include/linux/sockios.h /usr/include/linux/uio.h
CoopFinger.o: /usr/include/netinet/in.h /usr/include/linux/in.h
CoopFinger.o: /usr/include/asm/byteorder.h ../CoopThread/Subject.h
CoopFinger.o: ../CoopThread/Buffer.h
FPTalker.o: /usr/include/assert.h /usr/include/features.h
FPTalker.o: /usr/include/sys/cdefs.h /usr/include/linux/ctype.h
FPTalker.o: /usr/include/stdio.h /usr/include/libio.h
FPTalker.o: /usr/include/_G_config.h FPTalker.h Talker.h
FPTalker.o: ../CoopThread/CoopSleeper.h ../CoopThread/CoopThread.h
FPTalker.o: ../CoopThread/CoopThreadMgr.h ../CoopThread/hashset.h
FPTalker.o: /usr/lib/g++-include/list.h /usr/lib/g++-include/function.h
FPTalker.o: /usr/lib/g++-include/bool.h /usr/lib/g++-include/algobase.h
FPTalker.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/iterator.h
FPTalker.o: /usr/include/linux/stddef.h /usr/lib/g++-include/iostream.h
FPTalker.o: /usr/lib/g++-include/streambuf.h /usr/lib/g++-include/defalloc.h
FPTalker.o: /usr/lib/g++-include/new.h /usr/lib/g++-include/std/new.h
FPTalker.o: /usr/lib/g++-include/std/cstddef.h /usr/include/stdlib.h
FPTalker.o: /usr/include/errno.h /usr/include/linux/errno.h
FPTalker.o: /usr/include/asm/errno.h /usr/include/alloca.h
FPTalker.o: /usr/include/linux/limits.h ../CoopThread/hashmap.h
FPTalker.o: ../CoopThread/AllocDebug.h ../CoopThread/CoopRunnable.h
FPTalker.o: ../CoopThread/CoopSocket.h ../CoopThread/SockUtils.h
FPTalker.o: ../CoopThread/sstring.h /usr/include/linux/unistd.h
FPTalker.o: /usr/include/asm/unistd.h /usr/include/netdb.h
FPTalker.o: /usr/include/paths.h /usr/include/sys/types.h
FPTalker.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
FPTalker.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
FPTalker.o: /usr/include/sys/bitypes.h /usr/include/sys/socket.h
FPTalker.o: /usr/include/linux/socket.h /usr/include/asm/socket.h
FPTalker.o: /usr/include/asm/sockios.h /usr/include/linux/sockios.h
FPTalker.o: /usr/include/linux/uio.h /usr/include/netinet/in.h
FPTalker.o: /usr/include/linux/in.h /usr/include/asm/byteorder.h
FPTalker.o: ../CoopThread/OneTimeObserver.h FPFactory.h
FPTalker.o: ../CoopThread/Buffer.h FingerContainer.h CoopFinger.h
FPTalker.o: /usr/lib/g++-include/vector.h ../CoopThread/Subject.h
FPTalker.o: ../CoopThread/Observer.h ../CoopThread/OneTimeSubject.h
FingerContainer.o: FingerContainer.h CoopFinger.h
FingerContainer.o: /usr/lib/g++-include/vector.h
FingerContainer.o: /usr/lib/g++-include/function.h
FingerContainer.o: /usr/lib/g++-include/bool.h /usr/include/_G_config.h
FingerContainer.o: /usr/lib/g++-include/algobase.h
FingerContainer.o: /usr/lib/g++-include/pair.h
FingerContainer.o: /usr/lib/g++-include/iterator.h
FingerContainer.o: /usr/include/linux/stddef.h
FingerContainer.o: /usr/lib/g++-include/iostream.h
FingerContainer.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
FingerContainer.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
FingerContainer.o: /usr/lib/g++-include/std/new.h
FingerContainer.o: /usr/lib/g++-include/std/cstddef.h /usr/include/stdlib.h
FingerContainer.o: /usr/include/features.h /usr/include/sys/cdefs.h
FingerContainer.o: /usr/include/errno.h /usr/include/linux/errno.h
FingerContainer.o: /usr/include/asm/errno.h /usr/include/alloca.h
FingerContainer.o: /usr/include/linux/limits.h ../CoopThread/CoopRunnable.h
FingerContainer.o: ../CoopThread/CoopSocket.h ../CoopThread/CoopSleeper.h
FingerContainer.o: ../CoopThread/CoopThread.h ../CoopThread/CoopThreadMgr.h
FingerContainer.o: ../CoopThread/hashset.h /usr/lib/g++-include/list.h
FingerContainer.o: ../CoopThread/hashmap.h ../CoopThread/AllocDebug.h
FingerContainer.o: ../CoopThread/SockUtils.h ../CoopThread/sstring.h
FingerContainer.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
FingerContainer.o: /usr/include/netdb.h /usr/include/paths.h
FingerContainer.o: /usr/include/sys/types.h /usr/include/linux/types.h
FingerContainer.o: /usr/include/linux/posix_types.h
FingerContainer.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
FingerContainer.o: /usr/include/sys/bitypes.h /usr/include/sys/socket.h
FingerContainer.o: /usr/include/linux/socket.h /usr/include/asm/socket.h
FingerContainer.o: /usr/include/asm/sockios.h /usr/include/linux/sockios.h
FingerContainer.o: /usr/include/linux/uio.h /usr/include/netinet/in.h
FingerContainer.o: /usr/include/linux/in.h /usr/include/asm/byteorder.h
FingerContainer.o: ../CoopThread/Subject.h ../CoopThread/Buffer.h
FingerContainer.o: ../CoopThread/Observer.h ../CoopThread/OneTimeSubject.h
FingerContainer.o: FPFactory.h FPTalker.h Talker.h
FingerContainer.o: ../CoopThread/OneTimeObserver.h /usr/include/arpa/inet.h
FingerContainer.o: /usr/include/sys/param.h /usr/include/linux/limits.h
FingerContainer.o: /usr/include/linux/param.h /usr/include/asm/param.h
FingerContainer.o: /usr/include/assert.h /usr/include/linux/string.h
FingerContainer.o: /usr/include/asm/string.h /usr/include/linux/time.h
FingerProxy.o: FingerContainer.h CoopFinger.h /usr/lib/g++-include/vector.h
FingerProxy.o: /usr/lib/g++-include/function.h /usr/lib/g++-include/bool.h
FingerProxy.o: /usr/include/_G_config.h /usr/lib/g++-include/algobase.h
FingerProxy.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/iterator.h
FingerProxy.o: /usr/include/linux/stddef.h /usr/lib/g++-include/iostream.h
FingerProxy.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
FingerProxy.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
FingerProxy.o: /usr/lib/g++-include/std/new.h
FingerProxy.o: /usr/lib/g++-include/std/cstddef.h /usr/include/stdlib.h
FingerProxy.o: /usr/include/features.h /usr/include/sys/cdefs.h
FingerProxy.o: /usr/include/errno.h /usr/include/linux/errno.h
FingerProxy.o: /usr/include/asm/errno.h /usr/include/alloca.h
FingerProxy.o: /usr/include/linux/limits.h ../CoopThread/CoopRunnable.h
FingerProxy.o: ../CoopThread/CoopSocket.h ../CoopThread/CoopSleeper.h
FingerProxy.o: ../CoopThread/CoopThread.h ../CoopThread/CoopThreadMgr.h
FingerProxy.o: ../CoopThread/hashset.h /usr/lib/g++-include/list.h
FingerProxy.o: ../CoopThread/hashmap.h ../CoopThread/AllocDebug.h
FingerProxy.o: ../CoopThread/SockUtils.h ../CoopThread/sstring.h
FingerProxy.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
FingerProxy.o: /usr/include/netdb.h /usr/include/paths.h
FingerProxy.o: /usr/include/sys/types.h /usr/include/linux/types.h
FingerProxy.o: /usr/include/linux/posix_types.h
FingerProxy.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
FingerProxy.o: /usr/include/sys/bitypes.h /usr/include/sys/socket.h
FingerProxy.o: /usr/include/linux/socket.h /usr/include/asm/socket.h
FingerProxy.o: /usr/include/asm/sockios.h /usr/include/linux/sockios.h
FingerProxy.o: /usr/include/linux/uio.h /usr/include/netinet/in.h
FingerProxy.o: /usr/include/linux/in.h /usr/include/asm/byteorder.h
FingerProxy.o: ../CoopThread/Subject.h ../CoopThread/Buffer.h
FingerProxy.o: ../CoopThread/Observer.h ../CoopThread/OneTimeSubject.h
FingerProxy.o: FPFactory.h FriendsdContainer.h /usr/include/assert.h
FingerProxy.o: ../CoopThread/OneTimeObserver.h FPTalker.h Talker.h
FingerProxy.o: FriendsdTalker.h Property.h /usr/include/stdio.h
FingerProxy.o: /usr/include/linux/time.h
FriendsdContainer.o: /usr/include/assert.h /usr/include/features.h
FriendsdContainer.o: /usr/include/sys/cdefs.h /usr/include/linux/ctype.h
FriendsdContainer.o: /usr/include/linux/string.h /usr/include/linux/types.h
FriendsdContainer.o: /usr/include/linux/posix_types.h
FriendsdContainer.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
FriendsdContainer.o: /usr/include/asm/string.h /usr/include/linux/time.h
FriendsdContainer.o: FriendsdContainer.h /usr/lib/g++-include/vector.h
FriendsdContainer.o: /usr/lib/g++-include/function.h
FriendsdContainer.o: /usr/lib/g++-include/bool.h /usr/include/_G_config.h
FriendsdContainer.o: /usr/lib/g++-include/algobase.h
FriendsdContainer.o: /usr/lib/g++-include/pair.h
FriendsdContainer.o: /usr/lib/g++-include/iterator.h
FriendsdContainer.o: /usr/include/linux/stddef.h
FriendsdContainer.o: /usr/lib/g++-include/iostream.h
FriendsdContainer.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
FriendsdContainer.o: /usr/lib/g++-include/defalloc.h
FriendsdContainer.o: /usr/lib/g++-include/new.h
FriendsdContainer.o: /usr/lib/g++-include/std/new.h
FriendsdContainer.o: /usr/lib/g++-include/std/cstddef.h /usr/include/stdlib.h
FriendsdContainer.o: /usr/include/errno.h /usr/include/linux/errno.h
FriendsdContainer.o: /usr/include/asm/errno.h /usr/include/alloca.h
FriendsdContainer.o: /usr/include/linux/limits.h
FriendsdContainer.o: ../CoopThread/OneTimeObserver.h
FriendsdContainer.o: ../CoopThread/OneTimeSubject.h
FriendsdContainer.o: /usr/lib/g++-include/list.h FPFactory.h
FriendsdContainer.o: FingerContainer.h CoopFinger.h
FriendsdContainer.o: ../CoopThread/CoopRunnable.h ../CoopThread/CoopSocket.h
FriendsdContainer.o: ../CoopThread/CoopSleeper.h ../CoopThread/CoopThread.h
FriendsdContainer.o: ../CoopThread/CoopThreadMgr.h ../CoopThread/hashset.h
FriendsdContainer.o: ../CoopThread/hashmap.h ../CoopThread/AllocDebug.h
FriendsdContainer.o: ../CoopThread/SockUtils.h ../CoopThread/sstring.h
FriendsdContainer.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
FriendsdContainer.o: /usr/include/netdb.h /usr/include/paths.h
FriendsdContainer.o: /usr/include/sys/types.h /usr/include/sys/bitypes.h
FriendsdContainer.o: /usr/include/sys/socket.h /usr/include/linux/socket.h
FriendsdContainer.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
FriendsdContainer.o: /usr/include/linux/sockios.h /usr/include/linux/uio.h
FriendsdContainer.o: /usr/include/netinet/in.h /usr/include/linux/in.h
FriendsdContainer.o: /usr/include/asm/byteorder.h ../CoopThread/Subject.h
FriendsdContainer.o: ../CoopThread/Buffer.h ../CoopThread/Observer.h
FriendsdTalker.o: FriendsdTalker.h Talker.h ../CoopThread/CoopSleeper.h
FriendsdTalker.o: ../CoopThread/CoopThread.h ../CoopThread/CoopThreadMgr.h
FriendsdTalker.o: ../CoopThread/hashset.h /usr/lib/g++-include/list.h
FriendsdTalker.o: /usr/lib/g++-include/function.h /usr/lib/g++-include/bool.h
FriendsdTalker.o: /usr/include/_G_config.h /usr/lib/g++-include/algobase.h
FriendsdTalker.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/iterator.h
FriendsdTalker.o: /usr/include/linux/stddef.h /usr/lib/g++-include/iostream.h
FriendsdTalker.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
FriendsdTalker.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
FriendsdTalker.o: /usr/lib/g++-include/std/new.h
FriendsdTalker.o: /usr/lib/g++-include/std/cstddef.h /usr/include/stdlib.h
FriendsdTalker.o: /usr/include/features.h /usr/include/sys/cdefs.h
FriendsdTalker.o: /usr/include/errno.h /usr/include/linux/errno.h
FriendsdTalker.o: /usr/include/asm/errno.h /usr/include/alloca.h
FriendsdTalker.o: /usr/include/linux/limits.h ../CoopThread/hashmap.h
FriendsdTalker.o: ../CoopThread/AllocDebug.h ../CoopThread/CoopRunnable.h
FriendsdTalker.o: ../CoopThread/CoopSocket.h ../CoopThread/SockUtils.h
FriendsdTalker.o: ../CoopThread/sstring.h /usr/include/linux/unistd.h
FriendsdTalker.o: /usr/include/asm/unistd.h /usr/include/netdb.h
FriendsdTalker.o: /usr/include/paths.h /usr/include/sys/types.h
FriendsdTalker.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
FriendsdTalker.o: /usr/include/asm/posix_types.h /usr/include/asm/types.h
FriendsdTalker.o: /usr/include/sys/bitypes.h /usr/include/sys/socket.h
FriendsdTalker.o: /usr/include/linux/socket.h /usr/include/asm/socket.h
FriendsdTalker.o: /usr/include/asm/sockios.h /usr/include/linux/sockios.h
FriendsdTalker.o: /usr/include/linux/uio.h /usr/include/netinet/in.h
FriendsdTalker.o: /usr/include/linux/in.h /usr/include/asm/byteorder.h
FriendsdTalker.o: ../CoopThread/OneTimeObserver.h FPFactory.h
FriendsdTalker.o: FriendsdContainer.h /usr/include/assert.h
FriendsdTalker.o: /usr/lib/g++-include/vector.h
FriendsdTalker.o: ../CoopThread/OneTimeSubject.h ../CoopThread/Buffer.h
Property.o: Property.h ../CoopThread/hashmap.h /usr/lib/g++-include/list.h
Property.o: /usr/lib/g++-include/function.h /usr/lib/g++-include/bool.h
Property.o: /usr/include/_G_config.h /usr/lib/g++-include/algobase.h
Property.o: /usr/lib/g++-include/pair.h /usr/lib/g++-include/iterator.h
Property.o: /usr/include/linux/stddef.h /usr/lib/g++-include/iostream.h
Property.o: /usr/lib/g++-include/streambuf.h /usr/include/libio.h
Property.o: /usr/lib/g++-include/defalloc.h /usr/lib/g++-include/new.h
Property.o: /usr/lib/g++-include/std/new.h /usr/lib/g++-include/std/cstddef.h
Property.o: /usr/include/stdlib.h /usr/include/features.h
Property.o: /usr/include/sys/cdefs.h /usr/include/errno.h
Property.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
Property.o: /usr/include/alloca.h /usr/include/linux/limits.h
Property.o: ../CoopThread/sstring.h /usr/include/assert.h
Property.o: /usr/include/linux/ctype.h /usr/include/stdio.h
Property.o: /usr/include/linux/string.h /usr/include/linux/types.h
Property.o: /usr/include/linux/posix_types.h /usr/include/asm/posix_types.h
Property.o: /usr/include/asm/types.h /usr/include/asm/string.h
Talker.o: Talker.h ../CoopThread/CoopSleeper.h ../CoopThread/CoopThread.h
Talker.o: ../CoopThread/CoopThreadMgr.h ../CoopThread/hashset.h
Talker.o: /usr/lib/g++-include/list.h /usr/lib/g++-include/function.h
Talker.o: /usr/lib/g++-include/bool.h /usr/include/_G_config.h
Talker.o: /usr/lib/g++-include/algobase.h /usr/lib/g++-include/pair.h
Talker.o: /usr/lib/g++-include/iterator.h /usr/include/linux/stddef.h
Talker.o: /usr/lib/g++-include/iostream.h /usr/lib/g++-include/streambuf.h
Talker.o: /usr/include/libio.h /usr/lib/g++-include/defalloc.h
Talker.o: /usr/lib/g++-include/new.h /usr/lib/g++-include/std/new.h
Talker.o: /usr/lib/g++-include/std/cstddef.h /usr/include/stdlib.h
Talker.o: /usr/include/features.h /usr/include/sys/cdefs.h
Talker.o: /usr/include/errno.h /usr/include/linux/errno.h
Talker.o: /usr/include/asm/errno.h /usr/include/alloca.h
Talker.o: /usr/include/linux/limits.h ../CoopThread/hashmap.h
Talker.o: ../CoopThread/AllocDebug.h ../CoopThread/CoopRunnable.h FPFactory.h
Talker.o: ../CoopThread/sstring.h

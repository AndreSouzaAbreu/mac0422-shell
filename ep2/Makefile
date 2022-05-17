MINIXDIR=/home/indra/usp/minix
TTYDIR=${MINIXDIR}/drivers/tty
ISDIR=${MINIXDIR}/servers/is
PMDIR=${MINIXDIR}/servers/pm
SYSDIR=${MINIXDIR}/kernel
GUESTMINIXDIR=/usr/src
GUESTBUILDDIR=/usr/src/tools
ISSRC=${ISDIR}/dmp.c ${ISDIR}/proto.h ${ISDIR}/dmp_kernel.c
MINIXSRC=${MINIXDIR}/include/minix/callnr.h \
         ${MINIXDIR}/include/unistd.h \
         ${MINIXDIR}/lib/posix/_lockpriority.c \
         ${MINIXDIR}/lib/posix/_unlockpriority.c \
         ${MINIXDIR}/lib/posix/Makefile.in
PMSRC=${PMDIR}/table.c ${PMDIR}/proto.h ${PMDIR}/misc.c  ${PMDIR}/Makefile
KERNELSRC=${SYSDIR}/system.h ${SYSDIR}/system/do_unlockpriority.c ${SYSDIR}/system/do_lockpriority.c \
		  ${SYSDIR}/system/Makefile ${SYSDIR}/system.c ${SYSDIR}/proc.h ${SYSDIR}/proc.c
KERNELLIBSRC=${MINIXDIR}/include/minix/syslib.h ${MINIXDIR}/include/minix/com.h \
			 ${MINIXDIR}/lib/syslib/sys_unlockpriority.c ${MINIXDIR}/lib/syslib/sys_lockpriority.c ${MINIXDIR}/lib/syslib/Makefile.in


src: Makefile ./test.c ${MINIXSRC} ${ISSRC} ${PMSRC} ${KERNELSRC} ${KERNELLIBSRC}    # host
	cp ${MINIXSRC} ./src/lib/
	cp ${ISSRC} ./src/is/
	cp ${PMSRC} ./src/pm/
	cp ${KERNELSRC} ./src/kernel/
	cp ${KERNELLIBSRC} ./src/kernellib/

iso: src                                           # host
	genisoimage -l -o ep2.iso -r ./src Makefile test.c

zip: Minix.ova relatorio.pdf                       # host
	tar -cvf - Minix.ova relatorio.pdf | xz -9e - > ep1.tar.xz

.PHONY: update copy

update:                                            # minix guest
	isoread /dev/c0d2 MAKEFILE >           /home/ep2/Makefile
	isoread /dev/c0d2 TEST.C >             /home/ep2/test.c
	isoread /dev/c0d2 LIB/CALLNR.H >       /home/ep2/src/callnr.h
	isoread /dev/c0d2 LIB/UNISTD.H >       /home/ep2/src/unistd.h
	isoread /dev/c0d2 LIB/MAKEFILE.IN >    /home/ep2/src/lib/Makefile.in
	isoread /dev/c0d2 LIB/_LOCKPRIORITY.C >   /home/ep2/src/lib/_lockpriority.c
	isoread /dev/c0d2 LIB/_UNLOCKPRIORITY.C > /home/ep2/src/lib/_unlockpriority.c
	isoread /dev/c0d2 IS/DMP.C >           /home/ep2/src/is/dmp.c
	isoread /dev/c0d2 IS/DMP_KERNEL.C >      /home/ep2/src/is/dmp_kernel.c
	isoread /dev/c0d2 IS/PROTO.H >         /home/ep2/src/is/proto.h
	isoread /dev/c0d2 PM/MAKEFILE >        /home/ep2/src/pm/Makefile
	isoread /dev/c0d2 PM/PROTO.H >         /home/ep2/src/pm/proto.h
	isoread /dev/c0d2 PM/TABLE.C >         /home/ep2/src/pm/table.c
	isoread /dev/c0d2 PM/MISC.C >          /home/ep2/src/pm/misc.c
	isoread /dev/c0d2 KERNEL/SYSTEM.H >    /home/ep2/src/kernel/system.h
	isoread /dev/c0d2 KERNEL/DO_LOCKPRIORITY.C >    /home/ep2/src/kernel/do_lockpriority.c
	isoread /dev/c0d2 KERNEL/DO_UNLOCKPRIORITY.C >    /home/ep2/src/kernel/do_unlockpriority.c
	isoread /dev/c0d2 KERNEL/MAKEFILE >    /home/ep2/src/kernel/Makefile
	isoread /dev/c0d2 KERNEL/SYSTEM.C >    /home/ep2/src/kernel/system.c
	isoread /dev/c0d2 KERNEL/PROC.C >    /home/ep2/src/kernel/proc.c
	isoread /dev/c0d2 KERNEL/PROC.H >    /home/ep2/src/kernel/proc.h
	isoread /dev/c0d2 KERNELLIB/SYSLIB.H >    /home/ep2/src/kernellib/syslib.h
	isoread /dev/c0d2 KERNELLIB/COM.H >    /home/ep2/src/kernellib/com.h
	isoread /dev/c0d2 KERNELLIB/SYS_LOCKPRIORITY.C >    /home/ep2/src/kernellib/sys_lockpriority.c
	isoread /dev/c0d2 KERNELLIB/SYS_UNLOCKPRIORITY.C >    /home/ep2/src/kernellib/sys_unlockpriority.c
	isoread /dev/c0d2 KERNELLIB/MAKEFILE.IN >    /home/ep2/src/kernellib/Makefile.in

test: test.c
	cc test.c -o test

build:                                              # minix guest
	@echo "Building kernel call"
	cp /home/ep2/src/kernel/proc.* ${GUESTMINIXDIR}/kernel/
	cp /home/ep2/src/kernel/system.* ${GUESTMINIXDIR}/kernel/
	cp /home/ep2/src/kernel/do_lockpriority.c ${GUESTMINIXDIR}/kernel/system/
	cp /home/ep2/src/kernel/do_unlockpriority.c ${GUESTMINIXDIR}/kernel/system/
	cp /home/ep2/src/kernel/Makefile          ${GUESTMINIXDIR}/kernel/system/
	cp /home/ep2/src/kernellib/syslib.h ${GUESTMINIXDIR}/include/minix/
	cp /home/ep2/src/kernellib/com.h    ${GUESTMINIXDIR}/include/minix/
	cp /home/ep2/src/kernellib/sys_lockpriority.c ${GUESTMINIXDIR}/lib/syslib/
	cp /home/ep2/src/kernellib/sys_unlockpriority.c ${GUESTMINIXDIR}/lib/syslib/
	cp /home/ep2/src/kernellib/Makefile.in        ${GUESTMINIXDIR}/lib/syslib/
	@echo "Building library"
	cp /home/ep2/src/callnr.h ${GUESTMINIXDIR}/include/minix/callnr.h
	cp /home/ep2/src/unistd.h ${GUESTMINIXDIR}/include/unistd.h
	cp /home/ep2/src/lib/* ${GUESTMINIXDIR}/lib/posix/
	# cd ${GUESTMINIXDIR}/lib/posix/ && make Makefile
	# cd ${GUESTMINIXDIR} && make libraries
	@echo "Building servers"
	cp /home/ep2/src/is/* ${GUESTMINIXDIR}/servers/is/
	cp /home/ep2/src/pm/* ${GUESTMINIXDIR}/servers/pm/
	# cd ${GUESTMINIXDIR}/servers/ && make image && make install
	@echo "Run 'cd ${GUESTBUILDDIR}' and 'make fresh install'"

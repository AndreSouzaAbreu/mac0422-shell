#!/bin/bash

export MINIXDIR=/home/indra/usp/minix/
export TTYDIR=$MINIXDIR/drivers/tty
export ISDIR=$MINIXDIR/servers/is
export SYSDIR=$MINIXDIR/kernel
export PSDIR=$MINIXDIR/servers/pm

cd $MINIXDIR
# test1
# vim $TTYDIR/tty.c $TTYDIR/tty.h $TTYDIR/console.c $TTYDIR/keyboard.c $TTYDIR/pty.c $TTYDIR/rs232.c $TTYDIR/vidcopy.s $TTYDIR/keymaps $TTYDIR/Makefile

# parte 1 do ep2
# vim $ISDIR/dmp_kernel.c $ISDIR/dmp.c $ISDIR/proto.h

# parte 2.1 do ep2
# vim $MINIXDIR/lib/posix/_lockpriority.c $MINIXDIR/lib/posix/_unlockpriority.c $MINIXDIR/lib/posix/Makefile.in \
#     $PSDIR/table.c $PSDIR/proto.h \
#     $PSDIR/misc.c $PSDIR/Makefile $MINIXDIR/include/minix/callnr.h $MINIXDIR/include/unistd.h

# parte 2.2 do ep2: adiciona kernel call
# vim $SYSDIR/system.h $SYSDIR/system/do_lockpriority.c $SYSDIR/system/do_unlockpriority.c \
#     $SYSDIR/system/Makefile $SYSDIR/system.c $MINIXDIR/include/minix/syslib.h $MINIXDIR/include/minix/com.h \
#     $MINIXDIR/lib/syslib/sys_lockpriority.c $MINIXDIR/lib/syslib/sys_unlockpriority.c $MINIXDIR/lib/syslib/Makefile.in

# parte 2.2 do ep2: melhora a interface entre chamadas
# vim $SYSDIR/system/do_lockpriority.c $SYSDIR/proc.c $SYSDIR/proc.h \
#     $MINIXDIR/lib/syslib/sys_lockpriority.c \
#     $PSDIR/misc.c $PSDIR/mproc.h

# parte 2.2 do ep2: configura o escalonador
vim $SYSDIR/proc.c $SYSDIR/proc.h $SYSDIR/system/do_unlockpriority.c $SYSDIR/system/do_lockpriority.c $SYSDIR/priv.h

cd -

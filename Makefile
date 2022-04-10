CC := cc
CFLAGS := -Wall

BIN := mac422shell
SRC := ${BIN}.c
DEST_DIR_BIN := /usr/local/bin
DEST_DIR_SRC := /usr/local/src

MINIX_SCRIPT_WRITE := ./minix_write.sh
MINIX_SCRIPT_READ := ./minix_read.sh

PAPER := relatorio
PAPER_MD := ${PAPER}.md
PAPER_PDF := ${PAPER}.pdf

################################################################################

#
# main program
#

${BIN}:

theo:
	cc -Wall -Wextra -ansi mac422shellTheo.c -o mac422shellTheo
	cc test.c -o test

build: ${BIN}

install: build
	mkdir -p ${DEST_DIR_SRC} ${DIR_BIN}
	cp -f ${BIN} ${DEST_DIR_BIN}
	cp -r ${SRC} ${DEST_DIR_SRC}

clean:
	@rm -f ${DEST_DIR_SRC}/${SRC}
	@rm -f ${DEST_DIR_BIN}/${BIN}

#
# transfer files between host and guest
#

sync:
	${MINIX_SCRIPT_WRITE} ${DIR_SRC}

init:
	${MINIX_SCRIPT_WRITE} ${MINIX_SCRIPT_READ}

# 
# assignment paper
#

${PAPER_PDF}: ${PAPER_MD}
	pandoc --template college-report $^ -o $@

paper: ${PAPER_PDF}

watch:
	echo ${PAPER_MD} | entr make paper

.PHONY: build clean init install paper sync

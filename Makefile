CC := cc
CFLAGS := -Wall

BIN := mac422shell
SRC := ${BIN}.c
DEST_DIR_BIN := /usr/local/bin
DEST_DIR_SRC := /usr/local/src

${BIN}:

install: ${BIN}
	mkdir -p ${DEST_DIR_SRC} ${DIR_BIN}
	cp -f ${BIN} ${DEST_DIR_BIN}
	cp -r ${SRC} ${DEST_DIR_SRC}

clean:
	@rm -f ${DEST_DIR_SRC}/${SRC}
	@rm -f ${DEST_DIR_BIN}/${BIN}

.PHONY: install clean

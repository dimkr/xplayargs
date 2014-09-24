CC ?= cc
CFLAGS ?= -O2 -pipe
LDFLAGS ?= -s
DESTDIR ?=
BIN_DIR ?= /bin
MAN_DIR ?= /usr/share/man
DOC_DIR ?= /usr/share/doc

CFLAGS += -Wall -pedantic
INSTALL = install -v

all: xplayargs

xplayargs: xplayargs.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

xplayargs.o: xplayargs.c
	$(CC) -c -o $@ $< $(CFLAGS)

install: all
	$(INSTALL) -D -m 755 xplayargs $(DESTDIR)/$(BIN_DIR)/xplayargs
	$(INSTALL) -D -m 644 xplayargs.1 $(DESTDIR)/$(MAN_DIR)/man1/xplayargs.1
	$(INSTALL) -D -m 644 README $(DESTDIR)/$(DOC_DIR)/xplayargs/README
	$(INSTALL) -m 644 AUTHORS $(DESTDIR)/$(DOC_DIR)/xplayargs/AUTHORS
	$(INSTALL) -m 644 COPYING $(DESTDIR)/$(DOC_DIR)/xplayargs/COPYING

clean:
	rm -f xplayargs xplayargs.o

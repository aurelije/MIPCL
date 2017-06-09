CC=gcc
LFLAGS=
YFLAGS=-d
INS=install
ifdef OS
ifeq "Windows_NT" "$(OS)"
WIN32=yes
endif
endif
#
ifndef WIN32
MIP_DIR=$(MIPCLDIR)
TARGET=mod2cpp
LEX=/usr/bin/flex
YACC=/usr/bin/bison
else
MIP_DIR=/$(subst :/,/,$(subst \,/,$(MIPCLDIR)))
TARGET=mod2cpp.exe
LEX='c/Program Files/GnuWin32/Flex/bin/flex'
YACC='/c/Program Files/GnuWin32/Bison/bin/bison'
endif
#
INSDIR=$(MIP_DIR)/bin
vpath %.l ./sources
vpath %.y ./sources
#vpath %.c ./sources
#vpath %.h ./sources
build: lexer.c parser.tab.c
	$(CC) lexer.c parser.tab.c -o $(TARGET)
%tab.c %tab.h: %y
	$(YACC) $(YFLAGS) $<
%c: %l
	$(LEX) $(LFLAGS) -o$@ $<
#
.PHONY: clean
clean:
	rm -f *.o *.c *.h
install: $(TARGET)
	$(INS) $(TARGET)  $(INSDIR)
	chmod +x $(INSDIR)/$(TARGET)
	rm $(TARGET)
all: build install

CC=g++
ARC=-mfpmath=sse -msse2
CFLAGS=-c -g $(ARC) -I$(HDR_MIPSHELL) -I$(HDR_MIPCL) -D__THREADS_=""
LDFLAGS=-shared-libgcc
RM=rm -f
INS=install
INSDIR=../bin
MIP_DIR=$(MIPCLDIR)
CFLAGS+=-DMIPSHELL_API="" -DMIP_API=""
LDFLAGS+= -L$(MIP_DIR)/lib -lmipcl -lmipshell_dbg
TARGET=fcnf_dbg_ms
#
HDR_MIPCL=$(MIP_DIR)/mipcl/headers
HDR_MIPSHELL=$(MIP_DIR)/mipshell/headers
#
#vpath %.h ../sources
#vpath %.mod ../sources
#vpath %.cpp ../sources
VPATH=../sources
CFLAGS+=-I$(VPATH)
MOD= $(shell echo $(VPATH)/*.mod)
AUX=$(MOD:.mod=.aux)
SRC=$(notdir $(shell echo $(VPATH)/*.cpp))
#HDR=$(wildcard *.h)
OBJS=$(SRC:.cpp=.o)
#
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
%.aux : %.mod ;	$(MIP_DIR)/bin/mod2cpp $< $(<D)/$@
#
build: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)
#dependencies
fcnf.aux: fcnf.mod
fcnf.o: fcnf.cpp fcnf.aux fcnf.h
main.o: main.cpp fcnf.h
#
.PHONY: clean test
clean:
	$(RM) $(OBJS) $(AUX)
install: $(TARGET)
	$(INS) $(TARGET) $(INSDIR)
	$(RM) $(TARGET)
all: build install
test:
	echo $(MIPSHELL_API)

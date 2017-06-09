CC=g++
ARC=-mfpmath=sse -msse2
CFLAGS=-c -g $(ARC) -I$(HDR_MIPSHELL) -I$(HDR_MIPCL) -D__THREADS_=""
LDFLAGS=-shared-libgcc
INSDIR=../bin
RM=rm -f
INS=install
MIP_DIR=$(MIPCLDIR)
CFLAGS+=-DMIPSHELL_API="" -DMIP_API=""
LDFLAGS+= -L$(MIP_DIR)/lib -lmipcl -lmipshell_dbg
TARGET=genassign_dbg_ms
#
HDR_MIPCL=$(MIP_DIR)/mipcl/headers
HDR_MIPSHELL=$(MIP_DIR)/mipshell/headers
#
VPATH=../sources
CFLAGS+=-I$(VPATH)
MOD= $(shell echo $(VPATH)/*.mod)
AUX=$(MOD:.mod=.aux)
SRC=$(notdir $(shell echo $(VPATH)/*.cpp))
OBJS=$(SRC:.cpp=.o)
#
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
%.aux : %.mod ;	$(MIP_DIR)/bin/mod2cpp $< $(<D)/$@
#
build: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)
#dependencies
genassign.cpp: genassign.mod 
genassign.o: genassign.cpp genassign.aux genassign.h
main.o: main.cpp genassign.h
#
.PHONY: clean
clean:
	$(RM) $(OBJS) $(AUX)
install: $(TARGET)
	$(INS) $(TARGET) $(INSDIR)
	$(RM) $(TARGET)
all: build install


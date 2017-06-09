CC=g++
ARC=-mfpmath=sse -msse2
CFLAGS=-c -g $(ARC) -I$(HDR_MIPSHELL) -I$(HDR_MIPCL) -D__THREADS_=""
LDFLAGS=-shared-libgcc
RM=rm -f
INS=install
INSDIR=../bin
MIP_DIR=$(MIPCLDIR)
CFLAGS+=-DMIPSHELL_API="" -DMIP_API=""
LDFLAGS+= -L$(MIP_DIR)/lib -lmipcl -lmipshell
TARGET=optportfolio_dbg
#
HDR_MIPCL=$(MIP_DIR)/mipcl/headers
HDR_MIPSHELL=$(MIP_DIR)/mipshell/headers
#
VPATH=../sources
CFLAGS+=-I$(VPATH)
MOD= $(shell echo $(VPATH)/*.mod)
XYZ=$(MOD:.mod=.aux)
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
optportfolio.cpp: optportfolio.mod 
optportfolio.o: optportfolio.cpp optportfolio.aux optportfolio.h
main.o: main.cpp optportfolio.h
#
.PHONY: clean
clean:
	$(RM) $(OBJS) $(XYZ)
install: $(TARGET)
	$(INS) $(TARGET) $(INSDIR)
	$(RM) $(TARGET)
all: build install


CC=g++
ARC=-mfpmath=sse -msse2
CFLAGS=-c -O3 $(ARC) -I$(HDR_MIPSHELL) -I$(HDR_MIPCL) -D__THREADS_=""
LDFLAGS=-shared-libgcc
INSDIR=../bin
INS=install
MIP_DIR=$(MIPCLDIR)
RM=rm -f
CFLAGS+=-DMIPSHELL_API="" -DMIP_API=""
LDFLAGS+= -L$(MIP_DIR)/lib -lmipcl -lmipshell
TARGET=fcnf
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
.PHONY: clean
clean:
	$(RM) $(OBJS) $(AUX)
install: $(TARGET)
	$(INS) $(TARGET) $(INSDIR)
	$(RM) $(TARGET)
all: build install


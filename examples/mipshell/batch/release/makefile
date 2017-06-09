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
TARGET=batch
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
batch.cpp: batch.mod 
batch.o: batch.cpp batch.aux batch.h
main.o: main.cpp batch.h
#
.PHONY: clean
clean:
	$(RM) $(OBJS) $(AUX)
install: $(TARGET)
	$(INS) $(TARGET) $(INSDIR)
	$(RM) $(TARGET)
all: build install


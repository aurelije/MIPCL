CC=g++
ARC=-m64 -mfpmath=sse -msse2
CFLAGS=-c -O3 -I$(HDR_PATH) -I$(HDR_MIPCL)  $(ARC) -minline-all-stringops -D__THREADS_
LDFLAGS=-shared -shared-libgcc
#
INS=install
RM=rm -f
MIP_DIR=$(MIPCLDIR)
LDFLAGS+= -L$(MIP_DIR)/lib -lmipcl
CFLAGS+= -fpic
LINK_TARGET=libmipshell.so
TARGET=$(LINK_TARGET).$(MIPSHELL_VERSION)
#####
PRJ_DIR=$(MIP_DIR)/mipshell
INSDIR=$(MIP_DIR)/lib
######
SRC_PATH=$(PRJ_DIR)/sources
HDR_PATH=$(PRJ_DIR)/headers
HDR_MIPCL=$(MIP_DIR)/mipcl/headers
#
vpath %.h $(HDR_PATH)
vpath %.cpp $(SRC_PATH)
#
SRC=Array.cpp Var.cpp Ctr.cpp DVar.cpp ellstr.cpp Function.cpp Index.cpp \
	misc.cpp Problem.cpp Set.cpp VarArray.cpp Vector.cpp
HDR=Array.Var.h h Ctr.h DVar.h ellstr.h Function.h Index.h \
	mipshell.h Problem.h Set.h VarArray.h Vector.h
OBJS=$(SRC:.cpp=.o)
.cpp.o:
	$(CC) $(CFLAGS)  $<
build: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)
#dependencies
Array.o: Array.cpp Array.h Set.h Index.h
Ctr.o: Ctr.cpp Var.h Ctr.h Problem.h
DVar.o: DVar.cpp Var.h Vector.h Set.h Index.h Problem.h DVar.h
ellstr.o: ellstr.cpp Set.h Index.h
Function.o: Function.cpp Var.h Problem.h Function.h
Index.o: Index.cpp Index.h
Problem.o: Problem.cpp Var.h Ctr.h Vector.h Set.h Index.h Function.h DVar.h Problem.h
Set.o: Set.cpp Set.h Index.h
suppl.o: suppl.cpp
VarArray.o: VarArray.cpp Set.h Index.h VarArray.h Var.h
Var.o: Var.cpp Ctr.h Var.h Problem.h
Vector.o: Vector.cpp Vector.h
.PHONY: clean
clean:
	rm -f $(OBJS)
#
install: $(TARGET)
	$(INS) $(TARGET) $(INSDIR)
	$(RM) $(INSDIR)/$(LINK_TARGET)
	ln -s $(INSDIR)/$(TARGET) $(INSDIR)/$(LINK_TARGET)
	$(RM) $(TARGET)
all: build install
.PHONY: test
test:
	echo $(HDR_MIPCL)

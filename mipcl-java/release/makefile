CC=g++
INS=install
RM=rm -f
JHOME=/usr/lib/jvm/java
MIP_DIR=$(MIPCLDIR)
ARC=-m64 -mfpmath=sse -msse2
CFLAGS=-c -fpic -std=c++17 -fno-rtti -O3 -I$(HDR_PATH) $(ARC) -minline-all-stringops -D__THREADS_
CFLAGS+=-I$(MIP_DIR)/mipcl/headers -I$(JHOME)/include -I$(JHOME)/include/linux
LDFLAGS=-shared -L$(MIPCLDIR)/lib -lmipcl -shared-libgcc
LINK_TARGET=libJVmipcl.so
TARGET=$(LINK_TARGET).$(MIPCL_VERSION)
INSDIR=$(MIP_DIR)/lib
MIPCL_JAVA_DIR=$(MIP_DIR)/mipcl-java
SRC_PATH=$(MIPCL_JAVA_DIR)/mipcl-jni
HDR_PATH=$(MIPCL_JAVA_DIR)/mipcl-jni
JSRC_PATH=$(MIPCL_JAVA_DIR)/sources/by/bsu/JVmipcl
CLASS_PATH=$(MIPCL_JAVA_DIR)/classes
vpath %.h $(HDR_PATH)
vpath %.cpp $(SRC_PATH)
vpath %.java $(JSRC_PATH)
#vpath %.o $(INTERMED_PATH)
#
#SRC=$(notdir $(wildcard $(SRC_PATH)/*.cpp))
#HDR=$(notdir $(wildcard $(HDR_PATH)/*.h))
JSRC=LP.java MIP.java
JCLS=LP.class MIP.class
CSRC=lp_jni.cpp mip_jni.cpp
#HRD=$(SRC:.cpp=.h)
OBJS=$(CSRC:.cpp=.o)
.cpp.o:
	$(CC) $(CFLAGS)  $<
%.class : %.java
	javac -cp $(CLASS_PATH) $< -d $(CLASS_PATH)
by_bsu_JVmipcl_%.h : %.class
	javah -jni -d $(HDR_PATH) -cp $(CLASS_PATH) by.bsu.JVmipcl.$*
#
build: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)
# java dependencies
LP.class: LP.java
MIP.class: MIP.java LP.java
by_bsu_JVmipcl_LP.h: LP.class
by_bsu_JVmipcl_MIP.h: MIP.class
# cpp dependencies
lp_jni.o: misc.h lp_jni.cpp by_bsu_JVmipcl_LP.h 
mip_jni.o: misc.h mip_jni.cpp by_bsu_JVmipcl_MIP.h
.PHONY: clean
clean:
	$(RM) $(OBJS)	
#
install: $(TARGET)
	$(INS) $(TARGET) $(INSDIR)
	$(RM) $(INSDIR)/$(LINK_TARGET)
	ln -s $(INSDIR)/$(TARGET) $(INSDIR)/$(LINK_TARGET)
	$(RM) $(TARGET)
	jar cf $(MIPCL_JAVA_DIR)/mipcl.jar -C $(CLASS_PATH) by	
#
all: build install
test:


APP_CLASS=fcnf
MAIN_CLASS=testFcnf
MIPCL_JAVA_DIR=$(MIPCLDIR)/mipcl-java
EXAMPLES_DIR=$(MIPCL_JAVA_DIR)/examples
TARGET=$(EXAMPLES_DIR)/$(APP_CLASS)/$(APP_CLASS).jar
CLASS_PATH=$(BIN_PATH):$(MIPCL_JAVA_DIR)/mipcl.jar
SRC_PATH=$(EXAMPLES_DIR)/$(APP_CLASS)/src/$(APP_CLASS)
BIN_PATH=$(EXAMPLES_DIR)/$(APP_CLASS)/bin
#SRC=$(wildcard $(SRC_PATH)/*.java)
SRC=fcnf.java testFcnf.java
CLS=$(SRC:.java=.class)
vpath %.java $(SRC_PATH)
#
%.class : %.java
	javac -cp $(CLASS_PATH) $< -d $(BIN_PATH)
build: $(CLS)
#	cd $(EXAMPLES_DIR)/$(APP_CLASS)/bin
#	jar cfe $(TARGET) $(APP_CLASS) $(APP_CLASS)/$(MAIN_CLASS).class
	jar cfm $(TARGET) META-INF/MANIFEST.MF -C $(EXAMPLES_DIR)/$(APP_CLASS)/bin fcnf
# java dependencies
$(APP_CLASS).class: $(APP_CLASS).java
$(MAIN_CLASS).class: $(MAIN_CLASS).java
run: $(TARGET)
	java -cp $(MIPCL_JAVA_DIR)/mipcl.jar -jar $(APP_CLASS).jar  $(test)
.PHONY: xxxx
xxxx:
	echo $(SRC_PATH)


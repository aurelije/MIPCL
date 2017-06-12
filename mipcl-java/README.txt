Welcome to MIPCL-JAVA
=======================

MIPCL-JAVA is a module that implements the Java Native Interface (JNI) to the MIPCL.
MIPCL-JAVA implements only a part of the MIPCL API.
Complex applications requiring redefining MIPCL virtual functions
--- say, those needed for generating problem specific cuts ---
cannot be implemented using MIPCL-JAVA.
Efficiency issues are the main reason for such restrictions. 

MIPCL-JAVA comprises of two packages: by.bsu.JVmipcl and by.bsu.JVmipshell.
The former contains two classes --- LP and MIP --- that are wrappers, respectively,
for the CLP and CMIP, which are MIPCL main classes.
You can use LP and MIP for modeling simply structured LPs and MIPs.
But for more complex (from modeling point of view) optimization problems,
it is preferable to use two classes --- LPshell and MIPshell --- from `by.bsu.JVmipshell`.

To run some MIPCL_JAVA examples, you need to have the google gson library on your computer.
Just copy this library to
$MIPCLDIR/mipcl-java/examples/gson.jar  in Linux (or make a soft link),
or to   %MIPCLDIR%\mipcl-java\examples\gson.jar  in Windows.

MIPCL-JAVA is a part of the MIPCL installation bundle.


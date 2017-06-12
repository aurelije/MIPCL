set "MIPCL_JAVA_DIR=%MIPCLDIR%\mipcl-java"
javac -cp %MIPCL_JAVA_DIR%\mipcl.jar;%MIPCL_JAVA_DIR%\examples\gson.jar %MIPCL_JAVA_DIR%\examples\projectScheduling\src\projectScheduling\*.java -d %MIPCL_JAVA_DIR%\examples\projectScheduling\bin

jar cfm %MIPCL_JAVA_DIR%\examples\projectScheduling\projectScheduling.jar manifest.fm -C %MIPCL_JAVA_DIR%\examples\projectScheduling\bin projectScheduling


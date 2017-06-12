set "MIPCL_JAVA_DIR=%MIPCLDIR%\mipcl-java"
javac -cp %MIPCL_JAVA_DIR%\mipcl.jar;%MIPCL_JAVA_DIR%\examples\gson.jar %MIPCL_JAVA_DIR%\examples\fcnf\src\fcnf\*.java -d %MIPCL_JAVA_DIR%\examples\fcnf\bin

jar cfm %MIPCL_JAVA_DIR%\examples\fcnf\fcnf.jar manifest.fm -C %MIPCL_JAVA_DIR%\examples\fcnf\bin fcnf


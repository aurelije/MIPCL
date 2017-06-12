set "MIPCL_JAVA_DIR=%MIPCLDIR%\mipcl-java"
javac -cp %MIPCL_JAVA_DIR%\mipcl.jar;%MIPCL_JAVA_DIR%\examples\gson.jar %MIPCL_JAVA_DIR%\examples\scheduling\src\scheduling\*.java -d %MIPCL_JAVA_DIR%\examples\scheduling\bin

jar cfm %MIPCL_JAVA_DIR%\examples\scheduling\scheduling.jar manifest.fm -C %MIPCL_JAVA_DIR%\examples\scheduling\bin scheduling


set "MIPCL_JAVA_DIR=%MIPCLDIR%\mipcl-java"
javac -cp %MIPCL_JAVA_DIR%\mipcl.jar;%MIPCL_JAVA_DIR%\examples\gson.jar %MIPCL_JAVA_DIR%\examples\multlotsizing\src\multlotsizing\*.java -d %MIPCL_JAVA_DIR%\examples\multlotsizing\bin

jar cfm %MIPCL_JAVA_DIR%\examples\multlotsizing\multlotsizing.jar manifest.fm -C %MIPCL_JAVA_DIR%\examples\multlotsizing\bin multlotsizing


SET ARCH=Visual Studio 12
IF [%1] == [64] (
	SET ARCH=Visual Studio 12 Win64
)
IF [%1] == [32] (
	SET ARCH=Visual Studio 12
)

call build-common.bat

Set ARCHIVE=glog-rev188.zip
Set BUILD_DIR=%TMP_DIR%/glog-master
Set PREFIX=%DEP_DIR%/glog

echo "Compile GLog"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

if not exist %BUILD_DIR% exit /b

echo "Building GLog"
cd %BUILD_DIR%

"%CMake3%"\cmake -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%ARCH%" .

MSBuild.exe google-glog.sln /p:Configuration=Release > NUL

echo "Installing GLog"

MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
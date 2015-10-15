SET ARCH=Visual Studio 12
IF [%1] == [64] (
	SET ARCH=Visual Studio 12 Win64
)
IF [%1] == [32] (
	SET ARCH=Visual Studio 12
)

call build-common.bat

Set ARCHIVE=gflags-2.1.2.zip
Set BUILD_DIR=%TMP_DIR%/gflags-2.1.2
Set PREFIX=%DEP_DIR%/gflags

echo "Compile GFlags"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Building GFlags"
cd %BUILD_DIR%

cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%ARCH%" .

MSBuild.exe gflags.sln /p:Configuration=Release > NUL

echo "Installing GFlags"

MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
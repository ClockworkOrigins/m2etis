call build-common.bat %1 %2

Set ARCHIVE=glog-rev188.zip
Set BUILD_DIR=%TMP_DIR%/glog-master
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/glog

echo "Compile GLog"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

if not exist %BUILD_DIR% exit /b

echo "Building GLog"
cd %BUILD_DIR%

"%CMake3%"\cmake -DCMAKE_INSTALL_PREFIX=%PREFIX% -DBUILD_SHARED_LIBS=ON -G "%VSCOMPILER%%VSARCH%" .

MSBuild.exe google-glog.sln /p:Configuration=Release > NUL

echo "Installing GLog"

MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
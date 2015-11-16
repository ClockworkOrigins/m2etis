call build-common.bat %1 %2

Set ARCHIVE=gflags-2.1.2.zip
Set BUILD_DIR=%TMP_DIR%/gflags-2.1.2
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/gflags

echo "Compile GFlags"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Building GFlags"
cd %BUILD_DIR%

cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%VSCOMPILER%%VSARCH%" .

MSBuild.exe gflags.sln /p:Configuration=Release > NUL

echo "Installing GFlags"

MSBuild.exe INSTALL.vcxproj /p:Configuration=Release > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
call build-common.bat

Set ARCHIVE=gflags-2.0-no-svn-files.tar
Set BUILD_DIR=%TMP_DIR%/gflags-2.0
Set PREFIX=%DEP_DIR%/gflags

echo "Compile GFlags"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Patching GFlags"

cd %DEP_DIR%

call download-dependency.bat gflags_2.0_patch_pre.zip

cd %TMP_DIR%

winrar.exe x gflags_2.0_patch_pre.zip

echo "Building GFlags"
cd %BUILD_DIR%

for %%X in (devenv.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND ( xcopy /S /Y "%TMP_DIR%/gflags_sln" "%BUILD_DIR%" ) else ( devenv gflags.sln /upgrade )

MSBuild.exe gflags.sln /p:Configuration=Release > NUL

echo "Installing GFlags"
mkdir "%PREFIX%"
mkdir "%PREFIX%/include"
mkdir "%PREFIX%/lib"
mkdir "%PREFIX%/include/gflags"
xcopy /S /Y "%BUILD_DIR%/src" "%PREFIX%/include/gflags" > NUL
xcopy /S /Y "%BUILD_DIR%/src/windows" "%PREFIX%/include/gflags" > NUL
xcopy /S /Y "%BUILD_DIR%/src/windows/gflags" "%PREFIX%/include/gflags" > NUL

xcopy /S /Y "%BUILD_DIR%/Release" "%PREFIX%/lib" > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
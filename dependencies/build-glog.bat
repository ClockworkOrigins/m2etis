call build-common.bat

Set ARCHIVE=glog-0.3.3.tar.gz
Set BUILD_DIR=%TMP_DIR%/glog-0.3.3
Set PREFIX=%DEP_DIR%/glog

echo "Compile GLog"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %DEP_DIR%

call download-dependency.bat glog_0.3.3_patch_pre1.zip
call download-dependency.bat glog_0.3.3_patch_pre2.zip

cd %TMP_DIR%

winrar.exe x glog_0.3.3_patch_pre1.zip
winrar.exe x glog_0.3.3_patch_pre2.zip

if not exist %BUILD_DIR% exit /b

echo "Patching GLog"
xcopy /S /Y "%TMP_DIR%/glog/src" "%BUILD_DIR%/src"

echo "Building GLog"
cd %BUILD_DIR%

for %%X in (devenv.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND ( xcopy /S /Y "%TMP_DIR%/glog_sln" "%BUILD_DIR%" ) else ( devenv google-glog.sln /upgrade )

MSBuild.exe google-glog.sln /p:Configuration=Release > NUL

echo "Installing GLog"
mkdir "%PREFIX%"
mkdir "%PREFIX%/lib"
mkdir "%PREFIX%/include"
mkdir "%PREFIX%/include/glog"

xcopy /S /Y "%BUILD_DIR%/src" "%PREFIX%/include/glog" > NUL
xcopy /S /Y "%BUILD_DIR%/src/windows" "%PREFIX%/include/glog" > NUL
xcopy /S /Y "%BUILD_DIR%/src/windows/glog" "%PREFIX%/include/glog" > NUL
xcopy /S /Y "%BUILD_DIR%/Release" "%PREFIX%/lib" > NUL

RD /S /Q "%PREFIX%/include/glog/windows"

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
RD /S /Q "%TMP_DIR%"
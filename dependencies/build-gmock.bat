@echo OFF

REM
REM Copyright (2016) Michael Baer, Daniel Bonrath, All rights reserved.
REM 
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM 
REM http://www.apache.org/licenses/LICENSE-2.0
REM 
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.
REM

call build-common.bat %1 %2

Set ARCHIVE=gmock-1.7.0.zip
Set BUILD_DIR=%TMP_DIR%/gmock-1.7.0
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/gmock

IF EXIST %PREFIX% EXIT /B

echo "Compile GoogleMock with GoogleTest"

echo "Extracting GoogleMock with GoogleTest"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Configuring GoogleMock with GoogleTest"

cd %BUILD_DIR%
cmake -DCMAKE_INSTALL_PREFIX=%PREFIX% -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -G "%VSCOMPILER%%VSARCH%" .

echo "Building GoogleMock with GoogleTest"

MSBuild.exe gmock.sln /p:Configuration=Release

echo "Installing GoogleMock with GoogleTest"

mkdir "%PREFIX%"
mkdir "%PREFIX%/include"
mkdir "%PREFIX%/lib"
xcopy /S /Y "%BUILD_DIR%/gtest/include" "%PREFIX%/include" > NUL
xcopy /S /Y "%BUILD_DIR%/include" "%PREFIX%/include" > NUL
xcopy /S /Y "%BUILD_DIR%/gtest/Release" "%PREFIX%/lib" > NUL
xcopy /S /Y "%BUILD_DIR%/Release" "%PREFIX%/lib" > NUL

echo "Cleaning up"

cd %DEP_DIR%
RD /S /Q "%TMP_DIR%"


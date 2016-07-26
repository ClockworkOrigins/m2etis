@echo OFF

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

call build-common.bat %1 %2

Set ARCHIVE=gflags-2.1.2.zip
Set BUILD_DIR=%TMP_DIR%/gflags-2.1.2
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/gflags

IF EXIST %PREFIX% EXIT /B

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
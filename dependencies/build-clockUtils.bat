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

Set ARCHIVE=clockutils-1.1.0-src.zip
Set BUILD_DIR=%TMP_DIR%/clockutils-1.1.0-src
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/clockUtils

IF EXIST %PREFIX% EXIT /B

echo "Compile clockUtils"

echo "Extracting clockUtils"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR% http://clockwork-origins.de/clockUtils/downloads/

echo "Configuring clockUtils"

cd %BUILD_DIR%
cmake -DWITH_TESTING=OFF -DWITH_LIBRARY_ARGPARSER=OFF  -DWITH_LIBRARY_COMPRESSION=OFF  -DWITH_LIBRARY_CONTAINER=OFF  -DWITH_LIBRARY_INIPARSER=OFF  -DWITH_LIBRARY_SOCKETS=ON  -DCLOCKUTILS_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX=%PREFIX% -G "%VSCOMPILER%%VSARCH%" .

echo "Building clockUtils"

MSBuild.exe clockUtils.sln /m:%NUMBER_OF_PROCESSORS% /p:Configuration=Release

echo "Installing clockUtils"

MSBuild.exe INSTALL.vcxproj /p:Configuration=Release

echo "Cleaning up"

cd %DEP_DIR%
RD /S /Q "%TMP_DIR%"


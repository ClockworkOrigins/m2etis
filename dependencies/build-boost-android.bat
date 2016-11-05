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

call build-common.bat android

Set ARCHIVE=boost_1_58_0.tar.bz2
Set BUILD_DIR=%TMP_DIR%/boost_1_58_0
Set PREFIX=%DEP_DIR%/%ARCH_DIR%/boost

IF EXIST %PREFIX% EXIT /B

echo "Compile Boost"

echo "Extracting Boost"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

echo "Configuring Boost"

cd %BUILD_DIR%
xcopy /F "%DEP_DIR%\..\ext\patches\boost\user-config-AndroidWindows.jam" "%BUILD_DIR%\tools\build\src\user-config.jam*"
call bootstrap.bat

echo "Building Boost"
b2 toolset=gcc-android target-os=linux --with-serialization --with-system link=static threading=multi --layout=system -j %NUMBER_OF_PROCESSORS% variant=release install --prefix=%PREFIX% stage > NUL

echo "Installing Boost"

echo #define BOOST_ALL_NO_LIB >> "%PREFIX%\include\boost\config\user.hpp"
echo "Cleaning up"

cd %DEP_DIR%
RD /S /Q "%TMP_DIR%"


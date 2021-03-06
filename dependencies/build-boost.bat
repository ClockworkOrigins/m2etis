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

Set ARCHIVE=boost_1_60_0.tar.bz2
Set BUILD_DIR=%TMP_DIR%/boost_1_60_0

Set PREFIX=%DEP_DIR%/%ARCH_DIR%/boost/
Set DEBUG_FLAG="variant=debug"
Set RELEASE_FLAG="variant=release"

Set BUILD_TYPE=%RELEASE_FLAG%

echo "Compile Boost"

call build-common.bat downloadAndUnpack %ARCHIVE% %BUILD_DIR%

cd %DEP_DIR%

call build-common.bat downloadAndUnpack boost_1_58_patch_post.zip %TMP_DIR%

if not exist %BUILD_DIR% exit /b
cd %BUILD_DIR%

echo "Bootstrapping Boost"

if not exist bootstrap.bat exit /b

call bootstrap.bat

if not exist b2.exe exit /b

b2 toolset=%BOOSTCOMPILER% address-model=%BOOSTARCH% --with-chrono --with-date_time --with-filesystem --with-regex --with-serialization --with-system --with-thread link=shared threading=multi --layout=system variant=release install --prefix=%PREFIX% stage > NUL

echo "Cleaning up"
cd %DEP_DIR%
RD /S /Q "%BUILD_DIR%"
xcopy /I /S /Y "%TMP_DIR%\boost_post" "%PREFIX%\include\boost\config"

RD /S /Q "%TMP_DIR%"

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

SET DEP_DIR=%cd%
SET TMP_DIR=%cd%\tmp

IF "%1" == "downloadAndUnpack" (goto downloadAndUnpack)

IF [%1] == [msvc13] (
	SET VSCOMPILER=Visual Studio 12
	SET BOOSTCOMPILER=msvc-12.0
	SET ARCH_DIR=msvc13_
	SET CONFIG_BAT_PATH="%VS12%"
)
IF [%1] == [msvc15] (
	SET VSCOMPILER=Visual Studio 14
	SET BOOSTCOMPILER=msvc-14.0
	SET ARCH_DIR=msvc15_
	SET CONFIG_BAT_PATH="%VS15%"
)
IF [%1] == [android] (
	SET ARCH_DIR=android
)

IF [%2] == [32] (
	SET VSARCH=
	SET BOOSTARCH=32
	SET ARCH_DIR=%ARCH_DIR%32
	SET VSBATARCH=x86
)
IF [%2] == [64] (
	SET VSARCH= Win64
	SET BOOSTARCH=64
	SET ARCH_DIR=%ARCH_DIR%64
	SET VSBATARCH=amd64
)

IF [%CONFIG_BAT_PATH%] == [] EXIT /B

FOR %%X IN (MSBuild.exe) DO (SET FOUND=%%~$PATH:X)
IF NOT DEFINED FOUND (
	CALL %CONFIG_BAT_PATH%\vcvarsall.bat %VSBATARCH%
)

EXIT /B

:downloadAndUnpack

SET DOWNLOAD_URL=%4
IF [%4] == [] Set DOWNLOAD_URL=http://www.clockwork-origins.de/dependencies/
IF NOT EXIST %TMP_DIR% (mkdir %TMP_DIR%)
IF NOT EXIST %TMP_DIR%\%2 (bitsadmin /transfer "myDownloadJob%2" /download /priority normal %DOWNLOAD_URL%%2 %TMP_DIR%\%2)
CD %TMP_DIR%
IF EXIST %3 RD /S /Q "%2"
winrar.exe x -ibck %2
IF NOT EXIST %3 EXIT /B
EXIT /B 0


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

Set DEP_DIR=%cd%
Set TMP_DIR=%cd%\tmp
Set CONFIG_BAT_PATH="%VS12%"

IF "%1" == "downloadAndUnpack" (goto downloadAndUnpack)

SET VSCOMPILER=Visual Studio 12
SET VSARCH=
SET BOOSTCOMPILER=msvc-12.0
SET BOOSTARCH=32
SET ARCH_DIR=msvc12_x86
SET VSBATARCH=x86

IF [%1] == [msvc12] (
	SET VSCOMPILER=Visual Studio 12
	SET BOOSTCOMPILER=msvc-12.0
	SET ARCH_DIR=msvc12_
	Set CONFIG_BAT_PATH="%VS12%"
)
IF [%1] == [msvc14] (
	SET VSCOMPILER=Visual Studio 14
	SET BOOSTCOMPILER=msvc-14.0
	SET ARCH_DIR=msvc14_
	Set CONFIG_BAT_PATH="%VS14%"
)
IF [%1] == [android] (
	SET VSCOMPILER=
	SET BOOSTCOMPILER=
	SET ARCH_DIR=android
	Set CONFIG_BAT_PATH=
)
IF [%1] == [] (
	SET VSCOMPILER=Visual Studio 12
	SET BOOSTCOMPILER=msvc-12.0
	SET ARCH_DIR=msvc12_
	Set CONFIG_BAT_PATH="%VS12%"
)
IF [%2] == [32] (
	SET VSARCH=
	SET BOOSTARCH=32
	SET ARCH_DIR=%ARCH_DIR%x86
	SET VSBATARCH=x86
)
IF [%2] == [64] (
	SET VSARCH= Win64
	SET BOOSTARCH=64
	SET ARCH_DIR=%ARCH_DIR%x64
	SET VSBATARCH=amd64
)
IF [%2] == [] (
	SET VSARCH=
	SET BOOSTARCH=32
	IF NOT [%ARCH_DIR%] == [android] (
		SET ARCH_DIR=%ARCH_DIR%x86
	)
	SET VSBATARCH=x86
)

IF [%CONFIG_BAT_PATH%] == [] EXIT /B

call %CONFIG_BAT_PATH%\vcvarsall.bat %VSBATARCH%

EXIT /B

:downloadAndUnpack
Set DOWNLOAD_URL=%4
IF [%4] == [] Set DOWNLOAD_URL=http://www.clockwork-origins.de/dependencies/
IF not exist %TMP_DIR% (mkdir %TMP_DIR%)
IF not exist %TMP_DIR%\%2 (bitsadmin /transfer "myDownloadJob%2" /download /priority normal %DOWNLOAD_URL%%2 %TMP_DIR%\%2)
cd %TMP_DIR%
if exist %3 RD /S /Q "%2"
winrar.exe x %2
if not exist %3 exit /b
EXIT /B 0

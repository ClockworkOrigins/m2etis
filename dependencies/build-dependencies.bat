SET ARCH=32

IF [%1] == [64] (
	SET ARCH=64
)
IF [%1] == [] (
	SET ARCH=32
)

call build-common.bat

call build-boost.bat %ARCH%
call build-clockUtils.bat %ARCH%
call build-gflags.bat %ARCH%
call build-glog.bat %ARCH%
call build-gmock.bat %ARCH%
call build-gperftools.bat %ARCH%

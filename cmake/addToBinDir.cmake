##
# Copyright (2016) Michael Baer, Daniel Bonrath, All rights reserved.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##

IF(WIN32)
	file(GLOB_RECURSE files "${M2ETIS_DEP_DIR}/*.dll")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${filename} ${CMAKE_BINARY_DIR}/bin)
	endforeach()
ENDIF(WIN32)
IF(UNIX)
	file(GLOB_RECURSE files "${M2ETIS_DEP_DIR}/*.so")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filename} ${CMAKE_BINARY_DIR}/bin/${filename_pure})
	endforeach()
ENDIF(UNIX)

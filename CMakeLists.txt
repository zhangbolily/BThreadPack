# MIT License
# 
# Copyright (c) 2018 Ball Chang
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required (VERSION 2.8.12)
project (BThreadPack)

aux_source_directory (src DIR_SRCS)
include_directories ("${PROJECT_SOURCE_DIR}/include")
include_directories ("${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}")

# Build examples
add_subdirectory (examples)

set (EXTRA_LIBS ${EXTRA_LIBS} rt)
set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library (bthreadpack SHARED ${DIR_SRCS})
target_link_libraries (bthreadpack ${EXTRA_LIBS})

install (TARGETS bthreadpack DESTINATION lib)
install (
    FILES
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BAbstractClass.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BAbstractTask.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BAbstractThreadPool.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BGeneralTask.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BGeneralThreadPool.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BThreadPack.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BTimer.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BWorkerTask.h"
    DESTINATION include/${PROJECT_NAME}
)

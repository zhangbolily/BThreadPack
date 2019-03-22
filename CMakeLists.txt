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

aux_source_directory (src/BCore DIR_SRCS)
aux_source_directory (src/BUtils DIR_SRCS)
aux_source_directory (src/BThreadPack DIR_SRCS)
include_directories ("${PROJECT_SOURCE_DIR}/include")

# Set build environment
set (CMAKE_CXX_STANDARD_REQUIRED "11")

if (UNIX)
    add_definitions (-DUNIX)
endif (UNIX)

# Build examples
add_subdirectory (examples)

# Build deb package
# Package file name prefix
set (CPACK_PACKAGE_NAME ${PROJECT_NAME})
set (CPACK_DEBIAN_PACKAGE_NAME ${PROJECT_NAME})
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set (MAJOR_VERSION 0)
set (MINOR_VERSION 1)
set (PATCH_VERSION 2-alpha)
set (CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set (CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set (CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libstdc++6")
set (CPACK_PACKAGE_DESCRIPTION "High effeciency multi-thread task framework.")
set (CPACK_PACKAGE_CONTACT "zhangbolily@gmail.com")
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Ball Chang")
include(CPack)

set (EXTRA_LIBS ${EXTRA_LIBS} rt)
set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library (bthreadpack SHARED ${DIR_SRCS})
target_link_libraries (bthreadpack ${EXTRA_LIBS})

# Set install files
install (TARGETS bthreadpack DESTINATION lib)
install (
    FILES
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BAbstractClass.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BAbstractTask.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BAbstractThreadPool.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BGeneralTask.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BGeneralThreadPool.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BGroupTask.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BThreadPack.h"
    "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/BWorkerTask.h"
    DESTINATION include/${PROJECT_NAME}
)

install (
    FILES
    "${PROJECT_SOURCE_DIR}/include/BCore/BCore.h"
    "${PROJECT_SOURCE_DIR}/include/BCore/BDebug.h"
    DESTINATION include/BCore
)

install (
    FILES
    "${PROJECT_SOURCE_DIR}/include/BUtils/BTimer.h"
    "${PROJECT_SOURCE_DIR}/include/BUtils/BUtils.h"
    DESTINATION include/BUtils
)

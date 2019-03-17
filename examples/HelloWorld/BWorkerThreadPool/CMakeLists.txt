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
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
aux_source_directory (src DIR_EXAMPLES_HELLO_WORLD_BWT_SRCS)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS src/task_data.proto)
add_executable (WorkerThreadPool_HelloWorld ${DIR_EXAMPLES_HELLO_WORLD_BWT_SRCS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries (WorkerThreadPool_HelloWorld bthreadpack ${PROTOBUF_LIBRARIES} ${EXTRA_LIBS})


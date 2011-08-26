# COPYRIGHT AND PERMISSION NOTICE
# 
# Copyright (c) 2010, 2011 Arne Welzel, <arne.welzel@googlemail.com>
#               
# All rights reserved.
#               
# Permission to use, copy, modify, and distribute this software for any purpose
# with or without fee is hereby granted, provided that the above copyright
# notice and this permission notice appear in all copies.
#      
# THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN
# NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
# OR OTHER DEALINGS IN THE SOFTWARE.
#              
# Except as contained in this notice, the name of a copyright holder shall not
# be used in advertising or otherwise to promote the sale, use or other dealings
# in this Software without prior written authorization of the copyright holder.
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libifmap2c-examples)

add_definitions(-Wall -Wextra -Werror -O3)

SET (LIBIFMAP2C_INCLUDE_DIR "/usr/local/include")
SET (LIBIFMAP2C_LIBRARIES "/usr/local/lib/libifmap2c.so")

INCLUDE_DIRECTORIES(
	"common/"
	"${LIBIFMAP2C_INCLUDE_DIR}")

# common functionality as a static library
ADD_LIBRARY(common STATIC common/common.cpp)

ADD_EXECUTABLE(subTest subTest.cpp)
ADD_EXECUTABLE(pdp-dhcp-example pdp-dhcp-example.cpp)
ADD_EXECUTABLE(metadata-examples metadata-examples.cpp)
ADD_EXECUTABLE(test-close-conn test-close-conn.cpp)
ADD_EXECUTABLE(example-mapc1 wiki/example-mapc1.cpp)
ADD_EXECUTABLE(example-mapc2 wiki/example-mapc2.cpp)
ADD_EXECUTABLE(test-update-twice test-update-twice.cpp)
ADD_EXECUTABLE(test-notify-event-twice test-notify-event-twice.cpp)

TARGET_LINK_LIBRARIES(subTest ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(pdp-dhcp-example ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(metadata-examples ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-close-conn ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(example-mapc1 ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(example-mapc2 ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(test-update-twice ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(test-notify-event-twice ${LIBIFMAP2C_LIBRARIES})

add_subdirectory(utils)
add_subdirectory(tests)
add_subdirectory(perf)

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

find_package (Threads)

ADD_EXECUTABLE(test-identifiers test-identifiers.cpp)
ADD_EXECUTABLE(test-max-poll-result-size-change test-max-poll-result-size-change.cpp)
ADD_EXECUTABLE(test-notify test-notify.cpp)
ADD_EXECUTABLE(test-poll-endsession test-poll-endsession.cpp)
ADD_EXECUTABLE(test-poll-results-too-big test-poll-results-too-big.cpp)
ADD_EXECUTABLE(test-poll-twice test-poll-twice.cpp)
ADD_EXECUTABLE(test-search-result-too-big test-search-result-too-big.cpp)
ADD_EXECUTABLE(test-search-depth test-search-depth.cpp)
ADD_EXECUTABLE(test-single-multi test-single-multi.cpp)
ADD_EXECUTABLE(test-subscription-too-big test-subscription-too-big.cpp)
ADD_EXECUTABLE(test-add-subgraph test-add-subgraph.cpp)
ADD_EXECUTABLE(test-remove-subgraph test-remove-subgraph.cpp)
ADD_EXECUTABLE(test-sub-depth test-sub-depth.cpp)
ADD_EXECUTABLE(test-subgraph1 test-subgraph1.cpp)
ADD_EXECUTABLE(test-subgraph2 test-subgraph2.cpp)
ADD_EXECUTABLE(test-subgraph3 test-subgraph3.cpp)
ADD_EXECUTABLE(test-subgraph4 test-subgraph4.cpp)
ADD_EXECUTABLE(test-subgraph5 test-subgraph5.cpp)
ADD_EXECUTABLE(test-close-conn test-close-conn.cpp)

TARGET_LINK_LIBRARIES(test-identifiers ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-poll-endsession  ${LIBIFMAP2C_LIBRARIES} common ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(test-poll-twice ${LIBIFMAP2C_LIBRARIES} common ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(test-notify ${LIBIFMAP2C_LIBRARIES} common ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(test-poll-results-too-big ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-search-result-too-big ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-search-depth ${LIBIFMAP2C_LIBRARIES} common testcommon)
TARGET_LINK_LIBRARIES(test-subscription-too-big ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-single-multi ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-max-poll-result-size-change ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-add-subgraph ${LIBIFMAP2C_LIBRARIES} common testcommon)
TARGET_LINK_LIBRARIES(test-remove-subgraph ${LIBIFMAP2C_LIBRARIES} common testcommon)
TARGET_LINK_LIBRARIES(test-sub-depth ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-subgraph1 ${LIBIFMAP2C_LIBRARIES} common testcommon)
TARGET_LINK_LIBRARIES(test-subgraph2 ${LIBIFMAP2C_LIBRARIES} common testcommon)
TARGET_LINK_LIBRARIES(test-subgraph3 ${LIBIFMAP2C_LIBRARIES} common testcommon)
TARGET_LINK_LIBRARIES(test-subgraph4 ${LIBIFMAP2C_LIBRARIES} common testcommon)
TARGET_LINK_LIBRARIES(test-subgraph5 ${LIBIFMAP2C_LIBRARIES} common testcommon)
TARGET_LINK_LIBRARIES(test-close-conn ${LIBIFMAP2C_LIBRARIES} common
	testcommon)

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
ADD_EXECUTABLE(test-single-multi test-single-multi.cpp)
ADD_EXECUTABLE(test-subscription-too-big test-subscription-too-big.cpp)

TARGET_LINK_LIBRARIES(test-identifiers ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-poll-endsession  ${LIBIFMAP2C_LIBRARIES} common ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(test-poll-twice ${LIBIFMAP2C_LIBRARIES} common ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(test-notify ${LIBIFMAP2C_LIBRARIES} common ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(test-poll-results-too-big ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-search-result-too-big ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-subscription-too-big ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-single-multi ${LIBIFMAP2C_LIBRARIES} common)
TARGET_LINK_LIBRARIES(test-max-poll-result-size-change ${LIBIFMAP2C_LIBRARIES} common)

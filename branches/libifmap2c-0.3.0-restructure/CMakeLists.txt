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
PROJECT(ifmap2c)

add_definitions(-Wall -Wextra -Werror -O2)

FIND_PACKAGE(LibXml2)
FIND_PACKAGE(CURL)

INCLUDE_DIRECTORIES(
	${ifmap2c_SOURCE_DIR}
	${ifmap2c_SOURCE_DIR}/include
	"${LIBXML2_INCLUDE_DIR}"
	"${CURL_INCLUDE_DIRS}")

ADD_LIBRARY(${PROJECT_NAME} SHARED
	communication/xmlcommunication.cpp
	communication/lowlevelcurlcommunication.cpp
	ifmap/identifierhandlerdispatch.cpp
	ifmap/requesthandlerdispatch.cpp
	ifmap/baseidentifierhandler.cpp
	ifmap/baserequesthandler.cpp
	ifmap/channel.cpp
	ifmap/ssrc.cpp
	ifmap/arc.cpp
	ifmap/identifier.cpp
	ifmap/publishrequest.cpp
	ifmap/requests.cpp
	ifmap/searchparam.cpp
	ifmap/searchrequest.cpp
	ifmap/subscriberequest.cpp
	ifmap/metadata.cpp
	response/errorresult.cpp
	response/ifmaperror.cpp
	response/newsessionresult.cpp
	response/pollresult.cpp
	response/searchresult.cpp
	xml/basicxmlmarshalable.cpp
	xml/libxml2marshaller.cpp
	xml/libxml2unmarshaller.cpp
	xml/xmlmarshalable.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${LIBXML2_LIBRARIES}
	${CURL_LIBRARIES}
)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
	VERSION 0.3.0
	SOVERSION 0.3
)

INSTALL(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION lib
)

INSTALL(FILES 
	include/accessrequest.h
	include/arc.h
	include/basicxmlmarshalable.h
	include/communicationerror.h
	include/device.h
	include/endsessionrequest.h
	include/endsessionresult.h
	include/errorresult.h
	include/identifier.h
	include/identifiers.h
	include/identity.h
	include/ifmapchannel.h
	include/ifmaperror.h
	include/ifmaprequest.h
	include/ipaddress.h
	include/lowlevelcommunication.h
	include/macaddress.h
	include/metadata.h
	include/newsessionrequest.h
	include/newsessionresult.h
	include/payload.h
	include/pollrequest.h
	include/pollresult.h
	include/publishrequest.h
	include/purgepublisherrequest.h
	include/renewsessionrequest.h
	include/request.h
	include/requests.h
	include/responseparseerror.h
	include/result.h
	include/resultitem.h
	include/results.h
	include/searchparam.h
	include/searchrequest.h
	include/searchresult.h
	include/soap.h
	include/ssrc.h
	include/subscriberequest.h
	include/tcgifmapbase.h
	include/tcgifmapmeta.h
	include/typedefs.h
	include/xmlbinding.h
	include/xmlcommunication.h
	include/xmlmarshalable.h
	DESTINATION include/lib${PROJECT_NAME}
)

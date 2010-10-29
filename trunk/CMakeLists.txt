CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ifmap2c)

add_definitions(-Wall -Wextra -pedantic) 

FIND_PACKAGE(LibXml2)
FIND_PACKAGE(CURL)

INCLUDE_DIRECTORIES(
	include
	"${LIBXML2_INCLUDE_DIR}"
	"${CURL_INCLUDE_DIRS}")

ADD_LIBRARY(${PROJECT_NAME} SHARED
	communication/arc.cpp
	communication/communicationerror.cpp
	communication/ifmapcommunication.cpp
	communication/lowlevelcurlcommunication.cpp
	communication/ssrc.cpp
	identifier/accessrequest.cpp
	identifier/device.cpp
	identifier/identifier.cpp
	identifier/identifiers.cpp
	identifier/identifierwithad.cpp
	identifier/identity.cpp
	identifier/ipaddress.cpp
	identifier/macaddress.cpp
	metadata/metadata.cpp
	request/endsessionrequest.cpp
	request/newsessionrequest.cpp
	request/pollrequest.cpp
	request/publishdelete.cpp
	request/publishnotify.cpp
	request/publishrequest.cpp
	request/publishupdate.cpp
	request/purgepublisherrequest.cpp
	request/renewsessionrequest.cpp
	request/requests.cpp
	request/searchrequest.cpp
	request/subpublish.cpp
	request/subscribedelete.cpp
	request/subscriberequest.cpp
	request/subscribeupdate.cpp
	request/subsubscribe.cpp
	response/errorresulterror.cpp
	response/newsessionresult.cpp
	response/pollresult.cpp
	response/responseparser.cpp
	response/resultitem.cpp
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
	VERSION 0.1.0
	SOVERSION 0.1
)

ADD_EXECUTABLE(subTest examples/subTest.cpp)
ADD_EXECUTABLE(ip-mac examples/ip-mac.cpp)
ADD_EXECUTABLE(test-identifiers examples/test-identifiers.cpp)
ADD_EXECUTABLE(pdp-dhcp-example examples/pdp-dhcp-example.cpp)
ADD_EXECUTABLE(test-purge-publisher examples/test-purge-publisher.cpp)

TARGET_LINK_LIBRARIES(subTest ${PROJECT_NAME})
TARGET_LINK_LIBRARIES(ip-mac ${PROJECT_NAME})
TARGET_LINK_LIBRARIES(test-identifiers ${PROJECT_NAME})
TARGET_LINK_LIBRARIES(pdp-dhcp-example ${PROJECT_NAME})
TARGET_LINK_LIBRARIES(test-purge-publisher ${PROJECT_NAME})

INSTALL(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION lib
)
# :r !ls ../include
INSTALL(FILES 
	include/accessrequest.h
	include/arc.h
	include/basicxmlmarshalable.h
	include/communicationerror.h
	include/device.h
	include/endsessionrequest.h
	include/endsessionresult.h
	include/errorresulterror.h
	include/identifier.h
	include/identifiers.h
	include/identifierwithad.h
	include/identity.h
	include/ifmapcommunication.h
	include/ipaddress.h
	include/libxml2marshaller.h
	include/libxml2unmarshaller.h
	include/lowlevelcommunication.h
	include/lowlevelcurlcommunication.h
	include/macaddress.h
	include/metadata.h
	include/newsessionrequest.h
	include/newsessionresult.h
	include/payload.h
	include/pollrequest.h
	include/pollresult.h
	include/publishdelete.h
	include/publishnotify.h
	include/publishrequest.h
	include/publishupdate.h
	include/purgepublisherrequest.h
	include/renewsessionrequest.h
	include/requests.h
	include/responseparseerror.h
	include/responseparser.h
	include/responses.h
	include/resultitem.h
	include/searchrequest.h
	include/searchresult.h
	include/ssrc.h
	include/subpublish.h
	include/subscribedelete.h
	include/subscriberequest.h
	include/subscribeupdate.h
	include/subsubscribe.h
	include/tcgifmapbase.h
	include/tcgifmapmeta.h
	include/xmlmarshalable.h
	include/xmlmarshaller.h
	include/xmlunmarshaller.h
	DESTINATION include/lib${PROJECT_NAME}
)
	

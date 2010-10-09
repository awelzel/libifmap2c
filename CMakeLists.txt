CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ifmap2c)

add_definitions(-DLIBRARY_NAMESPACE=ifmap2c -Wall -Wextra -pedantic) 

FIND_PACKAGE(LibXml2)
FIND_PACKAGE(CURL)

INCLUDE_DIRECTORIES(
	.
	common
	xml
	request
	response
	identifier
	communication
	metadata
	"${LIBXML2_INCLUDE_DIR}"
	"${CURL_INCLUDE_DIRS}")

ADD_LIBRARY(${PROJECT_NAME} SHARED
	identifier/identifiers.cpp
	identifier/identifierwithad.cpp
	identifier/identifier.cpp
	identifier/device.cpp
	identifier/ipaddress.cpp
	identifier/macaddress.cpp
	identifier/accessrequest.cpp
	identifier/identity.cpp
	request/requests.cpp
	request/newsessionrequest.cpp
	request/endsessionrequest.cpp
	request/publishrequest.cpp
	request/subpublish.cpp
	request/publishupdate.cpp
	request/publishdelete.cpp
	request/publishnotify.cpp
	request/searchrequest.cpp
	request/subscriberequest.cpp
	request/subsubscribe.cpp
	request/subscribeupdate.cpp
	request/subscribedelete.cpp
	request/pollrequest.cpp
	request/renewsessionrequest.cpp
	response/errorresulterror.cpp
	response/newsessionresult.cpp
	response/searchresult.cpp
	response/responseparser.cpp
	response/resultitem.cpp
	response/searchresult.cpp
	response/pollresult.cpp
	xml/libxml2marshaller.cpp
	xml/libxml2unmarshaller.cpp
	xml/basicxmlmarshalable.cpp
	xml/xmlmarshalable.cpp
	communication/lowlevelcurlcommunication.cpp
	communication/ifmapcommunication.cpp
	communication/communicationerror.cpp
	communication/ssrc.cpp
	communication/arc.cpp
	metadata/metadata.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${LIBXML2_LIBRARIES}
	${CURL_LIBRARIES}
)

#ADD_EXECUTABLE(test_new examples/test_new.cpp)
ADD_EXECUTABLE(subTest examples/subTest.cpp)
ADD_EXECUTABLE(ip-mac examples/ip-mac.cpp)
#TARGET_LINK_LIBRARIES(test_new ${PROJECT_NAME})
TARGET_LINK_LIBRARIES(subTest ${PROJECT_NAME})
TARGET_LINK_LIBRARIES(ip-mac ${PROJECT_NAME})

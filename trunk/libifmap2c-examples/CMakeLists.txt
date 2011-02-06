CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(libifmap2c-examples)

add_definitions(-Wall -Wextra -pedantic) 

SET (LIBIFMAP2C_INCLUDE_DIR "/usr/local/include")
SET (LIBIFMAP2C_LIBRARIES "/usr/local/lib/libifmap2c.so")

#FIND_PACKAGE(LibXml2)
#FIND_PACKAGE(CURL)

INCLUDE_DIRECTORIES(
	#	include
	#"${LIBXML2_INCLUDE_DIR}"
	#"${CURL_INCLUDE_DIRS}"
	"${LIBIFMAP2C_INCLUDE_DIR}")

ADD_EXECUTABLE(subTest subTest.cpp)
ADD_EXECUTABLE(ip-mac ip-mac.cpp)
ADD_EXECUTABLE(test-identifiers test-identifiers.cpp)
ADD_EXECUTABLE(pdp-dhcp-example pdp-dhcp-example.cpp)
ADD_EXECUTABLE(test-purge-publisher test-purge-publisher.cpp)
ADD_EXECUTABLE(metadata-examples metadata-examples.cpp)
ADD_EXECUTABLE(single-multi-test single-multi-test.cpp)
ADD_EXECUTABLE(example-mapc1 wiki/example-mapc1.cpp)
ADD_EXECUTABLE(example-mapc2 wiki/example-mapc2.cpp)

TARGET_LINK_LIBRARIES(subTest ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(ip-mac ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(test-identifiers ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(pdp-dhcp-example ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(test-purge-publisher ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(metadata-examples ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(single-multi-test ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(example-mapc1 ${LIBIFMAP2C_LIBRARIES})
TARGET_LINK_LIBRARIES(example-mapc2 ${LIBIFMAP2C_LIBRARIES})

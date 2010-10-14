/*
 * COPYRIGHT AND PERMISSION NOTICE
 * 
 * Copyright (c) 2010, Arne Welzel, <arne.welzel@googlemail.com>
 * 		 
 * All rights reserved.
 * 		 
 * Permission to use, copy, modify, and distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright
 * notice and this permission notice appear in all copies.
 * 	 
 * THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 * 		 
 * Except as contained in this notice, the name of a copyright holder shall not
 * be used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization of the copyright holder.
 */

#ifndef TCGIFMAPBASE_H_
#define TCGIFMAPBASE_H_

#define USE_SOAP_12

#ifdef USE_SOAP_12
#define SOAP_HREF			"http://www.w3.org/2003/05/soap-envelope"
#else
#define SOAP_HREF			"http://schemas.xmlsoap.org/soap/envelope/"
#endif

#define EMPTY_VALUE			""
#define NO_PREFIX			EMPTY_VALUE
#define NO_HREF				EMPTY_VALUE

#define SOAP_PREFIX 			"env"
#define SOAP_NSPAIR 			std::pair<std::string, std::string>(SOAP_PREFIX, SOAP_HREF)

#define IFMAP_PREFIX			"ifmap"
#define IFMAP_HREF 			"http://www.trustedcomputinggroup.org/2010/IFMAP/2"
#define IFMAP_NSPAIR 			std::pair<std::string, std::string>(IFMAP_PREFIX, IFMAP_HREF)

#define NO_NSPAIR			std::pair<std::string, std::string>(NO_PREFIX, NO_HREF)

#define IFMAP_OPERATION_NSPAIR		IFMAP_NSPAIR


#define ENVELOPE_ELEMENT_NAME		"Envelope"
#define ENVELOPE_ELEMENT_HREF		SOAP_HREF

#define BODY_ELEMENT_NAME		"Body"
#define BODY_ELEMENT_HREF		SOAP_HREF

#define RESPONSE_ELEMENT_NAME		"response"
#define RESPONSE_ELEMENT_HREF		IFMAP_HREF


#define ERRORRESULT_ELEMENT_NAME	"errorResult"
#define ERRORRESULT_ELEMENT_HREF	NO_HREF
#define ERRORCODE_ATTR_NAME		"errorCode"
#define ERRORSTRING_ELEMENT_NAME	"errorString"
#define ERRORSTRING_ELEMENT_HREF	NO_HREF

#define NEWSESSION_ELEMENT_NAME		"newSession"
#define NEWSESSION_ELEMENT_HREF		NO_HREF

#define NEWSESSIONRESULT_ELEMENT_NAME	"newSessionResult"
#define NEWSESSIONRESULT_ELEMENT_HREF	NO_HREF
#define SESSIONID_ATTR_NAME		"session-id"
#define PUBLISHERID_ATTR_NAME		"ifmap-publisher-id"
#define MAX_POLL_RES_SIZE_ATTR_NAME	"max-poll-result-size"


#define PUBLISH_ELEMENT_NAME		"publish"

#define PUBLISH_UPDATE_ELEMENT_NAME	"update"
#define PUBLISH_UPDATE_ELEMENT_HREF	NO_HREF
#define PUBLISH_DELETE_ELEMENT_NAME	"delete"
#define PUBLISH_DELETE_ELEMENT_HREF	NO_HREF
#define PUBLISH_DELETE_FILTER_ATTR_NAME	"filter"

#define PUBLISH_NOTIFY_ELEMENT_NAME	"notify"
#define PUBLISH_NOTIFY_ELEMENT_HREF	NO_HREF
#define PUBLISH_UPDATE_LIFET_ATTR_NAME	"lifetime"

#define METADATA_LIST_ELEMENT_NAME	"metadata"
#define METADATA_LIST_ELEMENT_HREF	NO_HREF


#define PUBLISHRECV_ELEMENT_NAME	"publishReceived"
#define PUBLISHRECV_ELEMENT_HREF	NO_HREF

#define SEARCH_ELEMENT_NAME		"search"
#define SEARCH_ELEMENT_HREF		NO_HREF
#define SEARCH_MATCH_LINKS_ATTR_NAME	"match-links"
#define SEARCH_MAX_DEPTH_ATTR_NAME	"max-depth"
#define SEARCH_TERM_IDENT_ATTR_NAME	"terminal-identifier-type"
#define SEARCH_MAX_SIZE_ATTR_NAME	"max-size"
#define SEARCH_RESULT_FILTER_ATTR_NAME	"result-filter"
#define SEARCH_NO_MAX_DEPTH		-1
#define SEARCH_NO_MAX_RESULT_SIZE	-1

#define SEARCHRESULT_ELEMENT_NAME	"searchResult"
#define SEARCHRESULT_ELEMENT_HREF	NO_HREF
#define SEARCHRESULT_NAME_ATTR_NAME	"name"

#define SUBSCRIBE_ELEMENT_NAME		"subscribe"
#define SUBSCRIBE_ELEMENT_HREF		NO_HREF
#define SUBSCRIBE_UPDATE_ELEMENT_NAME	"update"
#define SUBSCRIBE_UPDATE_ELEMENT_HREF	NO_HREF
#define SUBSCRIBE_UPDATE_NAME_ATTR_NAME	"name"
#define SUBSCRIBE_DELETE_ELEMENT_NAME	"delete"
#define SUBSCRIBE_DELETE_ELEMENT_HREF	NO_HREF

#define SUBSCRIBERECV_ELEMENT_NAME	"subscribeReceived"
#define SUBSCRIBERECV_ELEMENT_HREF	NO_HREF


#define POLLREQUEST_ELEMENT_NAME	"poll"
#define POLLRESULT_ELEMENT_NAME		"pollResult"
#define POLLRESULT_ELEMENT_HREF		NO_HREF
#define POLLRESULT_SEARCH_ELEMENT_NAME	"searchResult"
#define POLLRESULT_SEARCH_ELEMENT_HREF	NO_HREF
#define POLLRESULT_UPDATE_ELEMENT_NAME	"updateResult"
#define POLLRESULT_UPDATE_ELEMENT_HREF	NO_HREF
#define POLLRESULT_DELETE_ELEMENT_NAME	"deleteResult"
#define POLLRESULT_DELETE_ELEMENT_HREF	NO_HREF
#define POLLRESULT_NOTIFY_ELEMENT_NAME	"deleteResult"
#define POLLRESULT_NOTIFY_ELEMENT_HREF	NO_HREF
#define POLLRESULT_ERROR_ELEMENT_NAME	ERRORRESULT_ELEMENT_NAME
#define POLLRESULT_ERROR_ELEMENT_HREF	ERRORRESULT_ELEMENT_HREF

#define RENEWSESSION_ELEMENT_NAME	"renewSession"
#define RENEWSESSIONRESULT_ELEMENT_NAME	"renewSessionResult"
#define RENEWSESSIONRESULT_ELEMENT_HREF	NO_HREF

#define ENDSESSION_ELEMENT_NAME		"endSession"
#define ENDSESSIONRESULT_ELEMENT_NAME	"endSessionResult"
#define ENDSESSIONRESULT_ELEMENT_HREF	NO_HREF


#define IDENTIFIER_NSPAIR		NO_NSPAIR

#define DEVICE_ELEMENT_NAME		"device"
#define DEVICE_ELEMENT_HREF		NO_HREF
#define DEVICE_AIK_ELEMENT		"aik-name"
#define DEVICE_AIK_HREF			NO_HREF
#define DEVICE_NAME_ELEMENT		"name"
#define DEVICE_NAME_HREF		NO_HREF

#define IPADDR_ELEMENT_NAME		"ip-address"
#define IPADDR_ELEMENT_HREF		NO_HREF
#define IPADDR_VALUE_ATTR_NAME		"value"
#define IPADDR_TYPE_ATTR_NAME		"type"
#define IPADDR_TYPE_IPV4		"IPv4"
#define IPADDR_TYPE_IPV6		"IPv6"

#define MACADDR_ELEMENT_NAME		"mac-address"
#define MACADDR_ELEMENT_HREF		NO_HREF
#define MACADDR_VALUE_ATTR_NAME		"value"

#define IDENTITY_ELEMENT_NAME		"identity"
#define IDENTITY_ELEMENT_HREF		NO_HREF
#define IDENTITY_TYPE_ATTR_NAME		"type"
#define IDENTITY_NAME_ATTR_NAME		"name"
#define IDENTITY_OTHER_DEF_ATTR_NAME	"other-type-definition"

#define ACCESSREQ_ELEMENT_NAME		"access-request"
#define ACCESSREQ_ELEMENT_HREF		NO_HREF
#define ACCESSREQ_NAME_ATTR_NAME	"name"

#define ADMIN_DOMAIN_ATTR_NAME		"administrative-domain"


#define FILTER_MATCH_ALL		NULL
#define FILTER_MATCH_NOTHING		""

#endif /* TCGIFMAPBASE_H_ */

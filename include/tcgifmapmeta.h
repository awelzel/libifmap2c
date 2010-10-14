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

#ifndef TCGMIFMAPETA_H_
#define TCGIFMAPMETA_H_

#define TCG_META_PREFIX				"meta"
#define TCG_META_HREF				"http://www.trustedcomputinggroup.org/2010/IFMAP-METADATA/2"
#define TCG_META_NSPAIR 			std::pair<std::string, std::string>(TCG_META_PREFIX, TCG_META_HREF)

#define META_CARDINALITY_ATTR_NAME		"ifmap-cardinality"


#define META_IPMAC_ELEMENT_NAME			"ip-mac"
#define META_IPMAC_CARDINALITY			multi
#define META_IPMAC_START_TIME			"start-time"
#define META_IPMAC_END_TIME			"end-time"
#define META_IPMAC_DHCP_SERVER			"dhcp-server"

#define META_LAYER2INFO_ELEMENT_NAME		"layer2-information"
#define META_LAYER2INFO_CARDINALITY		multi
#define META_LAYER2INFO_VLAN_ELEMENT_NAME	"vlan"
#define META_LAYER2INFO_VLANNAME_ELEMENT_NAME	"vlan-name"
#define META_LAYER2INFO_PORT_ELEMENT_NAME	"port"
#define META_LAYER2INFO_ADMINDOM_ELEMENT_NAME	"administrative-domain"

#define META_ARDEV_ELEMENT_NAME			"access-request-device"
#define META_ARDEV_CARDINALITY			single

#define META_ARMAC_ELEMENT_NAME			"access-request-mac"
#define META_ARMAC_CARDINALITY			single

#define META_ARIP_ELEMENT_NAME			"access-request-ip"
#define META_ARIP_CARDINALITY			single

#define META_AUTHAS_ELEMENT_NAME		"authenticated-as"
#define META_AUTHAS_CARDINALITY			single

#define META_AUTHBY_ELEMENT_NAME		"authenticated-by"
#define META_AUTHBY_CARDINALITY			single

#define META_DEVIP_ELEMENT_NAME			"device-ip"
#define META_DEVIP_CARDINALITY			single

#define META_ROLE_ELEMENT_NAME			"role"
#define META_ROLE_CARDINALITY			multi

#endif /* TCGMETA_H_ */

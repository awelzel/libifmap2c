/*
 * COPYRIGHT AND PERMISSION NOTICE
 *
 * Copyright (c) 2011, Arne Welzel, <arne.welzel@googlemail.com>
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

/*
 * This currently only contains the all-ip global identifier, but in the
 * best case will evolve to contain all global identifiers.
 */

#ifndef IBOSGLOBALIDENTIFIER_H_
#define IBOSGLOBALIDENTIFIER_H_

#include <libifmap2c/identifiers.h>
#include <libifmap2c/basicxmlmarshalable.h>

#define STRP std::pair<std::string, std::string>

namespace ifmap2cibos {

class AllIp : public ifmap2c::Identifier {

public:
	ifmap2c::Identifier *clone(void) const { return new AllIp(); }

	std::string str(void) const { return "custom{all-ip}"; }
};


/* Our own handler for the ip-all custom Identifier.
 * A bit more complex, but it's alright.
 * Note: IBOS customidentifiers are "wrapped" in a
 * "custom-identifier" element:
 *
 * <custom-identifier>
 *   <all-ip/>
 * </custom-identifier>
 */
class AllIpHandler : public ifmap2c::IdentifierHandler {

public:

	AllIpHandler(const std::string& prefix, const std::string& href) :
		_prefix(prefix), _href(href) { }

	/* AllIp to XmlMarshalable. This is rather
	 * static, except for the NS HREF
	 */
	ifmap2c::XmlMarshalable *toXml(ifmap2c::Identifier *const i) {
		(void)i; // unused

		ifmap2c::XmlMarshalable *ret = new ifmap2c::BasicXmlMarshalable(
				"custom-identifier", EMPTY_VALUE, NO_NSPAIR);

		ifmap2c::XmlMarshalable *ident = new ifmap2c::BasicXmlMarshalable(
				"all-ip", EMPTY_VALUE, STRP(_prefix, _href));
		
		ident->addXmlNamespaceDefinition(STRP(_prefix, _href));

		ret->addXmlChild(ident);

		return ret;
	}

	/* Parsing of possible AllIp identifier:
	 * fromXml() is expected to return NULL, if this
	 * handler is not responsible for the given
	 * identifier.
	 */
	AllIp *fromXml(ifmap2c::XmlMarshalable *const i) {

		if (!cmpName(i, "custom-identifier", NO_HREF))
			return NULL; /* not for us */
	
		// Dangerous?
		ifmap2c::XmlMarshalable *firstChild = *i->getXmlChildren().begin();

		if (!cmpName(firstChild, "all-ip", _href))
			return NULL; /* not for us */

		// seems like a custom all-ip identifier,
		// could do some more checks...
		return new AllIp();
	}

	bool canHandle(ifmap2c::Identifier *const i) const {
		return typeid(*i) == typeid(AllIp);
	}

private:
	bool cmpName(ifmap2c::XmlMarshalable *const el, 
			const std::string& name, const std::string& href) {
		return ifmap2c::XmlMarshalable::compNameNs(el, name, href);
	}

	std::string _prefix;
	std::string _href;
};

} // namespace

#endif /* IBOSGLOBALIDENTIFIER_H_ */

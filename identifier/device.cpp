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

#include "device.h"

using namespace std;

namespace ifmap2c {

Device::Device(const string& val, DeviceType type) :
	Identifier (DEVICE_ELEMENT_NAME, EMPTY_VALUE), _value(val), _deviceType(type)
{
	BasicXmlMarshalable *child = NULL;
	
	if (type == aik)
		child = new BasicXmlMarshalable(DEVICE_AIK_ELEMENT,
				val, IDENTIFIER_NSPAIR);
	else if (type == name)
		child = new BasicXmlMarshalable(DEVICE_NAME_ELEMENT,
				val, IDENTIFIER_NSPAIR);

	addXmlChild(child);
}

Device *
Device::createDeviceAik(const string& val)
{
	return new Device(val, aik);
}

Device *
Device::createDeviceName(const string& val)
{
	return new Device(val, name);
}

const string&
Device::getValue() const
{
	return _value;
}

DeviceType
Device::getDeviceType() const
{
	return _deviceType;
}

} // namespace
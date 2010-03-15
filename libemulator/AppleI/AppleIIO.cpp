
/**
 * libemulator
 * Apple I IO
 * (C) 2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls the Apple I Input Output range
 */

#include "AppleIIO.h"

int AppleIIO::ioctl(int message, void *data)
{
	switch(message)
	{
		case OEIOCTL_CONNECT:
		{
			OEIoctlConnection *connection = (OEIoctlConnection *) data;
			if (connection->name == "pia")
				pia = connection->component;
			else if (connection->name == "floatingBus")
				floatingBus = connection->component;
			break;
		}
		case OEIOCTL_SET_PROPERTY:
		{
			OEIoctlProperty *property = (OEIoctlProperty *) data;
			if (property->name == "offset")
				offset = intValue(property->value);
			else if (property->name == "size")
				size = intValue(property->value);
			break;
		}
		case OEIOCTL_GET_MEMORYMAP:
		{
			OEIoctlMemoryMap *memoryMap = (OEIoctlMemoryMap *) data;
			memoryMap->component = this;
			memoryMap->offset = offset;
			memoryMap->size = size;
			break;
		}
	}
	
	return 0;
}

int AppleIIO::read(int address)
{
	if (address & 0x10)
		return pia->read(address);
	else
		return floatingBus->read(address);
}

void AppleIIO::write(int address, int value)
{
	if (address & 0x10)
		pia->write(address, value);
	else
		floatingBus->write(address, value);
}
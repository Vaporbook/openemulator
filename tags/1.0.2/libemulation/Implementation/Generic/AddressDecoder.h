
/**
 * libemulation
 * Address decoder
 * (C) 2010-2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls an address decoder
 */

#ifndef _ADDRESSDECODER_H
#define _ADDRESSDECODER_H

#include <list>

#include "OEComponent.h"

#include "MemoryInterface.h"

typedef list<MemoryMap> AddressDecoderMaps;
typedef map<string, string> AddressDecoderMap;
typedef map<string, OEComponent *> AddressDecoderRef;

class AddressDecoder : public OEComponent
{
public:
    AddressDecoder();
    
    bool setValue(string name, string value);
    bool setRef(string name, OEComponent *ref);
    bool init();
    
    bool postMessage(OEComponent *sender, int event, void *data);
    
    OEUInt8 read(OEAddress address);
    void write(OEAddress address, OEUInt8 value);
    
private:
    OEAddress addressSize;
    OEAddress blockSize;
    AddressDecoderMap confMap;
    
    OEComponent *floatingBus;
    AddressDecoderRef confRef;
    
    AddressDecoderMaps pendingMaps;
    
    OEComponents readMap;
    OEComponents writeMap;
    
    OEAddress addressMask;
    
    bool getMemoryMap(MemoryMap& theMap, OEComponent *component, string value);
    void mapMemory(MemoryMap *theMap);
    bool mapConf(OEComponent *component, string conf);
};

#endif

/**
 * libemulation
 * Apple Disk II Interface Card
 * (C) 2011-2012 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Implements an Apple Disk II interface card
 */

#include "AppleDiskIIInterfaceCard.h"

#include "AppleIIInterface.h"
#include "ControlBusInterface.h"

#define SEQUENCER_LOAD          (1 << 0)
#define SEQUENCER_WRITE         (1 << 1)

#define SEQUENCER_READ_SKIP     64
#define SEQUENCER_WRITE_SKIP    6656

typedef enum
{
    SEQUENCER_READSHIFT,
    SEQUENCER_READLOAD,
    SEQUENCER_WRITESHIFT,
    SEQUENCER_WRITELOAD,
} SequencerMode;

AppleDiskIIInterfaceCard::AppleDiskIIInterfaceCard()
{
	controlBus = NULL;
    drive[0] = &dummyDrive;
    drive[1] = &dummyDrive;
    
    phaseControl = 0;
    driveSel = 0;
    driveOn = false;
    sequencerMode = 0;
    
    currentDrive = &dummyDrive;
    reset = false;
    timerOn = false;
    
    driveEnableControl = false;
    lastCycles = 0;
}

bool AppleDiskIIInterfaceCard::setValue(string name, string value)
{
	if (name == "phaseControl")
		phaseControl = getOEInt(value);
	else if (name == "driveOn")
		driveOn = getOEInt(value);
	else if (name == "driveSel")
		driveSel = getOEInt(value);
	else if (name == "load")
        OESetBit(sequencerMode, SEQUENCER_LOAD, getOEInt(value));
	else if (name == "write")
        OESetBit(sequencerMode, SEQUENCER_WRITE, getOEInt(value));
	else if (name == "sequencerState")
		sequencerState = getOEInt(value);
	else if (name == "dataRegister")
		dataRegister = getOEInt(value);
	else
		return false;
    
    return true;
}

bool AppleDiskIIInterfaceCard::getValue(string name, string& value)
{
	if (name == "phaseControl")
		value = getString(phaseControl);
	else if (name == "driveOn")
		value = getString(driveOn);
	else if (name == "driveSel")
		value = getString(driveSel);
	else if (name == "load")
        value = getString(OEGetBit(sequencerMode, SEQUENCER_LOAD));
	else if (name == "write")
        value = getString(OEGetBit(sequencerMode, SEQUENCER_WRITE));
	else if (name == "sequencerState")
		value = getString(sequencerState);
	else if (name == "dataRegister")
		value = getString(dataRegister);
	else
		return false;
	
	return true;
}

bool AppleDiskIIInterfaceCard::setRef(string name, OEComponent *ref)
{
	if (name == "controlBus")
    {
        if (controlBus)
        {
            controlBus->removeObserver(this, CONTROLBUS_RESET_DID_ASSERT);
            controlBus->removeObserver(this, CONTROLBUS_RESET_DID_CLEAR);
            controlBus->removeObserver(this, CONTROLBUS_POWERSTATE_DID_CHANGE);
        }
		controlBus = ref;
        if (controlBus)
        {
            controlBus->addObserver(this, CONTROLBUS_RESET_DID_ASSERT);
            controlBus->addObserver(this, CONTROLBUS_RESET_DID_CLEAR);
            controlBus->addObserver(this, CONTROLBUS_POWERSTATE_DID_CHANGE);
        }
    }
    else if (name == "floatingBus")
        floatingBus = ref;
	else if (name == "drive1")
		drive[0] = ref ? ref : &dummyDrive;
	else if (name == "drive2")
		drive[1] = ref ? ref : &dummyDrive;
	else
		return false;
	
	return true;
}

bool AppleDiskIIInterfaceCard::init()
{
    if (!controlBus)
    {
        logMessage("controlBus not connected");
        
        return false;
    }
    
    if (!floatingBus)
    {
        logMessage("floatingBus not connected");
        
        return false;
    }
    
    update();
    
    return true;
}

void AppleDiskIIInterfaceCard::update()
{
    updateDriveEnableControl();
    
    currentDrive = &dummyDrive;
    
    updateDriveSel(driveSel);
}

void AppleDiskIIInterfaceCard::notify(OEComponent *sender, int notification, void *data)
{
    switch (notification)
    {
        case CONTROLBUS_TIMER_DID_FIRE:
            timerOn = false;
            
            break;
            
        case CONTROLBUS_RESET_DID_ASSERT:
            reset = true;
            
            setPhaseControl(0, false);
            setPhaseControl(1, false);
            setPhaseControl(2, false);
            setPhaseControl(3, false);
            setDriveOn(false);
            setDriveSel(0);
            setSequencerLoad(false);
            setSequencerWrite(false);
            
            break;
            
        case CONTROLBUS_RESET_DID_CLEAR:
            reset = false;
            
            break;
            
        case CONTROLBUS_POWERSTATE_DID_CHANGE:
            if (*((ControlBusPowerState *)data) != CONTROLBUS_POWERSTATE_ON)
            {
                driveOn = false;
                timerOn = false;
            }
            
            break;
    }
    
    updateDriveEnableControl();
}

OEChar AppleDiskIIInterfaceCard::read(OEAddress address)
{
    updateSequencer();
    
    switch (address & 0xf)
    {
        case 0x0: case 0x1:
            setPhaseControl(0, address & 0x1);
            
            break;
            
        case 0x2: case 0x3:
            setPhaseControl(1, address & 0x1);
            
            break;
            
        case 0x4: case 0x5:
            setPhaseControl(2, address & 0x1);
            
            break;
            
        case 0x6: case 0x7:
            setPhaseControl(3, address & 0x1);
            
            break;
            
        case 0x8: case 0x9:
            setDriveOn(address & 0x1);
            
            break;
            
        case 0xa: case 0xb:
            setDriveSel(address & 0x1);
            
            break;
            
        case 0xc: case 0xd:
            setSequencerLoad(address & 0x1);
            
            break;
            
        case 0xe: case 0xf:
            setSequencerWrite(address & 0x1);
            
            break;
    }
    
    if (driveEnableControl && !(address & 0x1))
        return dataRegister;
    
    return floatingBus->read(address);
}

void AppleDiskIIInterfaceCard::write(OEAddress address, OEChar value)
{
    updateSequencer();
    
    switch (address & 0xf)
    {
        case 0x0: case 0x1:
            setPhaseControl(0, address & 0x1);
            
            break;
            
        case 0x2: case 0x3:
            setPhaseControl(1, address & 0x1);
            
            break;
            
        case 0x4: case 0x5:
            setPhaseControl(2, address & 0x1);
            
            break;
            
        case 0x6: case 0x7:
            setPhaseControl(3, address & 0x1);
            
            break;
            
        case 0x8: case 0x9:
            setDriveOn(address & 0x1);
            
            break;
            
        case 0xa: case 0xb:
            setDriveSel(address & 0x1);
            
            break;
            
        case 0xc: case 0xd:
            setSequencerLoad(address & 0x1);
            
            break;
            
        case 0xe: case 0xf:
            setSequencerWrite(address & 0x1);
            
            break;
    }
    
    if (driveEnableControl && (address & 0x1))
        dataRegister = value;
}

void AppleDiskIIInterfaceCard::setPhaseControl(OEInt index, bool value)
{
    OEInt lastPhaseControl = phaseControl;
    
    OESetBit(phaseControl, 1 << index, value);
    
    if ((lastPhaseControl != phaseControl) && driveEnableControl)
        updatePhaseControl();
}

void AppleDiskIIInterfaceCard::updatePhaseControl()
{
    currentDrive->postMessage(this, APPLEII_SET_PHASECONTROL, &phaseControl);
}

void AppleDiskIIInterfaceCard::setDriveOn(bool value)
{
    if (driveOn == value)
        return;
    
    driveOn = value;
    
    if (timerOn)
    {
        controlBus->postMessage(this, CONTROLBUS_INVALIDATE_TIMERS, NULL);
        
        timerOn = false;
    }
    
    if (driveOn)
        controlBus->postMessage(this, CONTROLBUS_GET_CYCLES, &lastCycles);
    else
    {
        OELong cycles = 1.0 * APPLEII_CLOCKFREQUENCY;
        
        controlBus->postMessage(this, CONTROLBUS_SCHEDULE_TIMER, &cycles);
        
        timerOn = true;
    }
    
    updateDriveEnableControl();
}

void AppleDiskIIInterfaceCard::updateDriveEnableControl()
{
    bool wasDriveEnableControl = driveEnableControl;
    
    driveEnableControl = (driveOn || timerOn) && !reset;
    
    if (driveEnableControl != wasDriveEnableControl)
        updateDriveEnabled();
}

void AppleDiskIIInterfaceCard::updateDriveEnabled()
{
    currentDrive->postMessage(this, driveEnableControl ?
                              APPLEII_ASSERT_DRIVEENABLE :
                              APPLEII_CLEAR_DRIVEENABLE, NULL);
}

void AppleDiskIIInterfaceCard::setDriveSel(OEInt value)
{
    if (driveSel == value)
        return;
    
    updateDriveSel(value);
}

void AppleDiskIIInterfaceCard::updateDriveSel(OEInt value)
{
    driveSel = value;
    
    OEComponent *lastDrive = currentDrive;
    currentDrive = drive[driveSel];
    
    if (lastDrive == currentDrive)
        return;
    
    if (driveEnableControl)
        lastDrive->postMessage(this, APPLEII_CLEAR_DRIVEENABLE, NULL);
    
    updatePhaseControl();
    updateDriveEnabled();
}

void AppleDiskIIInterfaceCard::setSequencerLoad(bool value)
{
    OESetBit(sequencerMode, SEQUENCER_LOAD, value);
}

void AppleDiskIIInterfaceCard::setSequencerWrite(bool value)
{
    OESetBit(sequencerMode, SEQUENCER_WRITE, value);
}

void AppleDiskIIInterfaceCard::updateSequencer()
{
    if (!driveEnableControl)
        return;
    
    OELong cycles;
    
    controlBus->postMessage(this, CONTROLBUS_GET_CYCLES, &cycles);
    
    OELong bitNum = (cycles - (lastCycles & ~0x3)) >> 2;
    
	switch (sequencerMode)
    {
		case SEQUENCER_READSHIFT:
        {
            if (bitNum > SEQUENCER_READ_SKIP)
            {
                bitNum -= SEQUENCER_READ_SKIP;
                currentDrive->postMessage(this, APPLEII_SKIP_DATA, &bitNum);
                
                bitNum = SEQUENCER_READ_SKIP;
            }
            
			while (bitNum--)
            {
                bool bit = currentDrive->read(0);
                
                if (dataRegister & 0x80)
                {
                    if (!sequencerState)
                        sequencerState = bit;
                    else
                    {
                        sequencerState = 0;
                        dataRegister = 0x02 | bit;
                    }
                }
                else
                {
                    dataRegister <<= 1;
                    dataRegister |= bit;
                }
			}
            
			break;
        }
		case SEQUENCER_READLOAD:
        {
//            logMessage("RL: " + getString(bitNum) + " " + getString(cycles - lastCycles) + " 0");
            
            currentDrive->postMessage(this, APPLEII_SKIP_DATA, &bitNum);
            
            OELong deltaCycles = cycles - lastCycles;
            
            if (deltaCycles > 4)
                deltaCycles = 4;
            
            OEChar senseInput;
            
            currentDrive->postMessage(this, APPLEII_SENSE_INPUT, &senseInput);
            
            senseInput <<= 7;
            
            for (OEInt i = 0; i < 2 * deltaCycles; i++)
            {
                dataRegister >>= 1;
                dataRegister |= senseInput;                
            }
            
            sequencerState = 0;
            
			break;
		}
        case SEQUENCER_WRITESHIFT:
		case SEQUENCER_WRITELOAD:
        {
            sequencerState += (cycles - lastCycles);
            sequencerState &= 0x3;
            
//logMessage("W: " + getString(bitNum) + " " + getString(cycles - lastCycles) + " " + getString(sequencerState));
            
            if (bitNum > SEQUENCER_WRITE_SKIP)
            {
                bitNum -= SEQUENCER_WRITE_SKIP;
                currentDrive->postMessage(this, APPLEII_SKIP_DATA, &bitNum);
                
                bitNum = SEQUENCER_WRITE_SKIP;
            }
            
			while (bitNum--)
            {
                currentDrive->write(0, (dataRegister & 0x80) ? 0xff : 0x00);
                
                dataRegister <<= 1;
            }
            
            break;
        }
	}
    
    lastCycles = cycles;
}
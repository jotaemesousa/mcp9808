#!/usr/bin/env python

from mpsse import *

if __name__ == '__main__':
    mcp = MPSSE(I2C, FOUR_HUNDRED_KHZ, MSB)        # Initialize libmpsse for I2C ad 400kHz, MSB first
    mcp.Start()                        # Send I2C start condition
   
    mcp.Write("\x30")

    if mcp.GetAck() == 0:
        print "Got an ACK!"
    else:
        print "Got a NACK!"
    
    mcp.Write("\x05")

    if mcp.GetAck() == 0:
        print "Got an ACK!"
    else:
        print "Got a NACK!"
    
    mcp.Stop()
    mcp.Start()  
    mcp.Write("\x31")                    # Write the EEPROM slave address with the read bit set
    if mcp.GetAck() == 0:                # Make sure the EEPROM chip acknowledged our read request
        mcp.SetAck(0)                # Need to send a NACK after reading the last byte (only reading 1 byte)
        data = mcp.Read(1)                # Read a byte
        print "Read byte:", hex(ord(data)) 
        mcp.SetAck(1)                # Need to send a NACK after reading the last byte (only reading 1 byte)
        data = mcp.Read(1)                # Read a byte
        print "Read byte:", hex(ord(data))    
    mcp.Stop()                        # Send I2C stop condition
    mcp.Close()                        # Deinitialize libmpsse
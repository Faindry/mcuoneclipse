/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuGenericI2C.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : GenericI2C
**     Version     : Component 01.048, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-06-11, 17:17, # CodeGen: 537
**     Abstract    :
**         This component implements a generic I2C driver wrapper to work both with LDD and non-LDD I2C components.
**     Settings    :
**          Component name                                 : McuGenericI2C
**          Wait                                           : McuWait
**          Support STOP_NOSTART                           : no
**          Write Buffer Size                              : 32
**          non-LDD I2C                                    : Enabled
**            I2C                                          : McuGenericSWI2C
**          LDD I2C                                        : Disabled
**          RTOS                                           : Enabled
**            RTOS                                         : McuRTOS
**            Semaphore                                    : yes
**          Init() on startup                              : yes
**     Contents    :
**         SelectSlave       - uint8_t McuGenericI2C_SelectSlave(uint8_t i2cAddr);
**         UnselectSlave     - uint8_t McuGenericI2C_UnselectSlave(void);
**         RequestBus        - void McuGenericI2C_RequestBus(void);
**         ReleaseBus        - void McuGenericI2C_ReleaseBus(void);
**         WriteBlock        - uint8_t McuGenericI2C_WriteBlock(void* data, uint16_t dataSize,...
**         ReadBlock         - uint8_t McuGenericI2C_ReadBlock(void* data, uint16_t dataSize,...
**         ReadBlockGeneric  - uint8_t McuGenericI2C_ReadBlockGeneric(void* data, uint16_t dataSize,...
**         ReadAddressWait   - uint8_t McuGenericI2C_ReadAddressWait(uint8_t i2cAddr, uint8_t *memAddr,...
**         ReadAddress       - uint8_t McuGenericI2C_ReadAddress(uint8_t i2cAddr, uint8_t *memAddr, uint8_t...
**         WriteAddress      - uint8_t McuGenericI2C_WriteAddress(uint8_t i2cAddr, uint8_t *memAddr, uint8_t...
**         ReadByte          - uint8_t McuGenericI2C_ReadByte(uint8_t i2cAddr, uint8_t *data);
**         WriteByte         - uint8_t McuGenericI2C_WriteByte(uint8_t i2cAddr, uint8_t data);
**         ReadByteAddress8  - uint8_t McuGenericI2C_ReadByteAddress8(uint8_t i2cAddr, uint8_t memAddr,...
**         WriteByteAddress8 - uint8_t McuGenericI2C_WriteByteAddress8(uint8_t i2cAddr, uint8_t memAddr,...
**         ReadWordAddress8  - uint8_t McuGenericI2C_ReadWordAddress8(uint8_t i2cAddr, uint8_t memAddr,...
**         WriteWordAddress8 - uint8_t McuGenericI2C_WriteWordAddress8(uint8_t i2cAddr, uint8_t memAddr,...
**         ProbeACK          - uint8_t McuGenericI2C_ProbeACK(void* data, uint16_t dataSize,...
**         GetSemaphore      - void* McuGenericI2C_GetSemaphore(void);
**         ScanDevice        - uint8_t McuGenericI2C_ScanDevice(uint8_t i2cAddr);
**         Deinit            - void McuGenericI2C_Deinit(void);
**         Init              - void McuGenericI2C_Init(void);
**
** * Copyright (c) 2013-2019, Erich Styger
**  * Web:         https://mcuoneclipse.com
**  * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**  * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**  * All rights reserved.
**  *
**  * Redistribution and use in source and binary forms, with or without modification,
**  * are permitted provided that the following conditions are met:
**  *
**  * - Redistributions of source code must retain the above copyright notice, this list
**  *   of conditions and the following disclaimer.
**  *
**  * - Redistributions in binary form must reproduce the above copyright notice, this
**  *   list of conditions and the following disclaimer in the documentation and/or
**  *   other materials provided with the distribution.
**  *
**  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**  * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file McuGenericI2C.h
** @version 01.00
** @brief
**         This component implements a generic I2C driver wrapper to work both with LDD and non-LDD I2C components.
*/         
/*!
**  @addtogroup McuGenericI2C_module McuGenericI2C module documentation
**  @{
*/         

/* MODULE McuGenericI2C. */

#include "McuGenericI2C.h"
#include "McuWait.h"
#include "McuRTOS.h"
#include "McuGenericSWI2C.h"

#ifndef NULL
  #define NULL 0L
#endif /* NULL */

#if McuGenericI2C_CONFIG_USE_MUTEX
static SemaphoreHandle_t McuGenericI2C_busSem = NULL; /* Semaphore to protect I2C bus access */
#endif
/*
** ===================================================================
**     Method      :  RequestBus (component GenericI2C)
**
**     Description :
**         Starts a critical section for accessing the bus.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuGenericI2C_RequestBus(void)
{
  /*lint -save -e522 function lacks side effect  */
#if McuGenericI2C_CONFIG_USE_ON_REQUEST_BUS_EVENT
  McuGenericI2C_CONFIG_ON_REQUEST_BUS_EVENT();
#endif
  /*lint -restore */
#if McuGenericI2C_CONFIG_USE_MUTEX
  (void)xSemaphoreTakeRecursive(McuGenericI2C_busSem, portMAX_DELAY);
#endif
}

/*
** ===================================================================
**     Method      :  ReleaseBus (component GenericI2C)
**
**     Description :
**         Finishes a critical section for accessing the bus.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuGenericI2C_ReleaseBus(void)
{
#if McuGenericI2C_CONFIG_USE_MUTEX
  (void)xSemaphoreGiveRecursive(McuGenericI2C_busSem);
#endif
  /*lint -save -e522 function lacks side effect  */
#if McuGenericI2C_CONFIG_USE_ON_RELEASE_BUS_EVENT
  McuGenericI2C_CONFIG_ON_RELEASE_BUS_EVENT();
#endif
  /*lint -restore */
}

/*
** ===================================================================
**     Method      :  SelectSlave (component GenericI2C)
**
**     Description :
**         Selects the slave device on the bus. Method might use a
**         semaphore to protect bus access.
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_SelectSlave(uint8_t i2cAddr)
{
  McuGenericI2C_RequestBus();
  if (McuGenericI2C_CONFIG_SELECT_SLAVE(i2cAddr)!=ERR_OK) {
    McuGenericI2C_ReleaseBus();
  #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
    McuGenericI2C_CONFIG_ON_ERROR_EVENT();
  #endif
    return ERR_FAILED;
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  UnselectSlave (component GenericI2C)
**
**     Description :
**         Unselects the device. Method will release a used a semaphore.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_UnselectSlave(void)
{
  McuGenericI2C_ReleaseBus();
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  ReadBlockGeneric (component GenericI2C)
**
**     Description :
**         Read from the device a block with using additional control
**         and flags.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Read buffer
**         dataSize        - Size of read buffer
**         flagsSend       - flags for the send transaction
**         flagsStart      - Start flags
**         flagsAck        - Acknowledge flags
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ReadBlockGeneric(void* data, uint16_t dataSize, McuGenericI2C_EnumSendFlags flags, McuGenericI2C_EnumStartFlags flagsStart, McuGenericI2C_EnumAckFlags flagsAck)
{
  uint8_t res = ERR_OK;

#if McuGenericI2C_CONFIG_RECV_BLOCK_CUSTOM_AVAILABLE
  for(;;) { /* breaks */
    uint16_t nof;

    res = McuGenericI2C_CONFIG_RECV_BLOCK_CUSTOM(data, dataSize, &nof, flagsStart, flagsAck);
    if (res!=ERR_OK) {
    #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
      McuGenericI2C_CONFIG_ON_ERROR_EVENT();
    #endif
      break; /* break for(;;) */
    }
    if (flags==McuGenericI2C_SEND_STOP) {
      res = McuGenericI2C_CONFIG_SEND_STOP();
      if (res!=ERR_OK) {
      #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
        McuGenericI2C_CONFIG_ON_ERROR_EVENT();
      #endif
        break; /* break for(;;) */
      }
    }
    break; /* break for(;;) */
  } /* for(;;) */
#else
  return ERR_FAILED; /* RecvBlockCustom() is not available (e.g. not available in the Hardware I2C component */
#endif
  return res;
}

/*
** ===================================================================
**     Method      :  ReadBlock (component GenericI2C)
**
**     Description :
**         Read from the device a block.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Read buffer
**         dataSize        - Size of read buffer
**         flags           - flags for the transaction
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ReadBlock(void* data, uint16_t dataSize, McuGenericI2C_EnumSendFlags flags)
{
  uint8_t res = ERR_OK;
  uint16_t nof;

  for(;;) { /* breaks */
    res = McuGenericI2C_CONFIG_RECV_BLOCK(data, dataSize, &nof);
    if (res!=ERR_OK) {
    #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
      McuGenericI2C_CONFIG_ON_ERROR_EVENT();
    #endif
      break; /* break for(;;) */
    }
    if (flags==McuGenericI2C_SEND_STOP) {
      res = McuGenericI2C_CONFIG_SEND_STOP();
      if (res!=ERR_OK) {
      #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
        McuGenericI2C_CONFIG_ON_ERROR_EVENT();
      #endif
        break; /* break for(;;) */
      }
    }
    break; /* break for(;;) */
  } /* for(;;) */
  return res;
}

/*
** ===================================================================
**     Method      :  WriteBlock (component GenericI2C)
**
**     Description :
**         Write a block to the device.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Data write buffer
**         dataSize        - 
**         flags           - flags for the transaction
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_WriteBlock(void* data, uint16_t dataSize, McuGenericI2C_EnumSendFlags flags)
{
  uint16_t nof;
  uint8_t res = ERR_OK;

  for(;;) { /* breaks */
#if McuGenericI2C_CONFIG_SUPPORT_STOP_NO_START
    if (McuGenericI2C_STOP_NOSTART==flags) {
      res = McuGenericI2C_CONFIG_SEND_BLOCK_CONTINUE(data, dataSize, &nof);
    } else {
      res = McuGenericI2C_CONFIG_SEND_BLOCK(data, dataSize, &nof);
    }
#else
    res = McuGenericI2C_CONFIG_SEND_BLOCK(data, dataSize, &nof);
#endif
    if (res!=ERR_OK) {
      (void)McuGenericI2C_CONFIG_SEND_STOP();
      #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
        McuGenericI2C_CONFIG_ON_ERROR_EVENT();
      #endif
      break; /* break for(;;) */
    }
    if (flags==McuGenericI2C_SEND_STOP || (flags==McuGenericI2C_STOP_NOSTART)) {
      res = McuGenericI2C_CONFIG_SEND_STOP();
      if (res!=ERR_OK) {
      #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
        McuGenericI2C_CONFIG_ON_ERROR_EVENT();
      #endif
        break; /* break for(;;) */
      }
    }
    break; /* break for(;;) */
  } /* for(;;) */
  return res;
}

/*
** ===================================================================
**     Method      :  ReadAddressWait (component GenericI2C)
**
**     Description :
**         Same as ReadAddress, but with an optional wait between the
**         address and read. Read from the device. This writes
**         (S+i2cAddr+0), (memAddr), (Sr+i2cAddr+1), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**       * memAddr         - Pointer to device memory address
**         memAddrSize     - number of address bytes
**         waitMs          - Wait time between the address part
**                           and reading the data part. Wait time can be
**                           zero.
**       * data            - Pointer to read buffer
**         dataSize        - Size of read buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ReadAddressWait(uint8_t i2cAddr, uint8_t *memAddr, uint8_t memAddrSize, uint16_t waitMs, uint8_t *data, uint16_t dataSize)
{
  uint8_t res = ERR_OK;
  uint16_t nof;

  if (McuGenericI2C_SelectSlave(i2cAddr)!=ERR_OK) {
  #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
    McuGenericI2C_CONFIG_ON_ERROR_EVENT();
  #endif
    return ERR_FAILED;
  }
  for(;;) { /* breaks */
    if(memAddr!=NULL) { /* only if we want to send an address */
      res = McuGenericI2C_CONFIG_SEND_BLOCK((void*)memAddr, memAddrSize, &nof);
      if (res!=ERR_OK) {
        (void)McuGenericI2C_CONFIG_SEND_STOP();
      #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
        McuGenericI2C_CONFIG_ON_ERROR_EVENT();
      #endif
        break; /* break for(;;) */
      }
    }
    /* optional wait time */
    if (waitMs!=0) {
      McuWait_WaitOSms(waitMs);
    }
    if (data!=NULL) {
      /* receive data */
      res = McuGenericI2C_CONFIG_RECV_BLOCK(data, dataSize, &nof);
      if (res!=ERR_OK) {
        (void)McuGenericI2C_CONFIG_SEND_STOP();
      #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
        McuGenericI2C_CONFIG_ON_ERROR_EVENT();
      #endif
        break; /* break for(;;) */
      }
      res = McuGenericI2C_CONFIG_SEND_STOP();
      if (res!=ERR_OK) {
      #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
        McuGenericI2C_CONFIG_ON_ERROR_EVENT();
      #endif
        break; /* break for(;;) */
      }
    } /* if receive data */
    break; /* break for(;;) */
  } /* for(;;) */
  if (McuGenericI2C_UnselectSlave()!=ERR_OK) {
  #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
    McuGenericI2C_CONFIG_ON_ERROR_EVENT();
  #endif
    return ERR_FAILED;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  ReadAddress (component GenericI2C)
**
**     Description :
**         Read from the device. This writes (S+i2cAddr+0), (memAddr),
**         (Sr+i2cAddr+1), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**       * memAddr         - Pointer to device memory address
**         memAddrSize     - number of address bytes
**       * data            - Pointer to read buffer
**         dataSize        - Size of read buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ReadAddress(uint8_t i2cAddr, uint8_t *memAddr, uint8_t memAddrSize, uint8_t *data, uint16_t dataSize)
{
  return McuGenericI2C_ReadAddressWait(i2cAddr, memAddr, memAddrSize, 0, data, dataSize);
}

/*
** ===================================================================
**     Method      :  WriteAddress (component GenericI2C)
**
**     Description :
**         Write to the device: (S+i2cAddr+0), (memAddr), (data)...
**         (data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**       * memAddr         - Pointer to device memory address
**         memAddrSize     - number of address bytes
**       * data            - Pointer to data write buffer
**         dataSize        - Size of data buffer in bytes
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_WriteAddress(uint8_t i2cAddr, uint8_t *memAddr, uint8_t memAddrSize, uint8_t *data, uint16_t dataSize)
{
  static uint8_t writeBuf[McuGenericI2C_WRITE_BUFFER_SIZE];
  uint8_t *p;
  uint16_t i;
  uint16_t nof;
  uint8_t res = ERR_OK;

  if (McuGenericI2C_SelectSlave(i2cAddr)!=ERR_OK) {
  #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
    McuGenericI2C_CONFIG_ON_ERROR_EVENT();
  #endif
    return ERR_FAILED;
  }
  if (memAddrSize+dataSize>McuGenericI2C_WRITE_BUFFER_SIZE) {
  #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
    McuGenericI2C_CONFIG_ON_ERROR_EVENT();
  #endif
    return ERR_FAILED;
  }
  i = 0; p = memAddr;
  while(i<McuGenericI2C_WRITE_BUFFER_SIZE && memAddrSize>0) {
    writeBuf[i++] = *p++;
    memAddrSize--;
  }
  p = data;
  while(i<McuGenericI2C_WRITE_BUFFER_SIZE && dataSize>0) {
    writeBuf[i++] = *p++;
    dataSize--;
  }
  for(;;) { /* breaks */
    res = McuGenericI2C_CONFIG_SEND_BLOCK((void*)writeBuf, i, &nof);
    if (res!=ERR_OK) {
      (void)McuGenericI2C_CONFIG_SEND_STOP();
    #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
      McuGenericI2C_CONFIG_ON_ERROR_EVENT();
    #endif
      break; /* break for(;;) */
    }
    res = McuGenericI2C_CONFIG_SEND_STOP();
    if (res!=ERR_OK) {
    #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
      McuGenericI2C_CONFIG_ON_ERROR_EVENT();
    #endif
      break; /* break for(;;) */
    }
    break; /* break for(;;) */
  } /* for(;;) */
  if (McuGenericI2C_UnselectSlave()!=ERR_OK) {
  #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
    McuGenericI2C_CONFIG_ON_ERROR_EVENT();
  #endif
    return ERR_FAILED;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  Init (component GenericI2C)
**
**     Description :
**         Initializes the driver.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuGenericI2C_Init(void)
{
#if McuGenericI2C_CONFIG_USE_MUTEX
  McuGenericI2C_busSem = xSemaphoreCreateRecursiveMutex();
  if (McuGenericI2C_busSem==NULL) { /* semaphore creation failed */
  #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
    McuGenericI2C_CONFIG_ON_ERROR_EVENT();
  #endif
    for(;;) {} /* error, not enough memory? */
  }
  vQueueAddToRegistry(McuGenericI2C_busSem, "McuGenericI2C_Mutex");
#endif
}

/*
** ===================================================================
**     Method      :  Deinit (component GenericI2C)
**
**     Description :
**         Deinitializes the driver.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuGenericI2C_Deinit(void)
{
#if McuGenericI2C_CONFIG_USE_MUTEX
  vQueueUnregisterQueue(McuGenericI2C_busSem);
  vSemaphoreDelete(McuGenericI2C_busSem);
  McuGenericI2C_busSem = NULL;
#endif
}

/*
** ===================================================================
**     Method      :  GetSemaphore (component GenericI2C)
**
**     Description :
**         Returns the currently allocated semaphore.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
void* McuGenericI2C_GetSemaphore(void)
{
#if McuGenericI2C_CONFIG_USE_MUTEX
  return McuGenericI2C_busSem;
#else
  return NULL; /* RTOS and Semaphore enabled in properties */
#endif
}

/*
** ===================================================================
**     Method      :  ReadByteAddress8 (component GenericI2C)
**
**     Description :
**         Read a byte from the device using an 8bit memory address.
**         This writes (S+i2cAddr+0), (memAddr), (Sr+i2cAddr+1), (data)..
**         .(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**         memAddr         - Device memory address
**       * data            - Pointer to read buffer (single byte)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ReadByteAddress8(uint8_t i2cAddr, uint8_t memAddr, uint8_t *data)
{
  return McuGenericI2C_ReadAddress(i2cAddr, &memAddr, sizeof(memAddr), data, 1);
}

/*
** ===================================================================
**     Method      :  WriteByteAddress8 (component GenericI2C)
**
**     Description :
**         Write a byte to the device using an 8bit memory address:
**         (S+i2cAddr+0), (memAddr), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**         memAddr         - Device memory address
**         data            - Data value
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_WriteByteAddress8(uint8_t i2cAddr, uint8_t memAddr, uint8_t data)
{
  return McuGenericI2C_WriteAddress(i2cAddr, &memAddr, sizeof(memAddr), &data, sizeof(data));
}

/*
** ===================================================================
**     Method      :  ReadWordAddress8 (component GenericI2C)
**
**     Description :
**         Read a word from the device using an 8bit memory address.
**         This writes (S+i2cAddr+0), (memAddr), (Sr+i2cAddr+1), (data)..
**         .(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**         memAddr         - Device memory address
**       * data            - Pointer to read buffer (single byte)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ReadWordAddress8(uint8_t i2cAddr, uint8_t memAddr, uint16_t *data)
{
  return McuGenericI2C_ReadAddress(i2cAddr, &memAddr, sizeof(memAddr), (uint8_t*)data, 2);
}

/*
** ===================================================================
**     Method      :  WriteWordAddress8 (component GenericI2C)
**
**     Description :
**         Write a word to the device using an 8bit memory address:
**         (S+i2cAddr+0), (memAddr), (data)...(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**         memAddr         - Device memory address
**         data            - Data value
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_WriteWordAddress8(uint8_t i2cAddr, uint8_t memAddr, uint16_t data)
{
  return McuGenericI2C_WriteAddress(i2cAddr, &memAddr, sizeof(memAddr), (uint8_t*)&data, sizeof(data));
}

/*
** ===================================================================
**     Method      :  ScanDevice (component GenericI2C)
**
**     Description :
**         Checks if a device responds on the bus with an ACK.
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - 7bit I2C device address
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ScanDevice(uint8_t i2cAddr)
{
  uint8_t res = ERR_OK;
  uint16_t nof;
  uint8_t dummy;

  if (McuGenericI2C_SelectSlave(i2cAddr)!=ERR_OK) {
    return ERR_FAILED;
  }
  for(;;) { /* breaks */
    res = McuGenericI2C_CONFIG_RECV_BLOCK((void*)&dummy, 1, &nof);
    if (res!=ERR_OK) {
      (void)McuGenericI2C_CONFIG_SEND_STOP();
    #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
      McuGenericI2C_CONFIG_ON_ERROR_EVENT();
    #endif
      break; /* break for(;;) */
    }
    res = McuGenericI2C_CONFIG_SEND_STOP();
    if (res!=ERR_OK) {
    #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
      McuGenericI2C_CONFIG_ON_ERROR_EVENT();
    #endif
      break; /* break for(;;) */
    }
    break; /* break for(;;) */
  } /* for(;;) */
  if (McuGenericI2C_UnselectSlave()!=ERR_OK) {
  #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
    McuGenericI2C_CONFIG_ON_ERROR_EVENT();
  #endif
    return ERR_FAILED;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  ProbeACK (component GenericI2C)
**
**     Description :
**         Accesses the bus to check if the device responds with an ACK
**         (ACK polling).
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Data write buffer
**         dataSize        - 
**         flags           - flags for the transaction
**         WaitTimeUS      - Waiting time in microseconds
**                           to wait for the ACK on the bus.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ProbeACK(void* data, uint16_t dataSize, McuGenericI2C_EnumSendFlags flags, uint16_t WaitTimeUS)
{
  uint16_t nof;
  uint8_t res = ERR_OK;

  (void)WaitTimeUS; /* not used */
  for(;;) { /* breaks */
    res = McuGenericI2C_CONFIG_SEND_BLOCK(data, dataSize, &nof);
    if (res!=ERR_OK) {
      (void)McuGenericI2C_CONFIG_SEND_STOP();
    #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
      McuGenericI2C_CONFIG_ON_ERROR_EVENT();
    #endif
      break; /* break for(;;) */
    }
    if (flags==McuGenericI2C_SEND_STOP) {
      res = McuGenericI2C_CONFIG_SEND_STOP();
      if (res!=ERR_OK) {
      #if McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT
        McuGenericI2C_CONFIG_ON_ERROR_EVENT();
      #endif
        break; /* break for(;;) */
      }
    }
    break; /* break for(;;) */
  } /* for(;;) */
  return res;
}

/*
** ===================================================================
**     Method      :  ReadByte (component GenericI2C)
**
**     Description :
**         Read a byte from the device. This writes (S+i2cAddr+0),
**         (Sr+i2cAddr+1),(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C Address of device
**       * data            - Pointer to read buffer (single byte)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_ReadByte(uint8_t i2cAddr, uint8_t *data)
{
  return McuGenericI2C_ReadAddress(i2cAddr, NULL, 0, data, 1);
}

/*
** ===================================================================
**     Method      :  WriteByte (component GenericI2C)
**
**     Description :
**         Write a byte to the device: (S+i2cAddr+0).(data+P)
**     Parameters  :
**         NAME            - DESCRIPTION
**         i2cAddr         - I2C address of device
**         data            - Data value
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuGenericI2C_WriteByte(uint8_t i2cAddr, uint8_t data)
{
  return McuGenericI2C_WriteAddress(i2cAddr, NULL, 0, &data, 1);
}

/* END McuGenericI2C. */

/*!
** @}
*/

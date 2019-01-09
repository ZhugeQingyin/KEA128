/************************************************************************************** 
*                             
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : sd.c
* Version        : V1.0
* Programmer(s)  : 
* Parameters     : KEA128
* DebugTools     : JLINK(SWD) And Realview MDK
* Description    : SD driver
*
*
**************************************************************************************/

/**************************************************************************************
* header file                                                 
**************************************************************************************/
#include "SD.h"

/**************************************************************************************
* global variable                                              
**************************************************************************************/

uint8 SD_Type = 0;
uint32 Logic_sector;
/**************************************************************************************
* FunctionName   : MMCDelayUs()
* Description    : software delay
* EntryParameter : tt
* ReturnValue    : None
**************************************************************************************/
void MMCDelayUs(uint16 tt)
{
	uint8 i;

	while (tt--)
	{
		for (i = 0; i < 250; i++)
		{
			;
		}
	}
}

/**************************************************************************************
* FunctionName   : MMCCS()
* Description    : MMC card selection: 0 select ; 1 not select
* EntryParameter : cs 
* ReturnValue    : None
**************************************************************************************/
void MMCCS(uint8 cs)
{
	if (cs == 1)
	{
		MMC_CS_SET(); // SS=1
	}
	else
	{
		MMC_CS_CLR(); // SS=0
	}
}

uint8 sd_WaitRead(void)
{
	uint32 cnt = 0x00fffff;
	uint8 sta;
	do
	{
		sta = SPI_Send_Byte(spi1, 0xff);
		;
		if (sta == 0xff) //
		{
			return 0;
		}
		cnt--;
	} while (cnt);

	return 1;
}
static void sd_Disable_Select(void)
{
	MMCCS(1);
	SPI_Send_Byte(spi1, 0xff);
}

static uint8 sd_Enable_Select(void)
{
	MMCCS(0); //
	if (sd_WaitRead() == 0)
	{
		return 0;
	}
	sd_Disable_Select();

	return 1;
}

/**************************************************************************************
* FunctionName   : MMCWriteCmd()
* Description    : MMC write command
* EntryParameter : cmd - command,arg - addr,crc - crc data
* ReturnValue    : None
**************************************************************************************/

uint8 MMCWriteCmd(uint8 cmd, uint32 arg, uint8 crc)
{
	uint16 cnt = 512;
	uint8 sta;
	MMCCS(1);
	SPI_Send_Byte(spi1, 0xff); //write command delay

	MMCCS(0);
	if (sd_Enable_Select())
	{
		return 0xff;
	}

	SPI_Send_Byte(spi1, cmd | 0x40);
	SPI_Send_Byte(spi1, (uint8)(arg >> 24));
	SPI_Send_Byte(spi1, (uint8)(arg >> 16));
	SPI_Send_Byte(spi1, (uint8)(arg >> 8));
	SPI_Send_Byte(spi1, (uint8)(arg));
	SPI_Send_Byte(spi1, crc);

	do
	{
		sta = SPI_Send_Byte(spi1, 0xff);
		cnt--;
	} while ((cnt) && (sta == 0xff));

	return sta;
}

uint8 MMCWriteCmd_NoDessert(uint8 cmd, uint32 arg, uint8 crc)
{
	uint16 cnt = 512;
	uint8 sta;

	SPI_Send_Byte(spi1, 0xff);
	SPI_Send_Byte(spi1, 0xff);

	MMCCS(0);
	SPI_Send_Byte(spi1, cmd | 0x40);
	SPI_Send_Byte(spi1, (uint8)(arg >> 24));
	SPI_Send_Byte(spi1, (uint8)(arg >> 16));
	SPI_Send_Byte(spi1, (uint8)(arg >> 8));
	SPI_Send_Byte(spi1, (uint8)(arg));
	SPI_Send_Byte(spi1, crc);

	do
	{
		sta = SPI_Send_Byte(spi1, 0xff);
		cnt--;
	} while ((cnt) && (sta == 0xff));

	return sta;
}
/**************************************************************************************
* FunctionName   : SSP0LowSpeed()
* Description    : SSP0 low speed 400K
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void SSP0LowSpeed(void)
{
	SPI1_BASE_PTR->BR = 7;
}

/**************************************************************************************
* FunctionName   : SSP0HighSpeed()
* Description    : SSP0 high speed
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void SSP0HighSpeed(void)
{
	SPI1_BASE_PTR->BR = 2;
}

/**************************************************************************************
* FunctionName   : MMCInit()
* Description    : initial SD card
* EntryParameter : None
* ReturnValue    : 1 failure, 0 success
**************************************************************************************/
uint8 MMCInit(void)
{
	uint8 i = 0, k = 0, tmp = 0;
	uint16 cnt = 0;
	uint8 buff[512];

	SSP0LowSpeed(); // low speed
	MMCDelayUs(5000);

	for (i = 0; i < 0x0F; i++)
	{
		SPI_Send_Byte(spi1, 0xFF); // send 72 clocks
	}
	// Send Command CMD0 to SD/SD Card  enter idle
	do
	{
		tmp = MMCWriteCmd(CMD0, 0x00, 0x95); // CMD0
		k++;
	} while ((tmp != 1) && (k < 200));

	if (k == 0)
	{
		MMCCS(1); //cs pullup, disconnect
		SPI_Send_Byte(spi1, 0xFF);
		//printf("\n SD reset fail");
		return 1; //
	}
	//get SD card version
	tmp = MMCWriteCmd(CMD8, 0x1AA, 0x87);
	//printf( "SD_CMD8  return  %d........\n\n", tmp );
	if (tmp == 1) // 2.0 card
	{
		cnt = 0xffff;

		do
		{
			MMCWriteCmd(CMD55, 0, 0xff);
			tmp = MMCWriteCmd(CMD41, 0x40000000, 0xff); //CMD41
			cnt--;
		} while ((tmp) && (cnt));
		//Get OCR information
		tmp = MMCWriteCmd(CMD58, 0, 0);
		if (tmp != 0x00)
		{
			MMCCS(1); //cs pullup, SD card disconnect
			//printf( "\nSD_CMD58 return  %d....\n", tmp );
			return 1; //
		}

		for (i = 0; i < 4; i++)
		{
			buff[i] = SPI_Get_Byte(spi1);
		}
		MMCCS(1);
		//printf( "OCR return: %x %x %x %x....\n\n", buff[0],buff[1],buff[2],buff[3] );

		if (buff[0] & 0x40)
		{
			SD_Type = SD_TYPE_V2HC;
			//printf( "card is V2.0 SDHC.....\n\n" );
		}
		else
		{
			SD_Type = SD_TYPE_V2;
			//printf( "card is V2.0.....\n\n" );
		}
		while (MMCWriteCmd(CMD16, 512, 0xff) != 0)
			;
		MMCWriteCmd(CMD9, 0, 0xff);
	}
	SSP0HighSpeed(); //back to high speed
	MMCCS(1);
	return 0;
}

/**************************************************************************************
* FunctionName   : MMCCSD_CID()
* Description    : 
* EntryParameter : cmd :CMD9 - CSD ; CMD10 - CID ; buf - buffer
* ReturnValue    :  1 failure, 0 success
**************************************************************************************/
uint8 MMCCSD_CID(uint8 cmd, uint8 *buf)
{
	uint8 i;

	if (MMCWriteCmd(cmd, 0x00, 0xFF) != 0x00)
	{
		return 1;
	}

	for (i = 0; i < 16; i++)
	{
		*buf++ = SPI_Send_Byte(spi1, 0xFF);
	}

	return 0;
}

/**************************************************************************************
* FunctionName   : MMCReadSingleBolck()
* Description    : read a block data from SD card
* EntryParameter : None
* ReturnValue    : 1 failure, 0 success
**************************************************************************************/
uint8 MMCReadSingleBolck(uint32 addr, uint8 *buf)
{
	uint16 i;
	uint8 sta;
	if (SD_Type != SD_TYPE_V2HC)
	{
		addr = addr << 9;
	}

	sta = MMCWriteCmd(CMD17, addr, 0x01);
	while (sta != 0)
	{
		sta = MMCWriteCmd(CMD17, addr, 0x01);
	}

	while (SPI_Get_Byte(spi1) != 0xFE)
	{
		;
	}

	if (sta == 0)
	{
		for (i = 0; i < 512; i++)
		{
			buf[i] = SPI_Send_Byte(spi1, 0xFF);
		}
	}

	SPI_Send_Byte(spi1, 0xFF);
	SPI_Send_Byte(spi1, 0xFF);

	MMCCS(1);
	return 0;
}

/**************************************************************************************
* FunctionName   : MMCReadMultipleBolck()
* Description    : read multple block data from SD card
* EntryParameter : None
* ReturnValue    : 1 failure, 0 success
**************************************************************************************/
uint8 MMCReadMultipleBolck(uint32 addr, uint8 *buf, uint8 count)
{
	uint16 i;
	if (SD_Type != SD_TYPE_V2HC)
	{
		addr = addr << 9;
	}

	if (MMCWriteCmd(CMD18, addr, 0xFF) != 0x00)
	{
		return 1;
	}

	MMCCS(0);
	do
	{
		while (SPI_Send_Byte(spi1, 0xFF) != 0xFE)
		{
			;
		}

		for (i = 0; i < 512; i++)
		{
			*buf++ = SPI_Send_Byte(spi1, 0xFF);
		}

		SPI_Send_Byte(spi1, 0xFF);
		SPI_Send_Byte(spi1, 0xFF);

	} while (--count);

	MMCCS(1);
	MMCWriteCmd(CMD12, 0x00, 0xFF);
	SPI_Send_Byte(spi1, 0xFF); //delay
	return 0;
}

/**************************************************************************************
* FunctionName   : MMCWriteSingleBlock()
* Description    : write a block data to SD card
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
uint8 MMCWriteSingleBlock(uint32 addr, const uint8 *buf)
{
	uint16 i, retry;
	uint8 temp;

	if (SD_Type != SD_TYPE_V2HC)
	{
		addr = addr << 9;
	}

	if (MMCWriteCmd(CMD24, addr, 0x01) != 0x00)
	{
		return 1;
	}

	MMCCS(0);
	//wait SD card ready
	SPI_Send_Byte(spi1, 0xFF);
	SPI_Send_Byte(spi1, 0xFF);
	SPI_Send_Byte(spi1, 0xFF);
	SPI_Send_Byte(spi1, 0xFE);

	for (i = 0; i < 512; i++)
	{
		SPI_Send_Byte(spi1, buf[i]);
	}
	//Dummy CRC
	SPI_Send_Byte(spi1, 0xFF);
	SPI_Send_Byte(spi1, 0xFF);

	temp = SPI_Send_Byte(spi1, 0xFF);
	temp &= 0x1F;

	if (temp != 0x05)
	{
		MMCCS(1);
		return 1;
	}

	while (SPI_Send_Byte(spi1, 0xFF) == 0x00)
	{
		retry++;

		if (retry > 0xfffe)
		{
			MMCCS(1);
			return 1;
		}
	}

	MMCCS(1);
	SPI_Send_Byte(spi1, 0xFF);
	return 0;
}

uint8 SD_WaitReady(void)
{
	uint8 r1;
	uint16 retry;
	retry = 0;
	do
	{
		r1 = SPI_Send_Byte(spi1, 0xFF);
		if (retry == 0xfffe)
			return 1;
	} while (r1 != 0xFF);
	return 0;
}

/**************************************************************************************
* FunctionName   : MMCWriteMultipleBlock()
* Description    : write multiple block data to SD card
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
uint8 MMCWriteMultipleBlock(uint32 addr, const uint8 *buf, uint8 count)
{
	uint16 i;
	uint8 tmp;
	if (SD_Type != SD_TYPE_V2HC)
	{
		addr = addr << 9;
	}
	if (SD_Type != SD_TYPE_MMC)
	{
		tmp = MMCWriteCmd(CMD23, count, 0x00);
	}
	if (MMCWriteCmd(CMD25, addr, 0xFF) != 0x00)
	{
		return 1;
	}

	MMCCS(0);
	SPI_Send_Byte(spi1, 0xFF);
	SPI_Send_Byte(spi1, 0xFF);

	do
	{
		SPI_Send_Byte(spi1, 0xFC);

		for (i = 0; i < 512; i++)
		{
			SPI_Send_Byte(spi1, *buf++);
		}
		SPI_Send_Byte(spi1, 0xFF);
		SPI_Send_Byte(spi1, 0xFF);
		tmp = SPI_Send_Byte(spi1, 0xFF);
		tmp &= 0x1F;
		if (tmp != 0x05)
		{
			MMCCS(1);
			return 1;
		}

		while (SD_WaitReady() == 0x01)
		{
			MMCCS(1);
			return 1;
		}
	} while (--count);

	SPI_Send_Byte(spi1, 0xFD); // send 'stop transmission token'
	if (SD_WaitReady())
	{
		while (1)
		{
		}
	}
	MMCCS(1);
	SPI_Send_Byte(spi1, 0xFF);

	return 0;
}

/**************************************************************************************
*                                         End Of File                  
**************************************************************************************/

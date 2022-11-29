/*
 *   Mops.c
 *
 *   Copyright 2010-2011 Olivier De Smet
 */

//
//   Low level chipset, bus and CPU stuff based on docs
//
//

#include "StdAfx.h"
#include "HP98x6.h"
// #include "kml.h"
#include "mops.h"

// prom for a 9816A (not included in a real 9816A)
static BYTE prom16a[256] = {
	0x00,0x00,		// checksum
	0x00,			// 256 bytes idprom
	 '2', '0', '1', '0', 'A', '0', '0', '0', '0', '0', '0',		// serial in ascii DDDDCSSSSSS date code, country, serial number
	 '9', '8', '1', '6', 'A', ' ', ' ',							// product number
	0xff,			// 8 bits processor board config														
	0x01,			// keyboard 98203B
	0x02,			// CRT alpha see crtid for monitor
	0x03,			// HP-IB
	0x04,			// Graphics
	0xff,			// end 
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,0xfe,0x00,0x00,			// bottom minimun address for ram size
	0xff,0xff,						// 16 required IO cards here not used
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,0xff,0xff,			// boot msus to try before Boot list scan
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// Boot file name
	0x00,0x00,0x00,0x00,			// delay in millisec before boot scan
	0x00,							// owner byte, HP format
	0x00,							// id prom revision byte : 0x00
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,									// rest reserved at 0xFF
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
// prom for a 9826A
static BYTE prom26a[256] = {
	0x00,0x00,		// checksum
	0x00,			// 256 bytes idprom
	 '2', '0', '1', '0', 'A', '0', '0', '0', '0', '0', '0',		// serial in ascii DDDDCSSSSSS date code, country, serial number
	 '9', '8', '2', '6', 'A', ' ', ' ',							// product number
	0xff,			// 8 bits processor board config														
	0x01,			// keyboard 98203B
	0x02,			// CRT alpha see crtid for monitor
	0x03,			// HP-IB
	0x04,			// graphics
	0x05,			// mass storage 1 drive floppy
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,0xfe,0x00,0x00,			// bottom minimun address for ram size
	0xff,0xff,						// 16 required IO cards here not used
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,0xff,0xff,			// boot msus to try before Boot list scan
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// Boot file name
	0x00,0x00,0x00,0x00,			// delay in millisec before boot scan
	0x00,							// owner byte, HP format
	0x00,							// id prom revision byte : 0x00
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,									// rest reserved at 0xFF
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
// prom for a 9836A
static BYTE prom36a[256] = {
	0x00,0x00,		// checksum
	0x00,			// 256 bytes idprom
	 '2', '0', '1', '0', 'A', '0', '0', '0', '0', '0', '0',		// serial in ascii DDDDCSSSSSS date code, country, serial number
	 '9', '8', '3', '6', 'A', ' ', ' ',							// product number
	0xff,			// 8 bits processor board config														
	0x01,			// keyboard 98203B
	0x02,			// CRT alpha see crtid for monitor
	0x03,			// HP-IB
	0x04,			// graphics
	0x15,			// mass storage 2 drives floppies
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,0xfe,0x00,0x00,			// bottom minimun address for ram size
	0xff,0xff,						// 16 required IO cards here not used
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,0xff,0xff,			// boot msus to try before Boot list scan
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// Boot file name
	0x00,0x00,0x00,0x00,			// delay in millisec before boot scan
	0x00,							// owner byte, HP format
	0x00,							// id prom revision byte : 0x00
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,									// rest reserved at 0xFF
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
// prom for a 9836C
static BYTE prom36c[256] = {
	0x00,0x00,		// checksum
	0x00,			// 256 bytes idprom
	 '2', '4', '4', '1', 'A', '0', '3', '9', '8', '6', ' ',		// serial in ascii DDDDCSSSSSS date code, country, serial number
	 '9', '8', '3', '6', 'C', ' ', ' ',							// product number
	0xff,			// 8 bits processor board config														
	0x01,			// keyboard 98203B
	0x32,			// CRT alpha see crtid for monitor
	0x03,			// HP-IB
	0x04,			// graphics on a 236C monitor 
	0x15,			// mass storage 2 drives floppies
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,0xfe,0x00,0x00,			// bottom minimun address for ram size
	0xff,0xff,						// 16 required IO cards here not used
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,0xff,0xff,			// boot msus to try before Boot list scan
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// Boot file name
	0x00,0x00,0x00,0x00,			// delay in millisec before boot scan
	0x00,							// owner byte, HP format
	0x00,							// id prom revision byte : 0x00
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,									// rest reserved at 0xFF
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
// prom for a 9837
static BYTE prom37a[256] = {
	0x00,0x00,		// checksum
	0x00,			// 256 bytes idprom
	 '2', '0', '1', '0', 'A', '0', '0', '0', '0', '0', '0',		// serial in ascii DDDDCSSSSSS date code, country, serial number
	 '9', '8', '3', '7', 'A', ' ', ' ',							// product number
	0xff,			// 8 bits processor board config														
	0x01,			// keyboard 98203B
	0x03,			// HP-IB
	0x09,			// high res display 
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,0xfe,0x00,0x00,			// bottom minimun address for ram size
	0xff,0xff,						// 16 required IO cards here not used
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,
	0xff,0xff,0xff,0xff,			// boot msus to try before Boot list scan
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	// Boot file name
	0x00,0x00,0x00,0x00,			// delay in millisec before boot scan
	0x00,							// owner byte, HP format
	0x00,							// id prom revision byte : 0x00
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,									// rest reserved at 0xFF
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};

//
// calc good prom crc
//
static VOID CrcIDProm(LPBYTE prom)
{
	DWORD sodd;
	WORD i;
	
	sodd = 0;
	for (i=0; i < 256; i+=2) {
		sodd += ((prom[i] << 8) | prom[i+1]);
		if (sodd & 0x10000) sodd++;
		sodd &= 0xFFFF;
	}
	sodd++;
	sodd &= 0xFFFF;
	if (sodd != 0x0000) {
		sodd = (0x10000 - sodd);
		prom[0] = (BYTE) (sodd >> 8);
		prom[1] = (BYTE) (sodd & 0xFF);
	}
}

//################
//#
//#    Public functions
//#
//################

//
// reset whole system
//
VOID SystemReset(VOID)							// register setting after System Reset
{
	Chipset.Cpu.A[0].l = 0x00000000;
	Chipset.Cpu.A[1].l = 0x00000000;
	Chipset.Cpu.A[2].l = 0x00000000;
	Chipset.Cpu.A[3].l = 0x00000000;
	Chipset.Cpu.A[4].l = 0x00000000;
	Chipset.Cpu.A[5].l = 0x00000000;
	Chipset.Cpu.A[6].l = 0x00000000;
	Chipset.Cpu.A[7].l = 0x00000000;
	Chipset.Cpu.A[8].l = 0x00000000;
	Chipset.Cpu.D[0].l = 0x00000000;
	Chipset.Cpu.D[1].l = 0x00000000;
	Chipset.Cpu.D[2].l = 0x00000000;
	Chipset.Cpu.D[3].l = 0x00000000;
	Chipset.Cpu.D[4].l = 0x00000000;
	Chipset.Cpu.D[5].l = 0x00000000;
	Chipset.Cpu.D[6].l = 0x00000000;
	Chipset.Cpu.D[7].l = 0x00000000;

	Chipset.Cpu.SR.sr = 0x2700;

	Chipset.Cpu.State = EXCEPTION;
	Chipset.Cpu.lastVector = 0;

	Chipset.I210 = 0;
	Chipset.cycles = 0x00000000;
	Chipset.dcycles = 0x0000;
	Chipset.ccycles = 0x00000000;

	ZeroMemory(Chipset.Ram, Chipset.RamSize);
	
	Chipset.Keyboard.ram[0x12] = 0x00;				// us keyboard
	Chipset.annun &= 0x00FFFFFF;

// for test only (reload a new rom at reset
	UnmapRom();
	_ASSERT(Chipset.Rom != NULL);
	MapRom(szCurrentDirectory);

// load initila pc and ssp
	ReadMEM((BYTE *)& Chipset.Cpu.PC, 0x000004, 4);
	ReadMEM((BYTE *)& Chipset.Cpu.A[8], 0x000000, 4);

	// do the crcs
	CrcIDProm(prom16a);
	CrcIDProm(prom26a);
	CrcIDProm(prom36a);
	CrcIDProm(prom36c);
	CrcIDProm(prom37a);

	hpib_stop_bus();								// stop hpib bus activity
	hp9130_stop();									// stop internal floppy activity

	hpib_init_bus();								// initialize hpib bus
	Reset_Keyboard();								// reset the keyboard 8041 emulation

// end of test

	UpdateMainDisplay(TRUE);						// refresh whole screen
	hpib_names();									// display lif names
	UpdateAnnunciators(TRUE);						// update all annutiators
}

//
// read byte from prom and switch1 byte
//
static BYTE Read_Prom(BYTE *a, WORD d, BYTE s)
{
		while (s) {
			if (d & 0x0001) {
				if (Chipset.type == 16)
					*(--a) = prom16a[(d>>1)&0xFF];				// PROM reading 
				else if (Chipset.type == 26)
					*(--a) = prom26a[(d>>1)&0xFF];				// PROM reading 
				else if (Chipset.type == 35)
					*(--a) = prom36c[(d>>1)&0xFF];				// PROM reading 
				else if (Chipset.type == 36)
					*(--a) = prom36a[(d>>1)&0xFF];				// PROM reading 
				else if (Chipset.type == 37)
					*(--a) = prom37a[(d>>1)&0xFF];				// PROM reading 
			} else
				*(--a) = Chipset.switch1;						// switch reading
			s--;
		}
		return BUS_OK;		
}

////////////////////////////////////////////////////////////////////////////////
//
//   Bus Commands: for mc68000
//		be carefull for endianness
//
////////////////////////////////////////////////////////////////////////////////

// 
// read a word from RAM and ROM only for debug and disasm
//
WORD GetWORD(DWORD d)
{
	DWORD ad = d & 0x00FFFFFF;
	WORD data = 0xFFFF;
	BYTE *a = (BYTE *) &data;

	a += 2;
	if (ad < (Chipset.RomSize - 1))												// ROM
	{
		*(--a) = Chipset.Rom[ad++];
		*(--a) = Chipset.Rom[ad++];
	}
	else if ((ad >= Chipset.RamStart) && (ad < 0x00FFFFFF))						// RAM
	{
		ad -= Chipset.RamStart;

		*(--a) = Chipset.Ram[ad++];
		*(--a) = Chipset.Ram[ad++];
	}
	return data;
}
//
// read at address d to *a with s byte size
//   return BUS_OK, BUS_ERROR or ADRESS_ERROR
//
BYTE ReadMEM(BYTE *a, DWORD d, BYTE s)
{
	DWORD ad = d & 0x00FFFFFF;
	BYTE sc = (BYTE) (ad >> 16);
	WORD dw = (WORD) (ad & 0x00FFFF);

	Chipset.Cpu.lastMEM = ad;			// address of last access for error
	Chipset.Cpu.lastRW = 0x10;			// read access							

	if ((s > 1) && (ad & 0x00000001))						// odd access as word
		return ADDRESS_ERROR;

	a += s;
	if (ad <= Chipset.RomSize-s) {												// ROM
		while (s) {
			*(--a) = Chipset.Rom[ad++]; s--;
		}
		return BUS_OK;
	} else if ((ad >= Chipset.RamStart) & (ad <= (unsigned)(0x01000000-s)))	{	// RAM
		ad -= Chipset.RamStart;
		while (s) {
			*(--a) = Chipset.Ram[ad++]; s--;
		}
		return BUS_OK;
	}
	// internal I/O from $400000 to $5FFFFF
	else {
		switch (sc) {
			case 0x30:
			case 0x31:
			case 0x32:
			case 0x33:
			case 0x34:
			case 0x35:
			case 0x36:
			case 0x37:
			case 0x38:
			case 0x39:
			case 0x3A:
			case 0x3B:
			case 0x3C:
			case 0x3D:
			case 0x3E:
			case 0x3F:
				if (Chipset.type == 37) {		// Display graph 37 mem
					return Read_Graph37(a, ad & 0x000FFFFF, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x42:							// Keyboard						SC 2
				return Read_Keyboard(a, dw, s);
				break;
			case 0x44:							// Internal disc 9130
				if ((Chipset.type == 36) || (Chipset.type == 35)) {
					return Read_9130(a, dw, s);
				} else
					return BUS_ERROR;
				break;
			case 0x47:							// Internal HPIB ($478000)		SC 7
				return Read_HPIB(a, dw, s);
				break;
			case 0x51:							// Display alpha				SC 1
				if (Chipset.type == 35) {
					return Read_Display36c(a, dw, s);
				} else if (Chipset.type != 37) {
					return Read_Display16(a, dw, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x52:							// Display graph 36c
				if (Chipset.type == 35) {
					return Read_Graph36c(a, ad & 0x0000FFFF, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x53:							// Display graph 16			
				if (Chipset.type == 35) {
					return Read_Graph36c(a, ad & 0x0001FFFF, s);
				} else if (Chipset.type != 37) { 
					return Read_Graph16(a, dw, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x54:							// Display graph 36c
			case 0x55:
				if (Chipset.type == 35) {
					return Read_Graph36c(a, (ad & 0x0001FFFF) | 0x00020000, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x56:
				if (Chipset.type == 37) { 		// Display graph 37 ctrl
					return Read_Display37(a, dw, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x5C:							// 98635 float card
				if (Chipset.Hp98635) 
					return Read_98635(a, dw, s);
				else 
					return BUS_ERROR;
				break;
			case 0x5F:							// ID PROM & SWITCH 
				if (Chipset.type == 16) {
					return BUS_ERROR;
				} else {
					if (dw < 0x4000) {
						return Read_Prom(a, dw, s);			// (from 0x0000 to 0x3FFF)
					} else 
						return BUS_ERROR;
				}
				break;
			// external I/O
			case 0x69:								// 216/217 rs-232 98626			SC 9
				if (Chipset.type == 16) {
					return Read_98626(a, dw, s);
				} else
					return BUS_ERROR;
				break;
			default:
				return BUS_ERROR;
				break;
		}
	}
	return BUS_ERROR;						// nothing mapped
}

//
// write at address d from *a with s byte size
//

BYTE WriteMEM(BYTE *a, DWORD d, BYTE s)
{
	DWORD ad = d & 0x00FFFFFF;
	BYTE sc = (BYTE) (ad >> 16);
	WORD dw = (WORD) (ad & 0x00FFFF);

	Chipset.Cpu.lastMEM = ad;			// address of last access for error
	Chipset.Cpu.lastRW = 0x00;			// write access							

	if ((s > 1) && (ad & 0x00000001))						// odd access as word
		return ADDRESS_ERROR;

	a += s;
	if (ad <= (unsigned)(0x002FFFFF-s)) {								// ROM (up to $2FFFFF) for 9837 ...
		while (s) {
			if (s & 0x01) {
				Chipset.annun &= 0x00FFFFFF;
				Chipset.annun |= (~(*(--a))) << 24;						// low byte is diagnostic leds
			}
			s--;
		}
		return BUS_OK;												// do nothing but no error
	}
	else if ((ad >= Chipset.RamStart) & (ad <= (unsigned)(0x01000000-s)))	// RAM (from $880000)
	{
		ad -= Chipset.RamStart;
		while (s) {
			Chipset.Ram[ad++] = *(--a); s--;
		} 
		return BUS_OK;
	}
	// internal I/O from $400000 to $5FFFFF
	else {
		switch (sc) {
			case 0x30:
			case 0x31:
			case 0x32:
			case 0x33:
			case 0x34:
			case 0x35:
			case 0x36:
			case 0x37:
			case 0x38:
			case 0x39:
			case 0x3A:
			case 0x3B:
			case 0x3C:
			case 0x3D:
			case 0x3E:
			case 0x3F:
				if (Chipset.type == 37) {		// Display graph 37 mem
					return Write_Graph37(a, ad & 0x000FFFFF, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x42:						// Keyboard						SC 2
				return Write_Keyboard(a, dw, s);
				break;
			case 0x44:						// Internal disc
				if ((Chipset.type == 36) || (Chipset.type == 35)) {
					return Write_9130(a, dw, s);
				} else
					return BUS_ERROR;
				break;
			case 0x47:						// Internal HPIB ($478000)		SC 7
				return Write_HPIB(a, dw, s);
				break;
			case 0x51:						// Display						SC 1
				if (Chipset.type == 35) {
					return Write_Display36c(a, dw, s);
				} else if (Chipset.type != 37) { 
					return Write_Display16(a, dw, s);
				} else
					return BUS_ERROR;
				break;
			case 0x52:							// Display graph 36c
				if (Chipset.type == 35) {
					return Write_Graph36c(a, ad & 0x0000FFFF, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x53:						// Display graph 16			
				if (Chipset.type == 35) {
					return Write_Graph36c(a, ad & 0x0001FFFF, s);
				} else if (Chipset.type != 37) { 
					return Write_Graph16(a, dw, s);
				} else
					return BUS_ERROR;
				break;
			case 0x54:							// Display graph 36c
			case 0x55:
				if (Chipset.type == 35) {
					return Write_Graph36c(a, (ad & 0x0001FFFF) | 0x00020000, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x56:
				if (Chipset.type == 37) { 		// Display graph 37 ctrl
					return Write_Display37(a, dw, s);
				} else 
					return BUS_ERROR;
				break;
			case 0x5C:							// 98635 float card
				if (Chipset.Hp98635) 
					return Write_98635(a, dw, s);
				else
					return BUS_ERROR;
				break;
			// external I/O
			case 0x69: 						// 216/217 rs-232 98626			SC 9
				if (Chipset.type == 16) {
					return Write_98626(a, dw, s);
				} else
					return BUS_ERROR;
				break;
			default:
				return BUS_ERROR;
				break;
		}
	}		
	return BUS_ERROR;						// nothing mapped
}

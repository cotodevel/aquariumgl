/*

			Copyright (C) 2017  Coto
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
USA

*/

//TGDS required version: IPC Version: 1.3

//IPC FIFO Description: 
//		struct sIPCSharedTGDS * TGDSIPC = TGDSIPCStartAddress; 														// Access to TGDS internal IPC FIFO structure. 		(ipcfifoTGDS.h)
//		struct sIPCSharedTGDSSpecific * TGDSUSERIPC = (struct sIPCSharedTGDSSpecific *)TGDSIPCUserStartAddress;		// Access to TGDS Project (User) IPC FIFO structure	(ipcfifoTGDSUser.h)

//inherits what is defined in: ipcfifoTGDS.h
#ifndef __ipcfifoTGDSUser_h__
#define __ipcfifoTGDSUser_h__

#include "dsregs.h"
#include "dsregs_asm.h"
#include "ipcfifoTGDS.h"
#include "dswnifi.h"
#include "posixHandleTGDS.h"

//---------------------------------------------------------------------------------
struct sIPCSharedTGDSSpecific {
//---------------------------------------------------------------------------------
	char filename[256];
};

//TGDS Memory Layout ARM7/ARM9 Cores
#define TGDS_ARM7_MALLOCSTART (u32)(0x06018000)
#define TGDS_ARM7_MALLOCSIZE (int)(16*1024)
#define TGDSDLDI_ARM7_ADDRESS (u32)(TGDS_ARM7_MALLOCSTART + TGDS_ARM7_MALLOCSIZE)	//0x0601C000
#define TGDS_ARM7_AUDIOBUFFER_STREAM (u32)(0x03800000)

#define FIFO_PLAYSOUNDSTREAM_FILE (u32)(0xFFFFABCB)
#define FIFO_STOPSOUNDSTREAM_FILE (u32)(0xFFFFABCC)
#define FIFO_PLAYSOUNDEFFECT_FILE (u32)(0xFFFFABCD)

#define NO_VIDEO_PLAYBACK	1

#endif

#ifdef __cplusplus
extern "C" {
#endif

//NOT weak symbols : the implementation of these is project-defined (here)
extern void HandleFifoNotEmptyWeakRef(u32 cmd1, uint32 cmd2);
extern void HandleFifoEmptyWeakRef(uint32 cmd1,uint32 cmd2);
extern struct sIPCSharedTGDSSpecific* getsIPCSharedTGDSSpecific();

extern bool soundGameOverEmitted;
extern void gameoverSound();

extern void MunchFoodSound();

extern void BgMusic(char * filename);
extern void BgMusicOff();
extern bool bgMusicEnabled;

#ifdef ARM9
extern u32 playSoundStreamFromFile(char * videoStructFDFilename, bool loop, u32 streamType);
#endif

#ifdef __cplusplus
}
#endif
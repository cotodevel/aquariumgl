
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

#include "ipcfifoTGDS.h"
#include "ipcfifoTGDSUser.h"
#include "dsregs.h"
#include "dsregs_asm.h"
#include "InterruptsARMCores_h.h"
#include "libutilsShared.h"
#include "microphoneShared.h"

#ifdef ARM7
#include <string.h>

#include "main.h"
#include "wifi_arm7.h"
#include "spifwTGDS.h"
#endif

#ifdef ARM9

#include <stdbool.h>
#include "main.h"
#include "wifi_arm9.h"
#include "dswnifi_lib.h"
#include "soundTGDS.h"
#include "biosTGDS.h"
#endif

#ifdef ARM9
__attribute__((section(".itcm")))
#endif
struct sIPCSharedTGDSSpecific* getsIPCSharedTGDSSpecific(){
	struct sIPCSharedTGDSSpecific* sIPCSharedTGDSSpecificInst = (struct sIPCSharedTGDSSpecific*)(TGDSIPCUserStartAddress);
	return sIPCSharedTGDSSpecificInst;
}

//inherits what is defined in: ipcfifoTGDS.c
#ifdef ARM9
__attribute__((section(".itcm")))
#endif
void HandleFifoNotEmptyWeakRef(u32 cmd1, uint32 cmd2){
	switch (cmd1) {
		#ifdef ARM7

		#endif
		
		#ifdef ARM9
		
		#endif
	}
}

#ifdef ARM9
__attribute__((section(".itcm")))
#endif
void HandleFifoEmptyWeakRef(uint32 cmd1,uint32 cmd2){
}

//project specific stuff

#ifdef ARM9

void BgMusic(){
	REG_IE = REG_IE & ~(IRQ_VBLANK);
	REG_IE |= IRQ_VCOUNT;
	
	//Play WAV/ADPCM 
	memset(curChosenBrowseFile, 0, sizeof(curChosenBrowseFile));
	strcpy(curChosenBrowseFile, "0:/tank.ima");
	pendPlay = 1;
}

void BgMusicOff(){
	pendPlay = 2;
}


void updateStreamCustomDecoder(u32 srcFrmt){

}

void freeSoundCustomDecoder(u32 srcFrmt){

}
#endif

//Libutils setup: TGDS project uses Soundstream, WIFI, ARM7 malloc, etc.
void setupLibUtils(){
	//libutils:
	
	//Stage 0
	#ifdef ARM9
	initializeLibUtils9(
		(HandleFifoNotEmptyWeakRefLibUtils_fn)&libUtilsFIFONotEmpty, //ARM7 & ARM9
		NULL, //ARM9 : Timer_50ms();
		(SoundStreamStopSoundStreamARM9LibUtils_fn)&stopSoundStream,	//ARM9: bool stopSoundStream(struct fd * tgdsStructFD1, struct fd * tgdsStructFD2, int * internalCodecType)
		(SoundStreamUpdateSoundStreamARM9LibUtils_fn)&updateStream, //ARM9: void updateStream() 
		NULL, //ARM7 & ARM9: DeInitWIFI()
		NULL //ARM9: bool switch_dswnifi_mode(sint32 mode)
	);
	#endif
	
	//Stage 1
	#ifdef ARM7
	initializeLibUtils7(
		(HandleFifoNotEmptyWeakRefLibUtils_fn)&libUtilsFIFONotEmpty, //ARM7 & ARM9
		NULL, //ARM7: wifiUpdateVBLANKARM7LibUtils_fn
		NULL, //ARM7: wifiInterruptARM7LibUtils_fn
		&TIMER1Handler, //ARM7: 
		&stopSound, //ARM7: 
		&setupSound, //ARM7: 
		(initMallocARM7LibUtils_fn)&initARM7Malloc, //ARM7: void initARM7Malloc(u32 ARM7MallocStartaddress, u32 ARM7MallocSize);
		NULL, //ARM7 & ARM9: DeInitWIFI()
		NULL  //ARM7: micInterrupt()
	);
	#endif
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cpp3ds/Graphics.hpp>

extern "C" {
#include <ctr/types.h>
#include <ctr/srv.h>
#include <ctr/APT.h>
#include <ctr/GSP.h>
#include <ctr/GX.h>
#include <ctr/GPU.h>
#include <ctr/HID.h>
#include <ctr/SHDR.h>
#include <ctr/svc.h>
}

extern "C" void cpp3ds_main();

u8* gspHeap;
u32* gxCmdBuf;

u8 currentBuffer;
u8* topLeftFramebuffers[2];

Handle gspEvent, gspSharedMemHandle;

s32 costable[] = {4096, 4095, 4094, 4093, 4091, 4088, 4084, 4080, 4076, 4071, 4065, 4058, 4051, 4044, 4035, 4026, 4017, 4007, 3996, 3985, 3973, 3960, 3947, 3934, 3919, 3904, 3889, 3873, 3856, 3839, 3821, 3803, 3784, 3765, 3744, 3724, 3703, 3681, 3659, 3636, 3612, 3588, 3564, 3539, 3513, 3487, 3461, 3434, 3406, 3378, 3349, 3320, 3290, 3260, 3229, 3198, 3167, 3135, 3102, 3069, 3035, 3002, 2967, 2932, 2897, 2861, 2825, 2788, 2751, 2714, 2676, 2638, 2599, 2560, 2521, 2481, 2441, 2401, 2360, 2318, 2277, 2235, 2193, 2150, 2107, 2064, 2020, 1976, 1932, 1888, 1843, 1798, 1753, 1707, 1662, 1616, 1569, 1523, 1476, 1429, 1382, 1334, 1287, 1239, 1191, 1143, 1095, 1046, 997, 949, 900, 851, 801, 752, 703, 653, 603, 554, 504, 454, 404, 354, 304, 254, 204, 153, 103, 53, 3, -46, -97, -147, -197, -247, -297, -347, -398, -448, -497, -547, -597, -647, -696, -746, -795, -844, -893, -942, -991, -1040, -1088, -1137, -1185, -1233, -1281, -1328, -1376, -1423, -1470, -1517, -1563, -1610, -1656, -1701, -1747, -1792, -1837, -1882, -1927, -1971, -2015, -2058, -2102, -2144, -2187, -2229, -2271, -2313, -2354, -2395, -2436, -2476, -2516, -2555, -2594, -2633, -2671, -2709, -2747, -2784, -2820, -2857, -2892, -2928, -2963, -2997, -3031, -3065, -3098, -3130, -3163, -3194, -3225, -3256, -3286, -3316, -3345, -3374, -3402, -3430, -3457, -3484, -3510, -3536, -3561, -3585, -3609, -3633, -3656, -3678, -3700, -3721, -3742, -3762, -3782, -3801, -3819, -3837, -3854, -3871, -3887, -3902, -3917, -3932, -3946, -3959, -3971, -3983, -3995, -4005, -4016, -4025, -4034, -4042, -4050, -4057, -4064, -4070, -4075, -4080, -4084, -4087, -4090, -4092, -4094, -4095, -4095, -4095, -4094, -4093, -4091, -4088, -4085, -4081, -4076, -4071, -4066, -4059, -4052, -4045, -4036, -4028, -4018, -4008, -3997, -3986, -3974, -3962, -3949, -3935, -3921, -3906, -3891, -3875, -3858, -3841, -3824, -3805, -3787, -3767, -3747, -3727, -3705, -3684, -3662, -3639, -3615, -3592, -3567, -3542, -3517, -3491, -3464, -3437, -3409, -3381, -3353, -3324, -3294, -3264, -3233, -3202, -3171, -3139, -3106, -3073, -3040, -3006, -2972, -2937, -2902, -2866, -2830, -2793, -2756, -2719, -2681, -2643, -2604, -2565, -2526, -2486, -2446, -2406, -2365, -2324, -2282, -2240, -2198, -2156, -2113, -2069, -2026, -1982, -1938, -1894, -1849, -1804, -1759, -1713, -1668, -1622, -1575, -1529, -1482, -1435, -1388, -1341, -1293, -1245, -1197, -1149, -1101, -1052, -1004, -955, -906, -857, -808, -758, -709, -660, -610, -560, -510, -460, -411, -360, -310, -260, -210, -160, -110, -60, -9, 40, 90, 140, 191, 241, 291, 341, 391, 441, 491, 541, 591, 640, 690, 739, 789, 838, 887, 936, 985, 1033, 1082, 1130, 1179, 1227, 1274, 1322, 1370, 1417, 1464, 1511, 1557, 1604, 1650, 1695, 1741, 1786, 1831, 1876, 1921, 1965, 2009, 2053, 2096, 2139, 2182, 2224, 2266, 2308, 2349, 2390, 2431, 2471, 2511, 2550, 2589, 2628, 2666, 2704, 2742, 2779, 2816, 2852, 2888, 2923, 2958, 2993, 3027, 3060, 3093, 3126, 3158, 3190, 3221, 3252, 3282, 3312, 3342, 3370, 3399, 3426, 3454, 3480, 3507, 3532, 3557, 3582, 3606, 3630, 3653, 3675, 3697, 3718, 3739, 3759, 3779, 3798, 3817, 3835, 3852, 3869, 3885, 3900, 3915, 3930, 3944, 3957, 3970, 3982, 3993, 4004, 4014, 4024, 4033, 4041, 4049, 4056, 4063, 4069, 4074, 4079, 4083, 4087, 4090, 4092, 4094, 4095};

void setPixel(int x, int y){
	u8* bufAdr=&gspHeap[0x46500*currentBuffer];
	u32 v=(x+y*240)*3;
	bufAdr[v]=255;
	bufAdr[v+1]=0;
	bufAdr[v+2]=0;
}

void gspGpuInit()
{
	gspInit();

	GSPGPU_AcquireRight(nullptr, 0x0);
	GSPGPU_SetLcdForceBlack(nullptr, 0x0);

	//set subscreen to blue
	u32 regData=0x01FF0000;
	GSPGPU_WriteHWRegs(nullptr, 0x202A04, &regData, 4);

	//grab main left screen framebuffer addresses
	GSPGPU_ReadHWRegs(nullptr, 0x400468, (u32*)&topLeftFramebuffers, 8);

	//convert PA to VA (assuming FB in VRAM)
	topLeftFramebuffers[0]+=0x7000000;
	topLeftFramebuffers[1]+=0x7000000;

	//setup our gsp shared mem section
	u8 threadID;
	svc_createEvent(&gspEvent, 0x0);
	GSPGPU_RegisterInterruptRelayQueue(nullptr, gspEvent, 0x1, &gspSharedMemHandle, &threadID);
	svc_mapMemoryBlock(gspSharedMemHandle, 0x10002000, 0x3, 0x10000000);

	//map GSP heap
	svc_controlMemory((u32*)&gspHeap, 0x0, 0x0, 0x2000000, 0x10003, 0x3);

	//wait until we can write stuff to it
	svc_waitSynchronization1(gspEvent, 0x55bcb0);

	//GSP shared mem : 0x2779F000
	gxCmdBuf=(u32*)(0x10002000+0x800+threadID*0x200);

	currentBuffer=0;
}

void gspGpuExit()
{
	GSPGPU_UnregisterInterruptRelayQueue(NULL);

	//unmap GSP shared mem
	svc_unmapMemoryBlock(gspSharedMemHandle, 0x10002000);
	svc_closeHandle(gspSharedMemHandle);
	svc_closeHandle(gspEvent);

	gspExit();

	//free GSP heap
	svc_controlMemory((u32*)&gspHeap, (u32)gspHeap, 0x0, 0x2000000, MEMOP_FREE, 0x0);
}

void swapBuffers()
{
	u32 regData;
	GSPGPU_ReadHWRegs(nullptr, 0x400478, static_cast<u32*>(&regData), 4);
	regData^=1;
	currentBuffer=regData&1;
	GSPGPU_WriteHWRegs(nullptr, 0x400478, static_cast<u32*>(&regData), 4);
}

void copyBuffer()
{
	//copy topleft FB
	u8 copiedBuffer=currentBuffer^1;
	u8* bufAdr=&gspHeap[0x46500*copiedBuffer];
	GSPGPU_FlushDataCache(nullptr, bufAdr, 0x46500);

	GX_RequestDma(gxCmdBuf, (u32*)bufAdr, (u32*)topLeftFramebuffers[copiedBuffer], 0x46500);
}

s32 pcCos(u16 v)
{
	return costable[v&0x1FF];
}

u32 cnt;

void renderEffect()
{
	u8* bufAdr=&gspHeap[0x46500*currentBuffer];

	int i, j;
	for(i=1;i<400;i++)
	{
		for(j=1;j<240;j++)
		{
			u32 v=(j+i*240)*3;
			bufAdr[v]=(pcCos(i+cnt)+4096)/32;
			bufAdr[v+1]=(pcCos(j-256+cnt)+4096)/64;
			bufAdr[v+2]=(pcCos(i+128-cnt)+4096)/32;
		}
	}
	cnt++;
}

extern "C" int cpp3ds_init(){
//	cpp3ds::TopScreen screen;

	initSrv();
	aptInit(APPID_APPLICATION);
	gspGpuInit();
	hidInit(nullptr);
	aptSetupEventHandler();

//	if (false)
//		cpp3ds_main();

	APP_STATUS status;
	while((status=aptGetStatus())!=APP_EXITING)
	{
		if(status==APP_RUNNING)
		{
			u32 PAD=hidSharedMem[7];

			u32 regData=PAD|0x01000000;
			GSPGPU_WriteHWRegs(nullptr, 0x202A04, (u32*)&regData, 4);

			renderEffect();
			setPixel(5,5);

//			screen.setPixel(5, 5, {0,0,255});

			swapBuffers();
			copyBuffer();
		}
		svc_sleepThread(16666666);
	}

	hidExit();
	gspGpuExit();
	aptExit();
	svc_exitProcess();
	return 0;
}

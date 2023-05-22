#pragma once
#ifndef _FAT_H_
#define _FAT_H_

#include <types.h>

// FAT specifications:
// https://academy.cba.mit.edu/classes/networking_communications/SD/FAT.pdf

namespace FileSystem
{
    namespace Fat
    {
        constexpr char FILE_SYS_TYPE_FAT12[8]   {"FAT12  "};
        constexpr char FILE_SYS_TYPE_FAT16[8]   {"FAT16  "};
        constexpr char FILE_SYS_TYPE_FAT32[8]   {"FAT32  "};
        constexpr char FILE_SYS_TYPE_FAT[8]     {"FAT    "};

        constexpr u32  VALID_LEAD_SIG           {0x41615252};
                                                // WYSI
        constexpr u32  VALID_SIG_2              {0x61417272};

        constexpr u32  FAT_32_TRAIL_SIG         {0xAA550000};

        struct Fat32BPB {
            u8  jmpBoot[3];
            u8  oemName[8];
            u16 bytesPerSector;
            u8  secPerCluste;
            u16 rsvdSecCount;
            u8  numFats;
            u16 totSec16;
            u8  media;
            u16 fatSz16;
            u16 secPerTrk;
            u16 numHeads;
            u32 hdnSecCount;
            u32 totSec32;

            union {
                // FAT 12/16
                struct {
                    u8      drvNum;
                    u8      RESV1; // Must be 0x00
                    u8      bootSig;
                    u32     volID;
                    u8      volLabel[11];
                    char    FilSysType[8];
                    
                    //  448 bytes worth of '0x00'
                    u8  bootCode[448];

                    //  2 bytes dedicated to 0x55AA 
                    u16 bootPartSig;
                    
                    // TODO: Documentation says there is 
                    // {x} byes worth of '0x00' for all bytes left in sector (bytesPerSector > 512)
                    // here so I guess I will set the padding and sig word in the implementation code
                } FAT16;

                // FAT 32
                struct {
                    u32     fatSz32;
                    u16     extFlags;
                    u16     fsVersion;
                    u32     rootCluster;
                    u16     fsInfo;
                    u16     bkBootSect;
                    u8      RESV[12]; // Must be 0x00
                    u8      drvNum;
                    u8      RESV1; // Must be 0x00
                    u8      bootSig;
                    u32     volID;
                    u8      volLabel[11];
                    char    FileSysType[8];

                    //  420 bytes worth of '0x00'
                    u8  bootCode[420];

                    //  2 bytes dedicated to 0x55AA 
                    u16 bootPartSig;
                    
                    // TODO: Documentation says there is 
                    // {x} byes worth of '0x00' for all bytes left in sector (bytesPerSector > 512)
                    // here so I guess I will set the padding and sig word in the implementation code
                } FAT32;
            } description;
        };

        // FAT32 only
        struct FSInfo {
            u32 leadSig;
            u8 RESV0[480];
            u32 sig2; 
            u32 lstFreeClusterCount;
            u32 clusterNum;
            u8 RESV1[12];

        };
    }
}

#endif // _FAT_H_

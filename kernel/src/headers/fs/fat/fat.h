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
                    
                    // TODO: Documentation says there is 
                    //  448 bytes worth of '0x00'
                    //  2 bytes dedicated to 0x55AA afterwards
                    //  {x} byes worth of '0x00' for all bytes left in sector (bytesPerSector > 512)
                    //
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

                    // TODO: Documentation says there is 
                    //  420 bytes worth of '0x00'
                    //  2 bytes dedicated to 0x55AA afterwards
                    //  {x} byes worth of '0x00' for all bytes left in sector (bytesPerSector > 512)
                    //
                    // here so I guess I will set the padding and sig word in the implementation code
                } FAT32;
            } description;
        };
    }
}

#endif // _FAT_H_

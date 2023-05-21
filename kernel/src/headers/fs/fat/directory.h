#pragma once
#ifndef _FAT_DIRECTORY_H
#define _FAT_DIRECTORY_H

#include <types.h>

namespace FileSystem
{
    namespace Fat
    {
        constexpr u8 ATTR_READ_ONLY {0x01};
        constexpr u8 ATTR_HIDDEN    {0x02};
        constexpr u8 ATTR_SYSTEM    {0x04};
        constexpr u8 ATTR_VOLUME_ID {0x08};
        constexpr u8 ATTR_DIRECTORY {0x10};
        constexpr u8 ATTR_ARCHIVE   {0x20};

        struct Directory {
            char    name[11]; 
            u8      attr;
            u8      ntRESV;
            u8      crtTimeTenth;
            u16     crtTime;
            u16     crtDate;
            u16     lstAccDate;
            u16     fshClusHI;
            u16     wrtTime;
            u16     wrtDate;
            u16     fstClustLO;
            u32     fileSize;
        };
    }
}

#endif // _FAT_DIRECTORY_H

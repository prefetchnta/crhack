/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-18  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>> CrHack CRC-64/ECMA-182 哈希函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/* CRC-64/ECMA-182 常数表 (X^64 + X^62 + X^57 + X^55 +
   X^54 + X^53 + X^52 + X^47 + X^46 + X^45 + X^40 + X^39 +
   X^38 + X^37 + X^35 + X^33 + X^32 + X^31 + X^29 + X^27 +
   X^24 + X^23 + X^22 + X^21 + X^19 + X^17 + X^13 + X^12 +
   X^10 + X^9 + X^7 + X^4 + X^1 + 1) */
static const int64u _rom_ s_crc64[256] =
{
    CR_ULL(0x0000000000000000), CR_ULL(0x42F0E1EBA9EA3693),
    CR_ULL(0x85E1C3D753D46D26), CR_ULL(0xC711223CFA3E5BB5),
    CR_ULL(0x493366450E42ECDF), CR_ULL(0x0BC387AEA7A8DA4C),
    CR_ULL(0xCCD2A5925D9681F9), CR_ULL(0x8E224479F47CB76A),
    CR_ULL(0x9266CC8A1C85D9BE), CR_ULL(0xD0962D61B56FEF2D),
    CR_ULL(0x17870F5D4F51B498), CR_ULL(0x5577EEB6E6BB820B),
    CR_ULL(0xDB55AACF12C73561), CR_ULL(0x99A54B24BB2D03F2),
    CR_ULL(0x5EB4691841135847), CR_ULL(0x1C4488F3E8F96ED4),
    CR_ULL(0x663D78FF90E185EF), CR_ULL(0x24CD9914390BB37C),
    CR_ULL(0xE3DCBB28C335E8C9), CR_ULL(0xA12C5AC36ADFDE5A),
    CR_ULL(0x2F0E1EBA9EA36930), CR_ULL(0x6DFEFF5137495FA3),
    CR_ULL(0xAAEFDD6DCD770416), CR_ULL(0xE81F3C86649D3285),
    CR_ULL(0xF45BB4758C645C51), CR_ULL(0xB6AB559E258E6AC2),
    CR_ULL(0x71BA77A2DFB03177), CR_ULL(0x334A9649765A07E4),
    CR_ULL(0xBD68D2308226B08E), CR_ULL(0xFF9833DB2BCC861D),
    CR_ULL(0x388911E7D1F2DDA8), CR_ULL(0x7A79F00C7818EB3B),
    CR_ULL(0xCC7AF1FF21C30BDE), CR_ULL(0x8E8A101488293D4D),
    CR_ULL(0x499B3228721766F8), CR_ULL(0x0B6BD3C3DBFD506B),
    CR_ULL(0x854997BA2F81E701), CR_ULL(0xC7B97651866BD192),
    CR_ULL(0x00A8546D7C558A27), CR_ULL(0x4258B586D5BFBCB4),
    CR_ULL(0x5E1C3D753D46D260), CR_ULL(0x1CECDC9E94ACE4F3),
    CR_ULL(0xDBFDFEA26E92BF46), CR_ULL(0x990D1F49C77889D5),
    CR_ULL(0x172F5B3033043EBF), CR_ULL(0x55DFBADB9AEE082C),
    CR_ULL(0x92CE98E760D05399), CR_ULL(0xD03E790CC93A650A),
    CR_ULL(0xAA478900B1228E31), CR_ULL(0xE8B768EB18C8B8A2),
    CR_ULL(0x2FA64AD7E2F6E317), CR_ULL(0x6D56AB3C4B1CD584),
    CR_ULL(0xE374EF45BF6062EE), CR_ULL(0xA1840EAE168A547D),
    CR_ULL(0x66952C92ECB40FC8), CR_ULL(0x2465CD79455E395B),
    CR_ULL(0x3821458AADA7578F), CR_ULL(0x7AD1A461044D611C),
    CR_ULL(0xBDC0865DFE733AA9), CR_ULL(0xFF3067B657990C3A),
    CR_ULL(0x711223CFA3E5BB50), CR_ULL(0x33E2C2240A0F8DC3),
    CR_ULL(0xF4F3E018F031D676), CR_ULL(0xB60301F359DBE0E5),
    CR_ULL(0xDA050215EA6C212F), CR_ULL(0x98F5E3FE438617BC),
    CR_ULL(0x5FE4C1C2B9B84C09), CR_ULL(0x1D14202910527A9A),
    CR_ULL(0x93366450E42ECDF0), CR_ULL(0xD1C685BB4DC4FB63),
    CR_ULL(0x16D7A787B7FAA0D6), CR_ULL(0x5427466C1E109645),
    CR_ULL(0x4863CE9FF6E9F891), CR_ULL(0x0A932F745F03CE02),
    CR_ULL(0xCD820D48A53D95B7), CR_ULL(0x8F72ECA30CD7A324),
    CR_ULL(0x0150A8DAF8AB144E), CR_ULL(0x43A04931514122DD),
    CR_ULL(0x84B16B0DAB7F7968), CR_ULL(0xC6418AE602954FFB),
    CR_ULL(0xBC387AEA7A8DA4C0), CR_ULL(0xFEC89B01D3679253),
    CR_ULL(0x39D9B93D2959C9E6), CR_ULL(0x7B2958D680B3FF75),
    CR_ULL(0xF50B1CAF74CF481F), CR_ULL(0xB7FBFD44DD257E8C),
    CR_ULL(0x70EADF78271B2539), CR_ULL(0x321A3E938EF113AA),
    CR_ULL(0x2E5EB66066087D7E), CR_ULL(0x6CAE578BCFE24BED),
    CR_ULL(0xABBF75B735DC1058), CR_ULL(0xE94F945C9C3626CB),
    CR_ULL(0x676DD025684A91A1), CR_ULL(0x259D31CEC1A0A732),
    CR_ULL(0xE28C13F23B9EFC87), CR_ULL(0xA07CF2199274CA14),
    CR_ULL(0x167FF3EACBAF2AF1), CR_ULL(0x548F120162451C62),
    CR_ULL(0x939E303D987B47D7), CR_ULL(0xD16ED1D631917144),
    CR_ULL(0x5F4C95AFC5EDC62E), CR_ULL(0x1DBC74446C07F0BD),
    CR_ULL(0xDAAD56789639AB08), CR_ULL(0x985DB7933FD39D9B),
    CR_ULL(0x84193F60D72AF34F), CR_ULL(0xC6E9DE8B7EC0C5DC),
    CR_ULL(0x01F8FCB784FE9E69), CR_ULL(0x43081D5C2D14A8FA),
    CR_ULL(0xCD2A5925D9681F90), CR_ULL(0x8FDAB8CE70822903),
    CR_ULL(0x48CB9AF28ABC72B6), CR_ULL(0x0A3B7B1923564425),
    CR_ULL(0x70428B155B4EAF1E), CR_ULL(0x32B26AFEF2A4998D),
    CR_ULL(0xF5A348C2089AC238), CR_ULL(0xB753A929A170F4AB),
    CR_ULL(0x3971ED50550C43C1), CR_ULL(0x7B810CBBFCE67552),
    CR_ULL(0xBC902E8706D82EE7), CR_ULL(0xFE60CF6CAF321874),
    CR_ULL(0xE224479F47CB76A0), CR_ULL(0xA0D4A674EE214033),
    CR_ULL(0x67C58448141F1B86), CR_ULL(0x253565A3BDF52D15),
    CR_ULL(0xAB1721DA49899A7F), CR_ULL(0xE9E7C031E063ACEC),
    CR_ULL(0x2EF6E20D1A5DF759), CR_ULL(0x6C0603E6B3B7C1CA),
    CR_ULL(0xF6FAE5C07D3274CD), CR_ULL(0xB40A042BD4D8425E),
    CR_ULL(0x731B26172EE619EB), CR_ULL(0x31EBC7FC870C2F78),
    CR_ULL(0xBFC9838573709812), CR_ULL(0xFD39626EDA9AAE81),
    CR_ULL(0x3A28405220A4F534), CR_ULL(0x78D8A1B9894EC3A7),
    CR_ULL(0x649C294A61B7AD73), CR_ULL(0x266CC8A1C85D9BE0),
    CR_ULL(0xE17DEA9D3263C055), CR_ULL(0xA38D0B769B89F6C6),
    CR_ULL(0x2DAF4F0F6FF541AC), CR_ULL(0x6F5FAEE4C61F773F),
    CR_ULL(0xA84E8CD83C212C8A), CR_ULL(0xEABE6D3395CB1A19),
    CR_ULL(0x90C79D3FEDD3F122), CR_ULL(0xD2377CD44439C7B1),
    CR_ULL(0x15265EE8BE079C04), CR_ULL(0x57D6BF0317EDAA97),
    CR_ULL(0xD9F4FB7AE3911DFD), CR_ULL(0x9B041A914A7B2B6E),
    CR_ULL(0x5C1538ADB04570DB), CR_ULL(0x1EE5D94619AF4648),
    CR_ULL(0x02A151B5F156289C), CR_ULL(0x4051B05E58BC1E0F),
    CR_ULL(0x87409262A28245BA), CR_ULL(0xC5B073890B687329),
    CR_ULL(0x4B9237F0FF14C443), CR_ULL(0x0962D61B56FEF2D0),
    CR_ULL(0xCE73F427ACC0A965), CR_ULL(0x8C8315CC052A9FF6),
    CR_ULL(0x3A80143F5CF17F13), CR_ULL(0x7870F5D4F51B4980),
    CR_ULL(0xBF61D7E80F251235), CR_ULL(0xFD913603A6CF24A6),
    CR_ULL(0x73B3727A52B393CC), CR_ULL(0x31439391FB59A55F),
    CR_ULL(0xF652B1AD0167FEEA), CR_ULL(0xB4A25046A88DC879),
    CR_ULL(0xA8E6D8B54074A6AD), CR_ULL(0xEA16395EE99E903E),
    CR_ULL(0x2D071B6213A0CB8B), CR_ULL(0x6FF7FA89BA4AFD18),
    CR_ULL(0xE1D5BEF04E364A72), CR_ULL(0xA3255F1BE7DC7CE1),
    CR_ULL(0x64347D271DE22754), CR_ULL(0x26C49CCCB40811C7),
    CR_ULL(0x5CBD6CC0CC10FAFC), CR_ULL(0x1E4D8D2B65FACC6F),
    CR_ULL(0xD95CAF179FC497DA), CR_ULL(0x9BAC4EFC362EA149),
    CR_ULL(0x158E0A85C2521623), CR_ULL(0x577EEB6E6BB820B0),
    CR_ULL(0x906FC95291867B05), CR_ULL(0xD29F28B9386C4D96),
    CR_ULL(0xCEDBA04AD0952342), CR_ULL(0x8C2B41A1797F15D1),
    CR_ULL(0x4B3A639D83414E64), CR_ULL(0x09CA82762AAB78F7),
    CR_ULL(0x87E8C60FDED7CF9D), CR_ULL(0xC51827E4773DF90E),
    CR_ULL(0x020905D88D03A2BB), CR_ULL(0x40F9E43324E99428),
    CR_ULL(0x2CFFE7D5975E55E2), CR_ULL(0x6E0F063E3EB46371),
    CR_ULL(0xA91E2402C48A38C4), CR_ULL(0xEBEEC5E96D600E57),
    CR_ULL(0x65CC8190991CB93D), CR_ULL(0x273C607B30F68FAE),
    CR_ULL(0xE02D4247CAC8D41B), CR_ULL(0xA2DDA3AC6322E288),
    CR_ULL(0xBE992B5F8BDB8C5C), CR_ULL(0xFC69CAB42231BACF),
    CR_ULL(0x3B78E888D80FE17A), CR_ULL(0x7988096371E5D7E9),
    CR_ULL(0xF7AA4D1A85996083), CR_ULL(0xB55AACF12C735610),
    CR_ULL(0x724B8ECDD64D0DA5), CR_ULL(0x30BB6F267FA73B36),
    CR_ULL(0x4AC29F2A07BFD00D), CR_ULL(0x08327EC1AE55E69E),
    CR_ULL(0xCF235CFD546BBD2B), CR_ULL(0x8DD3BD16FD818BB8),
    CR_ULL(0x03F1F96F09FD3CD2), CR_ULL(0x41011884A0170A41),
    CR_ULL(0x86103AB85A2951F4), CR_ULL(0xC4E0DB53F3C36767),
    CR_ULL(0xD8A453A01B3A09B3), CR_ULL(0x9A54B24BB2D03F20),
    CR_ULL(0x5D45907748EE6495), CR_ULL(0x1FB5719CE1045206),
    CR_ULL(0x919735E51578E56C), CR_ULL(0xD367D40EBC92D3FF),
    CR_ULL(0x1476F63246AC884A), CR_ULL(0x568617D9EF46BED9),
    CR_ULL(0xE085162AB69D5E3C), CR_ULL(0xA275F7C11F7768AF),
    CR_ULL(0x6564D5FDE549331A), CR_ULL(0x279434164CA30589),
    CR_ULL(0xA9B6706FB8DFB2E3), CR_ULL(0xEB46918411358470),
    CR_ULL(0x2C57B3B8EB0BDFC5), CR_ULL(0x6EA7525342E1E956),
    CR_ULL(0x72E3DAA0AA188782), CR_ULL(0x30133B4B03F2B111),
    CR_ULL(0xF7021977F9CCEAA4), CR_ULL(0xB5F2F89C5026DC37),
    CR_ULL(0x3BD0BCE5A45A6B5D), CR_ULL(0x79205D0E0DB05DCE),
    CR_ULL(0xBE317F32F78E067B), CR_ULL(0xFCC19ED95E6430E8),
    CR_ULL(0x86B86ED5267CDBD3), CR_ULL(0xC4488F3E8F96ED40),
    CR_ULL(0x0359AD0275A8B6F5), CR_ULL(0x41A94CE9DC428066),
    CR_ULL(0xCF8B0890283E370C), CR_ULL(0x8D7BE97B81D4019F),
    CR_ULL(0x4A6ACB477BEA5A2A), CR_ULL(0x089A2AACD2006CB9),
    CR_ULL(0x14DEA25F3AF9026D), CR_ULL(0x562E43B4931334FE),
    CR_ULL(0x913F6188692D6F4B), CR_ULL(0xD3CF8063C0C759D8),
    CR_ULL(0x5DEDC41A34BBEEB2), CR_ULL(0x1F1D25F19D51D821),
    CR_ULL(0xD80C07CD676F8394), CR_ULL(0x9AFCE626CE85B507),
};

#define CRC64_INT_VALUE     CR_ULL(0xFFFFFFFFFFFFFFFF)
#define CRC64_XOR_VALUE     CR_ULL(0xFFFFFFFFFFFFFFFF)

/*
=======================================
    CRC-64/ECMA-182 初始化
=======================================
*/
CR_API int64u
hash_crc64e_init (void_t)
{
    return (CRC64_INT_VALUE);
}

/*
=======================================
    CRC-64/ECMA-182 计算数据块
=======================================
*/
CR_API int64u
hash_crc64e_update (
  __CR_IN__ int64u          hash,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    for (; size != 0; size--)
    {
        hash = s_crc64[(uchar)(hash >> 56) ^ *(uchar*)data] ^ (hash << 8);
        data = (const uchar*)data + 1;
    }
    return ((int64u)(hash));
}

/*
=======================================
    CRC-64/ECMA-182 获取结果
=======================================
*/
CR_API int64u
hash_crc64e_finish (
  __CR_IN__ int64u  hash
    )
{
    return ((int64u)(hash ^ CRC64_XOR_VALUE));
}

/*
=======================================
    CRC-64/ECMA-182 一次性完成
=======================================
*/
CR_API int64u
hash_crc64e_total (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u  hash = hash_crc64e_init();

    hash = hash_crc64e_update(hash, data, size);

    return (hash_crc64e_finish(hash));
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/

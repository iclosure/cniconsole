#ifndef ME_VERSION_H
#define ME_VERSION_H

// 内部版本
#define ME_INTERNAL_MAJOR_VERSION   1
#define ME_INTERNAL_MINOR_VERSION   1
#define ME_INTERNAL_PATCH_VERSION   0
#define ME_INTERNAL_BUILD_VERSION   0

// 产品版本
#define ME_MAJOR_VERSION    1
#define ME_MINOR_VERSION    1
#define ME_PATCH_VERSION    0
#define ME_BUILD_VERSION    0


#define ME_MK_VER_STR(MAJOR, MINOR, PATCH, BUILD) #MAJOR"."#MINOR"."#PATCH"."#BUILD

#define ME_VER_FILE            ME_MAJOR_VERSION, ME_MINOR_VERSION, ME_PATCH_VERSION, ME_BUILD_VERSION
#define ME_VER_PRODUCT         ME_MAJOR_VERSION, ME_MINOR_VERSION, ME_PATCH_VERSION, ME_BUILD_VERSION
#define ME_VER_STRING_FILE     ME_MK_VER_STR(ME_MAJOR_VERSION, ME_MINOR_VERSION, ME_PATCH_VERSION, ME_MAJOR_VERSION)
#define ME_VER_STRING_PRODUCT  ME_MK_VER_STR(ME_MAJOR_VERSION, ME_MINOR_VERSION, ME_PATCH_VERSION, ME_MAJOR_VERSION)


#endif // VERSION_H

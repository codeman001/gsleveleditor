/******************************************************************************
	Glitch Engine
	Mailing List: World-Glitch@gameloft.com
	Documentation: https://wiki.gameloft.org/twiki/bin/view/Main/GlitchProgramming
******************************************************************************/
#ifndef __GLITCH_VIDEO_DETAIL_SPVRHEADER_H__
#define __GLITCH_VIDEO_DETAIL_SPVRHEADER_H__

#include <irrTypes.h>

namespace irr
{
namespace video
{
namespace detail
{

enum E_PVR_FLAGS
{
	EPVRF_FORMAT_MASK = 0xFF,
	EPVRF_HAS_MIPMAPS = 0x100,
	EPVRF_TWIDDLED = 0x200,
	EPVRF_NORMAL_MAP = 0x400,
	EPVRF_BORDER = 0x800,
	EPVRF_CUBE_MAP = 0x1000,
	EPVRF_FALSE_COLOR_MIPMAPS = 0x2000,
	EPVRF_VOLUME_TEXTURE = 0x4000,
	EPVRF_ALPHA_IN_TEXTURE = 0x8000,
	EPVRF_FLIPPED = 0x10000
};

struct SPVRHeader
{
	u32 HeaderLength;
	u32 Height;
	u32 Width;
	u32 MipmapCount;
	u32 Flags;
	u32 DataLength;
	u32 BitsPerPixel;
	u32 BitmaskRed;
	u32 BitmaskGreen;
	u32 BitmaskBlue;
	u32 BitmaskAlpha;
	char PVRTag[4];
	u32 SurfaceCount;
};

} // end namespace detail
} // end namespace video
} // end namespace glitch

#endif //__GLITCH_VIDEO_DETAIL_SPVRHEADER_H__

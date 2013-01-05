#include "IrrCompileConfig.h" 
#include "CImageLoaderPVR.h" 

#ifdef _IRR_COMPILE_WITH_PVR_LOADER_

#include "IVideoDriver.h" 
#include "CReadFile.h"
#include "CColorConverter.h"
#include "CImage.h"
//#include "CCompressedImage.h" 
#include "SPVRHeader.h"
#include "os.h"

namespace irr
{
namespace video
{

namespace
{

bool
readPVRHeader(io::IReadFile* file, detail::SPVRHeader& header)
{
	file->seek(0);
	s32 readCount = file->read(&header, sizeof(detail::SPVRHeader));

#ifdef __BIG_ENDIAN__
	header.HeaderLength = os::byteswap(header.HeaderLength);
	header.Height = os::byteswap(header.Height);
	header.Width = os::byteswap(header.Width);
	header.MipmapCount = os::byteswap(header.MipmapCount);
	header.Flags = os::byteswap(header.Flags);
	header.DataLength = os::byteswap(header.DataLength);
	header.BitsPerPixel = os::byteswap(header.BitsPerPixel);
	header.BitmaskRed = os::byteswap(header.BitmaskRed);
	header.BitmaskGreen = os::byteswap(header.BitmaskGreen);
	header.BitmaskBlue = os::byteswap(header.BitmaskBlue);
	header.BitmaskAlpha = os::byteswap(header.BitmaskAlpha);
	header.SurfaceCount = os::byteswap(header.SurfaceCount);
#endif

	if(readCount != sizeof(detail::SPVRHeader)
	   || strncmp(header.PVRTag, "PVR!", 4) != 0
	   || header.HeaderLength != sizeof(detail::SPVRHeader)
	   || ((header.Flags & detail::EPVRF_HAS_MIPMAPS) != 0 && header.MipmapCount == 0)
	   || (header.Flags & detail::EPVRF_CUBE_MAP) != 0 && header.SurfaceCount != 6)
	{
		return false;
	}

	header.MipmapCount++;

	return true;	
}

} // end anonymous namespace

CImageLoaderPVR::CImageLoaderPVR()
{
}

CImageLoaderPVR::~CImageLoaderPVR()
{
}

bool
CImageLoaderPVR::isALoadableFileExtension(const c8* fileName) const
{
	return strstr(fileName, ".pvr") != 0; 
}

bool
CImageLoaderPVR::isALoadableFileFormat(irr::io::IReadFile* file) const
{
	if(file != NULL) 
	{
		detail::SPVRHeader header;
		long bytesRead = file->read(&header, sizeof(detail::SPVRHeader));
#ifdef __BIG_ENDIAN__
		header.HeaderLength = os::byteswap(header.HeaderLength);
#endif
		if(bytesRead == sizeof(detail::SPVRHeader)
		   && header.HeaderLength == sizeof(detail::SPVRHeader)
		   && strncmp(header.PVRTag, "PVR!", 4) == 0)
		{
			return true;
		}
	}
	return false; 
}

IImage*
CImageLoaderPVR::loadImage(irr::io::IReadFile* file) const
{
	detail::SPVRHeader header;
	if(!readPVRHeader(file, header))
	{
		return NULL;
	}
	u32 format = header.Flags & 0xff;
	ECOLOR_FORMAT colorFormat = ECF_UNKNOWN; 
        
	switch(format)
	{
		case 0x12:	// OGL_RGBA_8888
		{
			colorFormat = ECF_R8G8B8A8;
		}
		break;

		case 0x0C:
		case 0x18 : // OGL_PVRTC2
		{
			colorFormat = (header.Flags & 0x8000
						   ? ECF_COMPRESSED_RGBA_PVRTC_2BPP
						   : ECF_COMPRESSED_RGB_PVRTC_2BPP);
		}
		break;

		case 0x0D:
		case 0x19 : // OGL_PVRTC4
		{
			colorFormat = (header.Flags & 0x8000
						   ? ECF_COMPRESSED_RGBA_PVRTC_4BPP
						   : ECF_COMPRESSED_RGB_PVRTC_4BPP);
		}
		break;

		default :
		{
			os::Printer::log("PVR loader", "unsupported format", ELL_ERROR);
			return NULL;
		}
	}

	bool isCubeMap = (header.Flags & detail::EPVRF_CUBE_MAP)!=0;
	if (isCubeMap) 
	{
		if (header.MipmapCount > 1) {
			isCubeMap = false;
		}
	}

	u8* data = new u8[header.DataLength];

	long bytesRead = file->read(data, header.DataLength);
	if(bytesRead != long(header.DataLength))
	{
		os::Printer::log("corrupt PVR file", file->getFileName(), ELL_ERROR);
		delete[] data;
		return NULL;
	}

	IImage* result = NULL; 
	if(colorFormat == ECF_R8G8B8A8)
	{
		result = new CImage(ECF_R8G8B8A8, core::dimension2d<s32>(header.Width, header.Height));
		
		CColorConverter::convert32BitTo32Bit((s32*)data, (s32*)result->lock(), header.Width, header.Height, 0, false);
		delete[] data;
		result->unlock();
	}
	else
	{
		// -- CUBE MAP
		if (isCubeMap) 
		{
			result = new CCompressedImage( colorFormat, core::dimension2d<u32>( header.Width, header.Height ), data, header.DataLength, header.MipmapCount, true, true, true ); 

			for (int i=0;i<5;i++) 
			{
				long bytesRead = file->read(data, header.DataLength);
				CCompressedImage* compressedImage = static_cast<CCompressedImage*>(result);
				compressedImage->setCubeMapExtraFaceData(i+1, data, header.DataLength);
			}			
		}
		// -- NORMAL
		else
		{
			result = new CCompressedImage( colorFormat, core::dimension2d<u32>( header.Width, header.Height ), data, header.DataLength, header.MipmapCount ); 
		}
	}
	return result;
}

IImageLoader*
createImageLoaderPVR()
{
	return new CImageLoaderPVR;
}

} // end namespace video
} // end namespace irr

#endif //GLITCH_COMPILE_WITH_PVR_LOADER

#ifndef __C_DX11_TEXTURE_H_INCLUDED__
#define __C_DX11_TEXTURE_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

#include "ITexture.h"
#include "IImage.h"

namespace irr
{
namespace video
{

class CD3D11Driver;

class CD3D11Texture : public ITexture
{
public:
	//! constructor
	CD3D11Texture(IImage* image, CD3D11Driver* driver,
			u32 flags, const io::path& name, u32 arraySlices = 1, void* mipmapData=0);

	//! rendertarget constructor
	CD3D11Texture(CD3D11Driver* driver, const core::dimension2d<u32>& size, const io::path& name,
		const ECOLOR_FORMAT format = ECF_UNKNOWN, u32 arraySlices = 1, 
		u32 sampleCount = 1, u32 sampleQuality = 0 );

	//! destructor
	virtual ~CD3D11Texture();

	//! lock function
	virtual void* lock(bool readOnly = false, u32 mipmapLevel=0, u32 arraySlice = 0);

	//! unlock function
	virtual void unlock();

	//! Returns original size of the texture.
	virtual const core::dimension2d<u32>& getOriginalSize() const;

	//! Returns (=size) of the texture.
	virtual const core::dimension2d<u32>& getSize() const;

	//! returns driver type of texture (=the driver, who created the texture)
	virtual E_DRIVER_TYPE getDriverType() const;

	//! returns color format of texture
	virtual ECOLOR_FORMAT getColorFormat() const;

	virtual u32 getPitch() const;

	//! returns if texture has mipmap levels
	bool hasMipMaps() const;

	virtual u32 getNumberOfArraySlices() const;

	//! Regenerates the mip map levels of the texture. Useful after locking and
	//! modifying the texture
	virtual void regenerateMipMapLevels(void* mipmapData=0);

	//! returns if it is a render target
	virtual bool isRenderTarget() const;
private:
	CD3D11Driver* Driver;

	core::dimension2d<u32> TextureSize;
	core::dimension2d<u32> ImageSize;

	s32 Pitch;
	ECOLOR_FORMAT ColorFormat;
};


}
}

#endif

#endif
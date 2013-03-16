#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

#include "CD3D11Driver.h"
#include "CD3D11Texture.h"
#include "os.h"

#include "CImage.h"
#include "CColorConverter.h"

namespace irr
{
namespace video
{

//! constructor
CD3D11Texture::CD3D11Texture(IImage* image, CD3D11Driver* driver,
			u32 flags, const io::path& name, u32 arraySlices, void* mipmapData)
: ITexture(name),
Driver(driver)
{
}

//! rendertarget constructor
CD3D11Texture::CD3D11Texture(CD3D11Driver* driver, const core::dimension2d<u32>& size, const io::path& name,
		const ECOLOR_FORMAT format, u32 arraySlices, 
		u32 sampleCount, u32 sampleQuality)
: ITexture(name),
Driver(driver)
{
}

//! destructor
CD3D11Texture::~CD3D11Texture()
{
}

//! lock function
void* CD3D11Texture::lock(bool readOnly, u32 mipmapLevel, u32 arraySlice)
{
	return NULL;
}

//! unlock function
void CD3D11Texture::unlock()
{
}

//! Returns original size of the texture.
const core::dimension2d<u32>& CD3D11Texture::getOriginalSize() const
{
	return ImageSize;
}

//! Returns (=size) of the texture.
const core::dimension2d<u32>& CD3D11Texture::getSize() const
{
	return TextureSize;
}

//! returns driver type of texture (=the driver, who created the texture)	
E_DRIVER_TYPE CD3D11Texture::getDriverType() const
{
	return EDT_DIRECT3D11;
}

//! returns color format of texture
ECOLOR_FORMAT CD3D11Texture::getColorFormat() const
{
	return ColorFormat;
}

u32 CD3D11Texture::getPitch() const
{
	return Pitch;
}

//! returns if texture has mipmap levels
bool CD3D11Texture::hasMipMaps() const
{
	return false;
}

u32 CD3D11Texture::getNumberOfArraySlices() const
{
	return 0;
}

//! Regenerates the mip map levels of the texture. Useful after locking and
//! modifying the texture
void CD3D11Texture::regenerateMipMapLevels(void* mipmapData)
{
}

//! returns if it is a render target
bool CD3D11Texture::isRenderTarget() const
{
	return false;
}


}
}

#endif
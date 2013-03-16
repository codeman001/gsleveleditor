#include "CD3D11Driver.h"

#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_

namespace irr
{
namespace video
{

//! constructor
CD3D11Driver::CD3D11Driver(const SIrrlichtCreationParameters& params, io::IFileSystem* io)
	:CNullDriver(io, params.WindowSize)
{
}

//! destructor
CD3D11Driver::~CD3D11Driver()
{
}


//! Can be called by an IMaterialRenderer to make its work easier.
void CD3D11Driver::setBasicRenderStates(const SMaterial& material,
	const SMaterial& lastMaterial,
	bool resetAllRenderstates)
{
}

//! Sets a constant for the vertex shader based on a name.
bool CD3D11Driver::setVertexShaderConstant(const c8* name, const f32* floats, int count)
{
	return false;
}

//! Sets a vertex shader constant.
void CD3D11Driver::setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount)
{
}

//! Sets a constant for the pixel shader based on a name.
bool CD3D11Driver::setPixelShaderConstant(const c8* name, const f32* floats, int count)
{
	return false;
}

//! Sets a pixel shader constant.
void CD3D11Driver::setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount)
{
}

//! Get pointer to the IVideoDriver interface
/** \return Pointer to the IVideoDriver interface */
IVideoDriver* CD3D11Driver::getVideoDriver()
{
	return this;
}

} // namespace
} // namespace

#endif

namespace irr
{
namespace video
{

#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
//! creates a video driver
IVideoDriver* createDirectX11Driver(const SIrrlichtCreationParameters& params, 
									io::IFileSystem* io,
									HWND window)
{
	CD3D11Driver* dx11 = new CD3D11Driver(params, io);
	return dx11;
}
#endif // _IRR_COMPILE_WITH_DIRECT3D_9_

} // end namespace video
} // end namespace irr
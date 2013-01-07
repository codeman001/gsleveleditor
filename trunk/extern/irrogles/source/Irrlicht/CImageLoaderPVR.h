#ifndef __C_IMAGE_LOADER_PVR_H_INCLUDED__
#define __C_IMAGE_LOADER_PVR_H_INCLUDED__

#include "IrrCompileConfig.h" 

#ifdef _IRR_COMPILE_WITH_PVR_LOADER_

#include "IImageLoader.h" 

namespace irr 
{ 
namespace video 
{ 

class CImageLoaderPVR
	: public IImageLoader
{
public:

	//!
	CImageLoaderPVR();

	//!
	virtual ~CImageLoaderPVR();

	//! returns true if the file maybe is able to be loaded by this class 
	//! based on the file extension (e.g. ".tga") 
	virtual bool isALoadableFileExtension(const io::path& filename) const; 

	//! returns true if the file maybe is able to be loaded by this class 
	virtual bool isALoadableFileFormat(irr::io::IReadFile* file) const; 

	//! creates a surface from the file 
	virtual IImage* loadImage(irr::io::IReadFile* file) const;
};

IImageLoader* createImageLoaderPVR();

} // end namespace video 
} // end namespace demon 

#endif

#endif //__C_IMAGE_LOADER_PVR_H_INCLUDED__

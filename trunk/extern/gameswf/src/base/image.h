#pragma once
// image.h	-- Thatcher Ulrich <tu@tulrich.com> 2002

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Handy image utilities for RGB surfaces.

#ifndef IMAGE_H
#define IMAGE_H


#include "base/tu_config.h"
#include "base/tu_types.h"
#include "base/container.h"

namespace gameswf
{
	class tu_file;
	namespace jpeg { struct input; };

	struct image_base
	{
		enum id_image
		{
			INVALID,
			RGB,
			RGBA,
			ALPHA
		};

		id_image m_type;
		Uint8*	m_data;
		int	m_width;
		int	m_height;
		int	m_pitch;	// byte offset from one row to the next

		image_base(Uint8* data, int width, int height, int pitch, id_image type);
		virtual ~image_base();
	};

	// 24-bit RGB image.  Packed data, red byte first (RGBRGB...)
	struct image_rgb : public image_base
	{
		image_rgb(int width, int height);
		~image_rgb();
	};

	// 32-bit RGBA image.  Packed data, red byte first (RGBARGBA...)
	struct image_rgba : public image_base
	{
		image_rgba(int width, int height);
		~image_rgba();

		void	set_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	};

	// 8-bit alpha image.
	struct image_alpha : public image_base
	{
		image_alpha(int width, int height);
		~image_alpha();

		void	set_pixel(int x, int y, Uint8 a);

		// Bitwise content comparison.
		bool	operator==(const image_alpha& a) const;

		// Return a hash code based on the image contents.
		unsigned int	compute_hash() const;
	};


	// Make a system-memory 24-bit bitmap surface.  24-bit packed
	// data, red byte first.
	image_rgb*	create_rgb(int width, int height);


	// Make a system-memory 32-bit bitmap surface.  Packed data,
	// red byte first.
	image_rgba*	create_rgba(int width, int height);


	// Make a system-memory 8-bit bitmap surface.
	image_alpha*	create_alpha(int width, int height);

	
	Uint8*	scanline(image_base* surf, int y);
	const Uint8*	scanline(const image_base* surf, int y);


	void	resample(image_rgb* out, int out_x0, int out_y0, int out_x1, int out_y1,
			 image_rgb* in, float in_x0, float in_y0, float in_x1, float in_y1);

	void	resample(image_rgba* out, int out_x0, int out_y0, int out_x1, int out_y1,
			 image_rgba* in, float in_x0, float in_y0, float in_x1, float in_y1);

	void	zoom(image_base* src, image_base* dst);

	void	write_jpeg(tu_file* out, image_rgb* image, int quality);
	void	write_tga(tu_file* out, image_rgba* image);

	exported_module image_rgb*	read_jpeg(const char* filename);
	exported_module image_rgb*	read_jpeg(tu_file* in);

	// For reading SWF JPEG2-style image data (slight variation on
	// ordinary JPEG).
	image_rgb*	read_swf_jpeg2(tu_file* in);

	// For reading SWF JPEG2-style image data, using pre-loaded
	// headers stored in the given jpeg::input object.
	image_rgb*	read_swf_jpeg2_with_tables(jpeg::input* loader);

	// For reading SWF JPEG3-style image data, like ordinary JPEG, 
	// but stores the data in rgba format.
	image_rgba*	read_swf_jpeg3(tu_file* in);

	// Fast, in-place, DESTRUCTIVE resample.  For making mip-maps.
	// Munges the input image to produce the output image.
	void	make_next_miplevel(image_rgb* image);
	void	make_next_miplevel(image_rgba* image);
};


#endif // IMAGE_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:

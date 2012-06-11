#include "stdafx.h"
// image.h	-- Thatcher Ulrich <tu@tulrich.com> 2002

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Handy image utilities for RGB surfaces.

#include "base/image.h"

#include "base/container.h"
#include "base/utility.h"
#include "base/jpeg.h"
#include "base/tu_file.h"
#include <stdlib.h>
#include <string.h>


namespace gameswf
{
	//
	// image_base
	//
	image_base::image_base(Uint8* data, int width, int height, int pitch, id_image type)
		:
		m_type(type),
		m_data(data),
		m_width(width),
		m_height(height),
		m_pitch(pitch)
	{
	}

	image_base::~image_base()
	{
		if (m_data) 
		{
			swfdelete_primitives (m_data, m_pitch * m_height);
			m_data = 0;
		}
	}

	Uint8*	scanline(image_base* surf, int y)
	{
		assert(surf);
		assert(y >= 0 && y < surf->m_height);
		return ((Uint8*) surf->m_data) + surf->m_pitch * y;
	}


	const Uint8*	scanline(const image_base* surf, int y)
	{
		assert(surf);
		assert(y >= 0 && y < surf->m_height);
		return ((const Uint8*) surf->m_data) + surf->m_pitch * y;
	}


	//
	// rgb
	//

	image_rgb::image_rgb(int width, int height)
		:
		image_base(
			0,
			width,
			height,
			(width * 3 + 3) & ~3, RGB)	// round pitch up to nearest 4-byte boundary
	{
		assert(width > 0);
		assert(height > 0);
		assert(m_pitch >= m_width * 3);
		assert((m_pitch & 3) == 0);

//		m_data = (Uint8*) dlmalloc(m_pitch * m_height);
		m_data = swfnew Uint8[m_pitch * m_height];
	}

	image_rgb::~image_rgb()
	{
//		if (m_data) {
//			dlfree(m_data);
//			swfdelete_primitives (m_data, m_pitch * m_height);
//			m_data = 0;
//		}
	}


	image_rgb*	create_rgb(int width, int height)
	// Create an system-memory rgb surface.  The data order is
	// packed 24-bit, RGBRGB..., regardless of the endian-ness of
	// the CPU.
	{
		image_rgb*	s = swfnew image_rgb(width, height);

		return s;
	}


	//
	// rgba
	//


	image_rgba::image_rgba(int width, int height)
		:
		image_base(0, width, height, width * 4, RGBA)
	{
		assert(width > 0);
		assert(height > 0);
		assert(m_pitch >= m_width * 4);
		assert((m_pitch & 3) == 0);

//		m_data = (Uint8*) dlmalloc(m_pitch * m_height);
		m_data = swfnew Uint8[m_pitch * m_height];
	}

	image_rgba::~image_rgba()
	{
//		if (m_data) {
//			dlfree(m_data);
//			delete [] m_data;
//			m_data = 0;
//		}
	}


	image_rgba*	create_rgba(int width, int height)
	// Create an system-memory rgb surface.  The data order is
	// packed 32-bit, RGBARGBA..., regardless of the endian-ness
	// of the CPU.
	{
		image_rgba*	s = swfnew image_rgba(width, height);

		return s;
	}


	void	image_rgba::set_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	// Set the pixel at the given position.
	{
		assert(x >= 0 && x < m_width);
		assert(y >= 0 && y < m_height);

		Uint8*	data = scanline(this, y) + 4 * x;

		data[0] = r;
		data[1] = g;
		data[2] = b;
		data[3] = a;
	}


	//
	// alpha
	//


	image_alpha*	create_alpha(int width, int height)
	// Create an system-memory 8-bit alpha surface.
	{
		image_alpha*	s = swfnew image_alpha(width, height);

		return s;
	}


	image_alpha::image_alpha(int width, int height)
		:
		image_base(0, width, height, width, ALPHA)
	{
		assert(width > 0);
		assert(height > 0);

//		m_data = (Uint8*) dlmalloc(m_pitch * m_height);
		m_data = swfnew Uint8[m_pitch * m_height];
	}


	image_alpha::~image_alpha()
	{
//		if (m_data) {
//			dlfree(m_data);
//			delete [] m_data;
//			m_data = 0;
//		}
	}


	void	image_alpha::set_pixel(int x, int y, Uint8 a)
	// Set the pixel at the given position.
	{
		assert(x >= 0 && x < m_width);
		assert(y >= 0 && y < m_height);

		Uint8*	data = scanline(this, y) + x;

		data[0] = a;
	}


	bool	image_alpha::operator==(const image_alpha& a) const
	// Bitwise content comparison.
	{
		if (m_width != a.m_width
		    || m_height != a.m_height)
		{
			return false;
		}

		for (int j = 0, n = m_height; j < n; j++)
		{
			if (memcmp(scanline(this, j), scanline(&a, j), m_width))
			{
				// Mismatch.
				return false;
			}
		}

		// Images are identical.
		return true;
	}


	unsigned int	image_alpha::compute_hash() const
	// Compute a hash code based on image contents.  Can be useful
	// for comparing images.
	{
		unsigned int	h = (unsigned int) bernstein_hash(&m_width, sizeof(m_width));
		h = (unsigned int) bernstein_hash(&m_height, sizeof(m_height), h);

		for (int i = 0, n = m_height; i < n; i++)
		{
			h = (unsigned int) bernstein_hash(scanline(this, i), m_width, h);
		}

		return h;
	}


	//
	// utility
	//


	//void	write_jpeg(tu_file* out, image_rgb* image, int quality)
	//// Write the given image to the given out stream, in jpeg format.
	//{
	//	jpeg::output*	j_out = jpeg::output::create(out, image->m_width, image->m_height, quality);

	//	for (int y = 0; y < image->m_height; y++) {
	//		j_out->write_scanline(scanline(image, y));
	//	}

	//	delete j_out;
	//}


	image_rgb*	read_jpeg(const char* filename)
	// Create and read a swfnew image from the given filename, if possible.
	{
		tu_file	in(filename, "rb");	// file automatically closes when 'in' goes out of scope.
		if (! in.get_error())
		{
			image_rgb*	im = read_jpeg(&in);
			return im;
		}
		else
		{
			return NULL;
		}
	}


	image_rgb*	read_jpeg(tu_file* in)
	// Create and read a swfnew image from the stream.
	{
		jpeg::input*	j_in = jpeg::input::create(in);
		if (j_in == NULL) return NULL;
		
		image_rgb*	im = create_rgb(j_in->get_width(), j_in->get_height());

		for (int y = 0; y < j_in->get_height(); y++)
		{
			j_in->read_scanline(scanline(im, y));
		}

		swfdelete (j_in);

		return im;
	}


	image_rgb*	read_swf_jpeg2(tu_file* in)
	// Create and read a swfnew image from the stream.  Image is in
	// SWF JPEG2-style format (the encoding tables come first in a
	// separate "stream" -- otherwise it's just normal JPEG).  The
	// IJG documentation describes this as "abbreviated" format.
	{
		jpeg::input*	j_in = jpeg::input::create_swf_jpeg2_header_only(in);
		if (j_in == NULL) return NULL;
		
		image_rgb*	im = read_swf_jpeg2_with_tables(j_in);

		swfdelete (j_in);

		return im;
	}


	image_rgb*	read_swf_jpeg2_with_tables(jpeg::input* j_in)
	// Create and read a swfnew image, using a input object that
	// already has tables loaded.
	{
		assert(j_in);

		j_in->start_image();

		image_rgb*	im = gameswf::create_rgb(j_in->get_width(), j_in->get_height());

		for (int y = 0; y < j_in->get_height(); y++) {
			j_in->read_scanline(scanline(im, y));
		}

		j_in->finish_image();

		return im;
	}


	image_rgba*	read_swf_jpeg3(tu_file* in)
	// For reading SWF JPEG3-style image data, like ordinary JPEG, 
	// but stores the data in rgba format.
	{
		jpeg::input*	j_in = jpeg::input::create_swf_jpeg2_header_only(in);
		if (j_in == NULL) return NULL;
		
		j_in->start_image();

		image_rgba*	im = create_rgba(j_in->get_width(), j_in->get_height());

		Uint8*	line = swfnew Uint8[3*j_in->get_width()];

		for (int y = 0; y < j_in->get_height(); y++) 
		{
			j_in->read_scanline(line);

			Uint8*	data = scanline(im, y);
			for (int x = 0; x < j_in->get_width(); x++) 
			{
				data[4*x+0] = line[3*x+0];
				data[4*x+1] = line[3*x+1];
				data[4*x+2] = line[3*x+2];
				data[4*x+3] = 255;
			}
		}

		swfdelete_primitives (line, 3*j_in->get_width());

		j_in->finish_image();
		swfdelete (j_in);

		return im;
	}


	void	write_tga(tu_file* out, image_rgba* im)
	// Write a 32-bit Targa format bitmap.  Dead simple, no compression.
	{
		out->write_byte(0);
		out->write_byte(0);
		out->write_byte(2);	/* uncompressed RGB */
		out->write_le16(0);
		out->write_le16(0);
		out->write_byte(0);
		out->write_le16(0);	/* X origin */
		out->write_le16(0);	/* y origin */
		out->write_le16(im->m_width);
		out->write_le16(im->m_height);
		out->write_byte(32);	/* 32 bit bitmap */
		out->write_byte(0);

		for (int y = 0; y < im->m_height; y++)
		{
			uint8*	p = scanline(im, y);
			for (int x = 0; x < im->m_width; x++)
			{
				out->write_byte(p[x * 4]);
				out->write_byte(p[x * 4 + 1]);
				out->write_byte(p[x * 4 + 2]);
				out->write_byte(p[x * 4 + 3]);
			}
		}
	}

	void	make_next_miplevel(image_rgb* image)
	// Fast, in-place resample.  For making mip-maps.  Munges the
	// input image to produce the output image.
	{
		assert(image->m_data);

		int	new_w = image->m_width >> 1;
		int	new_h = image->m_height >> 1;
		if (new_w < 1) new_w = 1;
		if (new_h < 1) new_h = 1;

		int	new_pitch = new_w * 3;
		// Round pitch up to the nearest 4-byte boundary.
		new_pitch = (new_pitch + 3) & ~3;

		if (new_w * 2 != image->m_width  || new_h * 2 != image->m_height)
		{
			// Image can't be shrunk along (at least) one
			// of its dimensions, so don't bother
			// resampling.  Technically we should, but
			// it's pretty useless at this point.  Just
			// change the image dimensions and leave the
			// existing pixels.
		}
		else
		{
			// Resample.  Simple average 2x2 --> 1, in-place.
			int	pitch = image->m_pitch;
			for (int j = 0; j < new_h; j++) {
				Uint8*	out = ((Uint8*) image->m_data) + j * new_pitch;
				Uint8*	in = ((Uint8*) image->m_data) + (j << 1) * pitch;
				for (int i = 0; i < new_w; i++) {
					int	r, g, b;
					r = (*(in + 0) + *(in + 3) + *(in + 0 + pitch) + *(in + 3 + pitch));
					g = (*(in + 1) + *(in + 4) + *(in + 1 + pitch) + *(in + 4 + pitch));
					b = (*(in + 2) + *(in + 5) + *(in + 2 + pitch) + *(in + 5 + pitch));
					*(out + 0) = r >> 2;
					*(out + 1) = g >> 2;
					*(out + 2) = b >> 2;
					out += 3;
					in += 6;
				}
			}
		}

		// Munge image's members to reflect the shrunken image.
		image->m_width = new_w;
		image->m_height = new_h;
		image->m_pitch = new_pitch;
	}


	void	make_next_miplevel(image_rgba* image)
	// Fast, in-place resample.  For making mip-maps.  Munges the
	// input image to produce the output image.
	{
		assert(image->m_data);

		int	new_w = image->m_width >> 1;
		int	new_h = image->m_height >> 1;
		if (new_w < 1) new_w = 1;
		if (new_h < 1) new_h = 1;

		int	new_pitch = new_w * 4;

		if (new_w * 2 != image->m_width  || new_h * 2 != image->m_height)
		{
			// Image can't be shrunk along (at least) one
			// of its dimensions, so don't bother
			// resampling.  Technically we should, but
			// it's pretty useless at this point.  Just
			// change the image dimensions and leave the
			// existing pixels.
		}
		else
		{
			// Resample.  Simple average 2x2 --> 1, in-place.
			int	pitch = image->m_pitch;
			for (int j = 0; j < new_h; j++) {
				Uint8*	out = ((Uint8*) image->m_data) + j * new_pitch;
				Uint8*	in = ((Uint8*) image->m_data) + (j << 1) * pitch;
				for (int i = 0; i < new_w; i++) {
					int	r, g, b, a;
					r = (*(in + 0) + *(in + 4) + *(in + 0 + pitch) + *(in + 4 + pitch));
					g = (*(in + 1) + *(in + 5) + *(in + 1 + pitch) + *(in + 5 + pitch));
					b = (*(in + 2) + *(in + 6) + *(in + 2 + pitch) + *(in + 6 + pitch));
					a = (*(in + 3) + *(in + 7) + *(in + 3 + pitch) + *(in + 7 + pitch));
					*(out + 0) = r >> 2;
					*(out + 1) = g >> 2;
					*(out + 2) = b >> 2;
					*(out + 3) = a >> 2;
					out += 4;
					in += 8;
				}
			}
		}

		// Munge image's members to reflect the shrunken image.
		image->m_width = new_w;
		image->m_height = new_h;
		image->m_pitch = new_pitch;
	}
};


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:

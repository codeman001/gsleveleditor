// gameswf_test_ogl.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003 -*- coding: utf-8;-*-

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A minimal test player app for the gameswf library.

#include "base/tu_memdebug.h"	// must be the first in the include list

#include <psgl/psgl.h>

#include "gameswf/gameswf.h"
#include <stdlib.h>
#include <stdio.h>
#include "base/utility.h"
#include "base/container.h"
#include "base/tu_file.h"
#include "base/tu_types.h"
#include "net/tu_net_file.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_player.h"


#ifdef _WIN32
#	include <Winsock.h>
#endif

void	print_usage()
// Brief instructions.
{
	printf(
		"gameswf_test_ogl -- a test player for the gameswf library.\n"
		"\n"
		"This program has been donated to the Public Domain.\n"
		"See http://tulrich.com/geekstuff/gameswf.html for more info.\n"
		"\n"
		"usage: gameswf_test_ogl [options] movie_file.swf\n"
		"\n"
		"Plays a SWF (Shockwave Flash) movie, using OpenGL and the\n"
		"gameswf library.\n"
		"\n"
		"options:\n"
		"\n"
		"  -h          Print this info.\n"
		"  -c          Produce a core file instead of letting SDL trap it\n"
		"  -d num      Number of milli-seconds to delay in main loop\n"
		"  -a <level>  Specify the antialiasing level (0,1,2,4,8,16,...)\n"
		"  -v          Be verbose; i.e. print log messages to stdout\n"
		"  -va         Be verbose about movie Actions\n"
		"  -vp         Be verbose about parsing the movie\n"
		"  -ml <bias>  Specify the texture LOD bias (float, default is -1)\n"
		"  -p          Run full speed (no sleep) and log frame rate\n"
		"  -1          Play once; exit when/if movie reaches the last frame\n"
		"  -r <0|1|2>  0 disables renderering & sound (good for batch tests)\n"
		"              1 enables rendering & sound (default setting)\n"
		"              2 enables rendering & disables sound\n"
		"  -t <sec>    Timeout and exit after the specified number of seconds\n"
		"  -b <bits>   Bit depth of output window (16 or 32, default is 16)\n"
		"  -n          Allow use of network to try to open resource URLs\n"
		"  -u          Allow pass user variables to Flash\n"
		"  -k          Disables cursor\n"
		"  -w <w>x<h>  Specify the window size, for example 1024x768\n"
		"\n"
		"keys:\n"
		"  CTRL-Q          Quit/Exit\n"
		"  CTRL-W          Quit/Exit\n"
		"  ESC             Quit/Exit\n"
		"  CTRL-P          Toggle Pause\n"
		"  CTRL-[ or kp-   Step back one frame\n"
		"  CTRL-] or kp+   Step forward one frame\n"
		"  CTRL-A          Toggle antialiasing\n"
		"  CTRL-T          Debug.  Test the set_variable() function\n"
		"  CTRL-G          Debug.  Test the get_variable() function\n"
		"  CTRL-M          Debug.  Test the call_method() function\n"
		"  CTRL-B          Toggle background color\n"
		);
}

#define OVERSIZE	1.0f

static bool s_antialiased = true;
static int s_bit_depth = 16;

// by default it's used the simplest and the fastest edge antialiasing method
// if you have modern video card you can use full screen antialiasing
// full screen antialiasing level may be 2,4,8,16, ...
static int s_aa_level = 1;

static bool s_background = true;
static bool s_measure_performance = false;
// Controls whether we will try to load things over the net or not.
static bool s_allow_http = false;

static void	message_log(const char* message)
// Process a log message.
{
	if (gameswf::get_verbose_parse())
	{
		fputs(message, stdout);
		fflush(stdout);
	}
}


static void	log_callback(bool error, const char* message)
// Error callback for handling gameswf messages.
{
	if (error)
	{
		// Log, and also print to stderr.
		message_log(message);
		fputs(message, stderr);
		fflush(stderr);
	}
	else
	{
		message_log(message);
	}
}


static tu_file*	file_opener(const char* url)
// Callback function.  This opens files for the gameswf library.
{
	return new tu_file(url, "rb");
}


static void	fs_callback(gameswf::character* movie, const char* command, const char* args)
// For handling notification callbacks from ActionScript.
{
//	message_log("fs_callback: '");
//	message_log(command);
//	message_log("' '");
//	message_log(args);
//	message_log("'\n");
}



// TODO: clean up this interface and re-enable.
//extern bool gameswf_tesselate_dump_shape;

smart_ptr<gameswf::player> player;
smart_ptr<gameswf::root> root;

void initGameSWF()
{
		assert(tu_types_validate());

		const char* infile = NULL;

		tu_string flash_vars;

		int	width = 1280;
		int	height = 720;

		player = new gameswf::player();

		//infile = "example.swf";
		infile = "sample1.swf";

		// use this for multifile games
		// workdir is used when LoadMovie("myfile.swf", _root) is called
		{
			tu_string workdir;
			// Find last slash or backslash.
 			const char* ptr = infile + strlen(infile);
			for (; ptr >= infile && *ptr != '/' && *ptr != '\\'; ptr--) {}
			// Use everything up to last slash as the "workdir".
			int len = ptr - infile + 1;
			if (len > 0)
			{
				tu_string workdir(infile, len);
				player->set_workdir(workdir.c_str());
			}
		}

		gameswf::register_file_opener_callback(file_opener);
		gameswf::register_fscommand_callback(fs_callback);
		if (gameswf::get_verbose_parse())
		{
			gameswf::register_log_callback(log_callback);
		}
		
		gameswf::sound_handler*	sound = NULL;
		gameswf::render_handler*	render = NULL;
		
		//if (do_sound) {
		//	sound = gameswf::create_sound_handler_sdl();
		//	gameswf::set_sound_handler(sound);
		//}
		render = gameswf::create_render_handler_ogl();
		gameswf::set_render_handler(render);
		gameswf::create_glyph_provider();

		// gameswf::set_use_cache_files(true);

		player->set_flash_vars(flash_vars);
		
		root = player->load_file(infile);
		if (root == NULL)
		{
			exit(1);
		}

		if (width == 0 || height == 0)
		{
			width = root->get_movie_width();
			height = root->get_movie_height();
		}
		float scale_x = (float) width / root->get_movie_width();
		float scale_y = (float) height / root->get_movie_height();

		float	movie_fps = root->get_movie_fps();

		render->open();
		render->set_antialiased(s_antialiased);

		glMatrixMode(GL_PROJECTION);
		glOrthof(-OVERSIZE, OVERSIZE, OVERSIZE, -OVERSIZE, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// We don't need lighting effects
		glDisable(GL_LIGHTING);
		// glColorPointer(4, GL_UNSIGNED_BYTE, 0, *);
		// glInterleavedArrays(GL_T2F_N3F_V3F, 0, *)
		//glPushAttrib (GL_ALL_ATTRIB_BITS);		

		//TODO
//		gameswf::player* p = gameswf::create_player();
//		p.run();

}

void updateGameSWF(int delta_t)
{
	float speed_scale = 1.0f;
	int width = 1280;
	int height = 720;

	smart_ptr<gameswf::root> root = player->get_root();
	root->set_display_viewport(0, 0, width, height);
	root->set_background_alpha(s_background ? 1.0f : 0.05f);

	//root->notify_mouse_state(mouse_x, mouse_y, mouse_buttons);

	//Uint32 t_advance = SDL_GetTicks();
	root->advance(delta_t * speed_scale);
	//t_advance = SDL_GetTicks() - t_advance;

	//frame_counter++;
}

void renderGameSWF()
{
	smart_ptr<gameswf::root> root = player->get_root();

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrthof(-OVERSIZE, OVERSIZE, OVERSIZE, -OVERSIZE, -1, 1); // this without glScalef(1,-1,1);
	////glOrthof(-OVERSIZE, OVERSIZE, -OVERSIZE, OVERSIZE, -1, 1); // or this with glScalef(1,-1,1);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	////glScalef(1,-1,1);

	//glDisable(GL_DEPTH_TEST);	// Disable depth testing.
	//glDisable(GL_CULL_FACE);

	//// Turn on alpha blending.
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//// Turn on line smoothing.  Antialiased lines can be used to
	//// smooth the outsides of shapes.
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// GL_NICEST, GL_FASTEST, GL_DONT_CARE

	//Uint32 t_display = SDL_GetTicks();
	root->display();
	//t_display = SDL_GetTicks() - t_display;
}

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:

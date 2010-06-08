#include "gl_helper.h"

namespace gl
{
	void init()
	{
		glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

		if (!global::fullscreen)
		{
			glutInitWindowSize(global::width, global::height);
			glutCreateWindow(global::app_title);
		}
		else
		{
			//global::width = glutGet(GLUT_SCREEN_WIDTH);
			//global::height = glutGet(GLUT_SCREEN_HEIGHT);
			//char buffer[80];
			//snprintf(buffer, 80, "%dx%d", global::width, global::height);
			//glutGameModeString(buffer);
			glutGameModeString("1024x768"); // locked at 1024x768 for performance reasons
			atexit(glutLeaveGameMode);
			glutEnterGameMode();
		}

#ifdef _WIN32
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			fprintf(stderr, "GLEW initialization failed (%d)!\n", err);
			exit(EXIT_FAILURE);
		}
#endif

		glEnable(GL_DEPTH_TEST);
		glClearColor(global::fill_color[0], global::fill_color[1], global::fill_color[2], global::fill_color[3]);

		glEnable(GL_NORMALIZE);

		// turn on alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// set pixel packing alignment as OGLFT expects
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
}

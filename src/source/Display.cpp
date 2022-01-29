//***************************************************************************//
//                                                                           //
// @File Name:     Display.cpp                                               //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Used to display the video on the screen.			         //
//                                                                           //
//***************************************************************************//

#include "../header/Display.h"
#include "../utils/Log.h"


Display::Display()
{	
	// vfc::logger = vfc::Log::getInstance();

	if(!glfwInit())
	{
		LOG_ERROR("Display:       couldn't initialize GLFW");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1400, 740, "Play..", NULL, NULL);
	if (!window)
	{
		LOG_ERROR("Display:       Window or OpenGL context creation failed!");
	}

	glfwMakeContextCurrent(window);

	// Generate texture
    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// Set up orphographic projection
	int window_width, window_height;
	glfwGetFramebufferSize(window, &window_width, &window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window_width, window_height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	LOG_DEBUG("Display:       Constructor of the object: ", this);
}

Display::~Display()
{
	LOG_DEBUG("Display:       Destructor  of the object: ", this);
}

void Display::showMedia(int const& frame_width, int const& frame_height, uint8_t ** frame_data, double const& time)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSetTime(0.0);

	while(time > glfwGetTime())
	{
        glfwWaitEventsTimeout(time - glfwGetTime());
        // glfwWaitEventsTimeout(time);
    }
	
	// Rendering the video stream
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, *frame_data);
	
	glBegin(GL_QUADS);
	    glTexCoord2d(0,0); glVertex2i(150, 150);
	    glTexCoord2d(1,0); glVertex2i(frame_width, 150);
	    glTexCoord2d(1,1); glVertex2i(frame_width,frame_height);
	    glTexCoord2d(0,1); glVertex2i(150, frame_height);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glfwSwapBuffers(window);

}

void Display::destroyWindow()
{
	glfwWindowShouldClose(window);
	glfwDestroyWindow(window);
	glfwTerminate();
}


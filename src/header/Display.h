//***************************************************************************//
//                                                                           //
// @File Name:     Display.h                                                 //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Used to display the video on the screen.			         //
//                                                                           //
//***************************************************************************//

#pragma once

#include <GLFW/glfw3.h>
#include <iostream>


class Display
{
public:
	GLFWwindow* window;
	GLuint tex_handle;

public:
	Display();
	~Display();
	void showMedia(int const& frame_width, int const& frame_height, uint8_t ** data, double const& time);
	void destroyWindow();
};


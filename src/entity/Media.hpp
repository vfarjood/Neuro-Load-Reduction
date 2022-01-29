//***************************************************************************//
//                                                                           //
// @File Name:     Media.hpp                                                 //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   It is a warper class for those medias that has both Video //
//				   and Audio attributes. 									 //
//                                                                           //
//***************************************************************************//

#pragma once

#include "../utils/Log.h"

#include "../entity/MediaType.hpp"
#include "../entity/Video.hpp"
#include "../entity/Audio.hpp"

struct Media
{

	std::string media_path;
	Video video;
	Audio audio;

	Media(){LOG_DEBUG("Media:         Constructor of the object: ", this);}
	~Media(){LOG_DEBUG("Media:         Destructor  of the object: ", this);}

	void setPath(std::string const& filePath) { media_path = filePath;}
	std::string getPath()  { return media_path;}

};


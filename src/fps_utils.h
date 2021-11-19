//**********************************************************************
// fps utility functions
// usage:
//
//      auto scoped_fps = create_scoped_fps();
//      object_detect(...);
//      std::string fps = fps_to_string(get_fps());
//
// History:
//	No.		modify_date		author		content             email
//	1		2021-11-11		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef FPS_UTILS_H
#define FPS_UTILS_H

#include <memory>

class CScopedFPS;

//////////////////////////////////////// interface ////////////////////////////////////////

//create scoped fps instance
std::shared_ptr<CScopedFPS> create_scoped_fps(void);

//get fps
float get_fps(void);

//convert fps to string
std::string fps_to_string(float fps, unsigned int precision = 2);

//////////////////////////////////////// implement ////////////////////////////////////////

// scoped fps class
class CScopedFPS
{
public:
	typedef CScopedFPS											my_type;

////constructor, destructor
public:
	// default constructor
	CScopedFPS(void);
	// destructor
	~CScopedFPS(void);
private:
	// copy constructor
	CScopedFPS(const my_type&) = delete;
	// operator = override
	my_type& operator=(const my_type&) = delete;
};

#endif  //FPS_UTILS_H

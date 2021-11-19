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

#include "fps_utils.h"
#include <chrono>
#include <sstream>
#include <iomanip>

//////////////////////////////////////// FPS class ////////////////////////////////////////

// FPS class
class CFPS
{
public:
	typedef CFPS    											my_type;
    typedef std::chrono::steady_clock::time_point               time_type;
    enum {prpFPSPoolSize = 16};

//// constructor, destructor
public:
    // get singleton instance
    static my_type*& get_instance(void);
	// destructor
	~CFPS(void);
private:
	// default constructor
	CFPS(void);
	// copy constructor
	CFPS(const my_type&) = delete;
	// operator = override
	my_type& operator=(const my_type&) = delete;

//// query
public:
    // get fps
    float get_fps(void) const;
    // get current timestamp
    static time_type now(void);

//// command
public:
    // start timeing
    void frame_timeing_begin(void);
    // end timeing
    void frame_timeing_end(void);

//// member data
private:
    unsigned int _frame_count;                   // frame count
    float        _fps_pool[prpFPSPoolSize];      // fps pool
    time_type    _frame_time_begin;              // frame begin timestamp
};

// get singleton instance
CFPS*& CFPS::get_instance(void)
{
    static my_type* s_pInstance = nullptr;
    if(s_pInstance == nullptr)
    {
        s_pInstance = new my_type();
    }
    return s_pInstance;
}

//default constructor
CFPS::CFPS(void) : _frame_count(0)
{
    for(int i=0; i<prpFPSPoolSize; ++i)
        _fps_pool[i] = 0.0;
}

// destructor
CFPS::~CFPS(void)
{
}

// get fps
float CFPS::get_fps(void) const
{
    unsigned int dSummarySize = prpFPSPoolSize < _frame_count ? prpFPSPoolSize : _frame_count;
    if(dSummarySize > 0)
    {
        float dSum = 0;
        for(unsigned int i = 0; i < dSummarySize; ++i)
            dSum += _fps_pool[i];
        return dSum / dSummarySize;
    }
    return 0;
}

// get current timestamp
CFPS::time_type CFPS::now(void)
{
    return std::chrono::steady_clock::now();
}

// start timeing
void CFPS::frame_timeing_begin(void)
{
    _frame_time_begin = now();
}

// end timeing
void CFPS::frame_timeing_end(void)
{
    unsigned long elapse_milliseconds = std::chrono::duration_cast <std::chrono::milliseconds> (now() - _frame_time_begin).count();
    if(elapse_milliseconds > 0)
       _fps_pool[(_frame_count++) % prpFPSPoolSize] = 1000.0f / elapse_milliseconds;
}

// get singleton instance
CFPS*& get_fps_instance(void)
{
    return CFPS::get_instance();
}

//////////////////////////////////////// scoped fps ////////////////////////////////////////

// default constructor
CScopedFPS::CScopedFPS(void)
{
    get_fps_instance()->frame_timeing_begin();
}

// destructor
CScopedFPS::~CScopedFPS(void)
{
    get_fps_instance()->frame_timeing_end();
}

//////////////////////////////////////// implement ////////////////////////////////////////

//create scoped fps instance
std::shared_ptr<CScopedFPS> create_scoped_fps(void)
{
    return std::make_shared<CScopedFPS>();
}

//get fps
float get_fps(void)
{
    return get_fps_instance()->get_fps();
}

//convert fps to string
std::string fps_to_string(float fps, unsigned int precision)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << fps;
    return ss.str();
}

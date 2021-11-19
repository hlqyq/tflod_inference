//**********************************************************************
// log utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2020-11-04		hlq		    generate            66536382@qq.com
//*********************************************************************

#include "utils.h"
#include <iostream>
#include <string>
#include <cassert>

void default_log_handler(void* p_user_data, int severity_level, const char* message);

static log_handler_type	s_pLogHandler = &default_log_handler;
static void*			s_pLogHandlerUserData = 0;

// set log handler
void set_log_handler(log_handler_type log_handler, void* p_user_data)
{
	s_pLogHandler = log_handler ? log_handler : default_log_handler;
	s_pLogHandlerUserData = log_handler ? p_user_data : 0;
}

// print log message
void print_log(int severity_level, const char* message)
{
	if (s_pLogHandler)
		(*s_pLogHandler)(s_pLogHandlerUserData, severity_level, message);
}

// default log handler
void default_log_handler(void* p_user_data, int severity_level, const char* message)
{
	switch (severity_level)
	{
	case LOG_VERBOSE:
	case LOG_INFO:
	case LOG_WARNING:
		std::cout << message << std::endl;
		break;
	case LOG_ERROR:
	case LOG_ERROR_REPORT:
	case LOG_FATAL:
		std::cerr << message << std::endl;
		break;
	}
}

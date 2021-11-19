//**********************************************************************
// log utility functions
// History:
//	No.		modify_date		author		content             email
//	1		2020-11-04		hlq		    generate            66536382@qq.com
//*********************************************************************

#ifndef _UTILS_H
#define _UTILS_H

#include <string.h>
#include <sstream>

// log priority
enum{LOG_VERBOSE = -1, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_ERROR_REPORT, LOG_FATAL, LOG_NUM_SEVERITIES};
typedef void(*log_handler_type)(void* p_user_data, int severity_level, const char* message);

// print log message
void print_log(int severity_level, const char* message);

// set log handler
void set_log_handler(log_handler_type log_handler, void* p_user_data);

// log message class
class CLogMessage
{
public:
	typedef CLogMessage											my_type;

////constructor, destructor
public:
	//constructor
	CLogMessage(const char* file_name, int line_number, int severity_level) : _severity_level(severity_level)
	{
		//print prefix
		_ostream << '[';

		//print severity level
		const char* const s_log_severity_names[LOG_NUM_SEVERITIES] = { "INFO", "WARNING", "ERROR", "ERROR_REPORT", "FAULT"};
		if (severity_level >= 0)
			_ostream << s_log_severity_names[severity_level];
		else
			_ostream << "detail " << -severity_level;

		// output file name
		if (file_name && (*file_name != '\0'))
		{
			const char* psz1 = strrchr(file_name, '\\');
			const char* psz2 = strrchr(file_name, '/');
			const char* psz = psz1 < psz2 ? psz2 : psz1;
			_ostream << ':' << (psz ? psz + 1 : file_name) << '(' << line_number << ')';
		}

		//print suffix
		_ostream << "] ";
		_message_offset = static_cast<unsigned int>(_ostream.tellp());
	}
	//destructor
	~CLogMessage(void) { print_log(_severity_level, _ostream.str().c_str()); }
private:
	// copy constructor
	CLogMessage(const my_type&) = delete;
	// openator = override
	my_type& operator=(const my_type&) = delete;

//// query
public:
	// get output stream
	std::ostream& stream(void) { return _ostream; }

//// member data
private:
	int					_severity_level;
	std::ostringstream	_ostream;
	unsigned int		_message_offset;
};

#define LOG(severity)									CLogMessage(__FILE__, __LINE__,LOG_ ## severity).stream()

// output string stream
class CStringStream
{
public:
	typedef CStringStream											my_type;

//// constructor, destructor
public:
	// constructor
	CStringStream(std::string& result_string) : _result_string(result_string){}
	// destructor
	~CStringStream(void) { _result_string = _ostream.str(); }

//// query
public:
	// get output stream
	std::ostream& stream(void) { return _ostream; }

//// member data
private:
	std::string&			_result_string;
	std::ostringstream		_ostream;
};

// string stream
#define STRING_STREAM(result_string)									CStringStream(result_string).stream()

// scoped guard
template<typename T>
class CScopedGuard
{
public:
    CScopedGuard(T invoke_cb) : _invoke_cb(invoke_cb), _invoked(false) {}
    ~CScopedGuard(void) {invoke();}

public:
    // test is invoked
    bool is_invoked(void) const { return _invoked;}
    // invoke callback
    void invoke(void)
    {
        if(!_invoked)
        {
            _invoke_cb();
            _invoked = true;
        }
    }
    //reset
    void reset(void)
    {
        _invoked = true;
    }

private:
    T           _invoke_cb;
    bool        _invoked;
};

template<typename T>
inline CScopedGuard<T> create_scoped_guard(T cb)
{
    return CScopedGuard<T>(cb);
}

#endif //_UTILS_H

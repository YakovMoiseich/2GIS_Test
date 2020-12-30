#pragma once
#include <iomanip>
#include <sstream>

class main
{
public:
	static std::string readable_duration(std::string out_prefix, long long duration)
	{
		std::ostringstream oss;
		oss << std::setfill('0')          // set field fill character to '0'
		<< (duration % 1000000000) / 1000000 // format seconds
		<< " sec "
		<< std::setw(3)               // set width of milliseconds field
		<< (duration % 1000000) / 1000       // format milliseconds
		<< " ms "
		<< std::setw(3)               // set width of microseconds field
		<< duration % 1000
		<< " mcs";                  // format microseconds
		return out_prefix + oss.str();
	}
};

#include "user_profile.h"
#include <regex>
#include "../sprite.h"
#include "../system.h"
#include "../file_system.h"
#include "setting.h"

fr::UserProfile *fr::UserProfile::m_instance = 0;

void fr::UserProfile::init()
{
	m_user_name = "Guest";
	m_performance_point = 0;

	if (!read())
	{
		write();
	}

}

void fr::UserProfile::clear()
{

}


std::string fr::UserProfile::GetUserName()
{
	return m_user_name;
}

int fr::UserProfile::GetPerformancePoint()
{
	return m_performance_point;
}

void fr::UserProfile::SetUserName(std::string input)
{
	m_user_name = input;
	write();
}

int fr::UserProfile::CalculatePerformancePoint(int difficulty, double acc, bool record)
{
	int output = difficulty * 1000 * (acc - 0.6f);
	output = output > 0 ? output : 0;
	if (record)
	{
		m_performance_point = output > m_performance_point ? output : m_performance_point;
		write();
	}
	return output;
}

bool fr::UserProfile::read()
{
	std::string file;
	std::regex user_name_pattern("user_name:(.+?)\\n");
	std::regex performance_point_pattern("performance_point:(\\d+?)\\n");
	std::smatch user_name_line;
	std::smatch performance_point_line;

	if (!ReadFile(Setting::instance()->GetUserProfilePath(), file))
	{
		return false;
	}

	std::regex_search(file, user_name_line, user_name_pattern);
	std::regex_search(file, performance_point_line, performance_point_pattern);

	m_user_name = std::regex_replace(user_name_line.str(), user_name_pattern, "$1");
	m_performance_point = atoi(std::regex_replace(performance_point_line.str(), performance_point_pattern, "$1").c_str());
	return true;
}

bool fr::UserProfile::write()
{
	std::string file;
	char *performance_point_ch = new char[50];
	sprintf(performance_point_ch, "performance_point:%d\n", m_performance_point);
	file = "Force Rush user profile\n";
	file += "user_name:" + m_user_name + "\n";
	file += performance_point_ch;
	file += "\n";
	file += "End of file\n";
	delete [] performance_point_ch;
	
	WriteFile(Setting::instance()->GetUserProfilePath(), file);
	return true;
}
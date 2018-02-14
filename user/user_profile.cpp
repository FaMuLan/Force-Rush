#include "user_profile.h"
#include <boost/regex.hpp>
#include "../sprite.h"
#include "../system.h"
#include "../file_system.h"
#include "setting.h"

fr::UserProfile *fr::UserProfile::m_instance = 0;

void fr::UserProfile::init()
{
	m_user_name = "Guest";
	m_performance_point = 0;
	m_char_path = "assets/base/char.png";
	m_controller_path = "assets/base/controller.png";
	m_char = new Sprite;
	m_controller = new Sprite;

	if (!read())
	{
		write();
	}

	m_char->init(m_char_path);
	m_controller->init(m_controller_path, Rect(0, 0, 718.f * (System::instance()->GetWindowWidth() / 720.f), 196.f * (System::instance()->GetWindowHeigh() / 1280.f * Setting::instance()->GetDrawScale())));
	m_controller->SetPos(System::instance()->GetWindowWidth() / 2 - m_controller->GetW() / 2, System::instance()->GetWindowHeigh() - m_controller->GetH());

	m_char->SetPos(System::instance()->GetWindowWidth() / 2 - (m_char->GetW() * (float(System::instance()->GetWindowHeigh()) / float(m_char->GetH()))) / 2, 0);
	m_char->SetScale(float(System::instance()->GetWindowHeigh()) / float(m_char->GetH()));
}

void fr::UserProfile::clear()
{
	m_char->clear();
	m_controller->clear();
}

void fr::UserProfile::update()
{
	if (System::instance()->IsWindowModified())
	{
		m_char->SetPos(System::instance()->GetWindowWidth() / 2 - (m_char->GetW() * (float(System::instance()->GetWindowHeigh()) / float(m_char->GetH()))) / 2, 0);
		m_char->SetScale(float(System::instance()->GetWindowHeigh()) / float(m_char->GetH()));
//		m_controller->SetSize(718 * (System::instance()->GetWindowWidth() / 720.f), 196 * (System::instance()->GetWindowHeigh() / 1280.f * Setting::instance()->GetDrawScale()));
		m_controller->SetPos(System::instance()->GetWindowWidth() / 2 - m_controller->GetW() / 2, System::instance()->GetWindowHeigh() - m_controller->GetH());
	}
}

void fr::UserProfile::RenderCharacter()
{
	m_char->render();
}

void fr::UserProfile::RenderController()
{
	m_controller->render();
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

int fr::UserProfile::CalculatePerformancePoint(int difficulty, double acc)
{
	int output = difficulty * 1000 * (acc - 0.6f);
	output = output > 0 ? output : 0;
	m_performance_point = output > m_performance_point ? output : m_performance_point;
	write();
	return output;
}

bool fr::UserProfile::read()
{
	std::string file;
	boost::regex user_name_pattern("user_name:(.+?)\\n");
	boost::regex performance_point_pattern("performance_point:(\\d+?)\\n");
	boost::regex char_path_pattern("char_path:(.+?)\\n");
	boost::regex controller_path_pattern("controller_path:(.+?)\\n");
	boost::smatch user_name_line;
	boost::smatch performance_point_line;
	boost::smatch char_path_line;
	boost::smatch controller_path_line;

	if (!ReadFile(Setting::instance()->GetUserProfilePath(), file))
	{
		return false;
	}

	boost::regex_search(file, user_name_line, user_name_pattern);
	boost::regex_search(file, performance_point_line, performance_point_pattern);
	boost::regex_search(file, char_path_line, char_path_pattern);
	boost::regex_search(file, controller_path_line, controller_path_pattern);

	m_user_name = boost::regex_replace(user_name_line.str(), user_name_pattern, "$1");
	m_performance_point = atoi(boost::regex_replace(performance_point_line.str(), performance_point_pattern, "$1").c_str());
	m_char_path = boost::regex_replace(char_path_line.str(), char_path_pattern, "$1");
	m_controller_path = boost::regex_replace(controller_path_line.str(), controller_path_pattern, "$1");
	return true;
}

bool fr::UserProfile::write()
{
	std::string file;
	char *performance_point_ch = new char[20];
	sprintf(performance_point_ch, "performance_point:%d\n", m_performance_point);
	file = "Force Rush user profile\n";
	file += "user_name:" + m_user_name + "\n";
	file += performance_point_ch;
	file += "char_path:" + m_char_path + "\n";
	file += "controller_path:" + m_controller_path + "\n";
	file += "End of file\n";
	delete [] performance_point_ch;
	
	WriteFile(Setting::instance()->GetUserProfilePath(), file);
	return true;
}
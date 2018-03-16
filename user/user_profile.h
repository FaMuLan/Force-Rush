#ifndef FORCE_RUSH_USER_PROFILE_H
#define FORCE_RUSH_USER_PROFILE_H

#include <string>

namespace fr
{
	class Sprite;

	class UserProfile
	{
		public:
			static UserProfile *instance()
			{
				if (m_instance == 0)
				{
					m_instance = new UserProfile;
					return m_instance;
				}
				return m_instance;
			}
			void init();
			void clear();
			std::string GetUserName();
			int GetPerformancePoint();
			void SetUserName(std::string input);
			int CalculatePerformancePoint(int difficulty, double acc, bool record = false);
			bool read();
			bool write();
		private:
			UserProfile() {}
			~UserProfile() {}
			static UserProfile *m_instance;
			std::string m_user_name;
			int m_performance_point;
	};
};

#endif
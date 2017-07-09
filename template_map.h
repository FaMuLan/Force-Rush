#ifndef LUNATIC_MELODY_TEMPLATE_MAP_H
#define LUNATIC_MELODY_TEMPLATE_MAP_H

#include <string>
#include <map>

namespace lm
{
	enum ValueType
	{
		VALUE_TYPE_INT = 0,
		VALUE_TYPE_FLOAT,
		VALUE_TYPE_BOOL,
		VALUE_TYPE_STRING
	};

	union Template
	{
		int &t_int;
		float &t_float;
		bool &t_bool;
		std::string &t_string;
	};

	struct TemplateMapUnit
	{
		Template value;
		ValueType type;
	};

	class TemplateMap
	{
		public:
			static TemplateMap *instance()
			{
				if (m_instance == NULL)
				{
					m_instance = new TemplateMap();
					return m_instance;
				}
				return m_instance;
			}

			bool PushValue(std::string id, int &value);
			bool PushValue(std::string id, float &value);
			bool PushValue(std::string id, bool &value);
			bool PushValue(std::string id, std::string &value);

			bool IsInt(std::string id);
			bool IsFloat(std::string id);
			bool IsBool(std::string id);
			bool IsString(std::string id);

			int &GetInt(std::string id);
			float &GetFloat(std::string id);
			bool &GetBool(std::string id);
			std::string &GetString(std::string id);

		private:
			TemplateMap() {}
			~TemplateMap() {}
			static TemplateMap *m_instance;
			std::map<std::string, TemplateMapUnit*> m_unit;
	};	//class TemplateMap
};	//namespace lm

#endif	//LUJATIC_MELODY_TEMPLATE_MAP_H
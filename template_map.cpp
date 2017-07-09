#include "template_map.h"

lm::TemplateMap *lm::TemplateMap::m_instance = 0;

inline bool lm::TemplateMap::PushValue(std::string id, int &value)
{
	TemplateMapUnit *new_unit = new TemplateMapUnit;
	new_unit->value.t_int = value;
	new_unit->type = VALUE_TYPE_INT;
	if (m_unit[id] != NULL)
	{
		delete new_unit;
		return false;
	}
	m_unit[id] = new_unit;
	return true;
}
inline bool lm::TemplateMap::PushValue(std::string id, float &value)
{
	TemplateMapUnit *new_unit = new TemplateMapUnit;
	new_unit->value.t_float = value;
	new_unit->type = VALUE_TYPE_FLOAT;
	if (m_unit[id] != NULL)
	{
		delete new_unit;
		return false;
	}
	m_unit[id] = new_unit;
	return true;
}
inline bool lm::TemplateMap::PushValue(std::string id, bool &value)
{
	TemplateMapUnit *new_unit = new TemplateMapUnit;
	new_unit->value.t_bool = value;
	new_unit->type = VALUE_TYPE_BOOL;
	if (m_unit[id] != NULL)
	{
		delete new_unit;
		return false;
	}
	m_unit[id] = new_unit;
	return true;
}
inline bool lm::TwmplateMap::PushValue(std::string id, std::string &value)
{
	TemplateMapUnit *new_unit = new TemplateMapUnit;
	new_unit->value.t_string = value;
	new_unit->type = VALUE_TYPE_STRING;
	if (m_unit[id] != NULL)
	{
		delete new_unit;
		return false;
	}
	m_unit[id] = new_unit;
	return true;	
}

inline bool lm::TemplateMap::IsInt(std::string id)
{
	return m_unit[id]->type == VALUE_TYPE_INT;
}
inline bool lm::TemplateMap::IsFloat(std::string id)
{
	return m_unit[id]->type == VALUE_TYPE_FLOAT;
}
inline bool lm::TemplateMap::IsBool(std::string id)
{
	return m_unit[id]->type == VALUE_TYPE_BOOL;
}
inline bool lm::TemplateMap::IsString(std::string id)
{
	return m_unit[id]->type == VALUE_TYPE_STRING;
}

inline int &lm::TemplateMap::GetInt(std::string id)
{
	return m_unit[id]->value.t_int;
}
inline float &lm::TemplateMap::GetFloat(std::string id)
{
	return m_unit[id]->value.t_float;
}
inline bool &lm::TemplateMap::GetBool(std::string id)
{
	return m_unit[id]->value.t_bool;
}
inline std::string &lm::TemplateMap::GetString(std::string id)
{
	return m_unit[id]->value.t_string;
}
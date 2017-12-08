#include "iniParser.h"

IniParser::IniParser(std::string fileName)
{
	this->fileName = fileName;
	Parse();
}

void IniParser::PrintFile() const
{
	if (fileName == "")
	{
		return;
	}
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw FileNotFound();
		return;
	}
	std::string temp;
	while (!file.eof())
	{
		getline(file, temp);
		std::cout << temp << std::endl;
	}
}

void IniParser::Parse()
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw FileNotFound();
		return;
	}
	std::string temp;
	std::string currentSection;
	std::regex section(sectionRegex);
	std::regex keyValue(keyValueRegex);
	std::regex comment(commentRegex);
	std::smatch matches;
	while (getline(file, temp))
	{
		if (regex_match(temp, matches, section))
		{
			currentSection = matches[2];
			data.insert(std::pair<std::string, std::unordered_map<std::string, std::string>>(currentSection, {}));
		}
		else if (regex_match(temp, matches, keyValue))
		{
			std::string key = matches[1], value = matches[2];
			data[currentSection].insert(std::pair<std::string, std::string>(key, value));
		}
		else if (regex_match(temp, comment))
		{
			// Nothing to do here, comment or just empty line
		}
		else
		{
			// Even when this exception is thrown, we'll continue parsing our INI file
			throw DummyContent();
		}
	}
}

void IniParser::Dump(std::ostream &output) const
{
	for each (auto section in data)
	{
		output << "[" << section.first << "]" << std::endl;
		for each (auto parameter in section.second)
		{
			output << parameter.first << " = " << parameter.second << std::endl;
		}
		output << std::endl;
	}
}

template <>
int IniParser::GetValue<int>(std::string section, std::string key) const
{
	try
	{
		return stoi(data.at(section).at(key));
	}
	catch (const std::out_of_range&)
	{
		throw ParameterNotFound();
	}
	catch (const std::invalid_argument&)
	{
		throw BadIniCast();
	}
}

template <>
double IniParser::GetValue<double>(std::string section, std::string key) const
{
	try
	{
		return stod(data.at(section).at(key));
	}
	catch (const std::out_of_range&)
	{
		throw ParameterNotFound();
	}
	catch (const std::invalid_argument&)
	{
		throw BadIniCast();
	}
}

template <>
std::string IniParser::GetValue<std::string>(std::string section, std::string key) const
{
	try
	{
		return data.at(section).at(key);
	}
	catch (const std::out_of_range&)
	{
		throw ParameterNotFound();
	}
}

template <>
void IniParser::SetValue<int>(std::string section, std::string key, int Value)
{
	data[section][key] = std::to_string(Value);
}

template <>
void IniParser::SetValue<double>(std::string section, std::string key, double value)
{
	data[section][key] = std::to_string(value);
}

template <>
void IniParser::SetValue<std::string>(std::string section, std::string key, std::string value)
{
	data[section][key] = value;
}
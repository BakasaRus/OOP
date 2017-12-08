#include "iniParser.h"

IniParser::IniParser(std::string FileName)
{
	this->FileName = FileName;
	Parse();
}

void IniParser::PrintFile() const
{
	if (FileName == "")
	{
		return;
	}
	std::ifstream File(FileName);
	if (!File.is_open())
	{
		throw FileNotFound();
		return;
	}
	std::string Temp;
	while (!File.eof())
	{
		getline(File, Temp);
		std::cout << Temp << std::endl;
	}
}

void IniParser::Parse()
{
	std::ifstream File(FileName);
	if (!File.is_open())
	{
		throw FileNotFound();
		return;
	}
	std::string Temp;
	std::string CurrentSection;
	std::regex Section(SectionRegEx);
	std::regex KeyValue(KeyValueRegEx);
	std::regex Comment(CommentRegEx);
	std::smatch Matches;
	while (getline(File, Temp))
	{
		if (regex_match(Temp, Matches, Section))
		{
			CurrentSection = Matches[2];
			Data.insert(std::pair<std::string, std::unordered_map<std::string, std::string>>(CurrentSection, {}));
		}
		else if (regex_match(Temp, Matches, KeyValue))
		{
			std::string Key = Matches[1], Value = Matches[2];
			Data[CurrentSection].insert(std::pair<std::string, std::string>(Key, Value));
		}
		else if (regex_match(Temp, Comment))
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

void IniParser::Dump(std::ostream &Output) const
{
	for each (auto Section in Data)
	{
		Output << "[" << Section.first << "]" << std::endl;
		for each (auto Parameter in Section.second)
		{
			Output << Parameter.first << " = " << Parameter.second << std::endl;
		}
		Output << std::endl;
	}
}

int IniParser::GetInt(std::string Section, std::string Key) const
{
	try
	{
		return stoi(Data.at(Section).at(Key));
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

double IniParser::GetDouble(std::string Section, std::string Key) const
{
	try
	{
		return stof(Data.at(Section).at(Key));
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

std::string IniParser::GetString(std::string Section, std::string Key) const
{
	try
	{
		return Data.at(Section).at(Key);
	}
	catch (const std::out_of_range&)
	{
		throw ParameterNotFound();
	}
}

void IniParser::SetInt(std::string Section, std::string Key, int Value)
{
	Data[Section][Key] = std::to_string(Value);
}

void IniParser::SetDouble(std::string Section, std::string Key, double Value)
{
	Data[Section][Key] = std::to_string(Value);
}

void IniParser::SetString(std::string Section, std::string Key, std::string Value)
{
	Data[Section][Key] = Value;
}
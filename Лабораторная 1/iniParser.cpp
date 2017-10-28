#include "iniParser.h"
#include "exceptions.h"

IniParser::IniParser()
{

}

IniParser::IniParser(string FileName)
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
	ifstream File(FileName);
	if (!File.is_open())
	{
		throw FileNotFound();
		return;
	}
	string Temp;
	while (!File.eof())
	{
		getline(File, Temp);
		cout << Temp << endl;
	}
}

void IniParser::Parse()
{
	ifstream File(FileName);
	if (!File.is_open())
	{
		throw FileNotFound();
		return;
	}
	string Temp;
	string CurrentSection;
	regex Section(SectionRegEx);
	regex KeyValue(KeyValueRegEx);
	regex Comment(CommentRegEx);
	smatch Matches;
	while (getline(File, Temp))
	{
		if (regex_match(Temp, Matches, Section))
		{
			CurrentSection = Matches[2];
			Data.insert(pair<string, unordered_map<string, string>>(CurrentSection, {}));
		}
		else if (regex_match(Temp, Matches, KeyValue))
		{
			string Key = Matches[1], Value = Matches[2];
			Data[CurrentSection].insert(pair<string, string>(Key, Value));
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

void IniParser::Dump(ostream &Output) const
{
	for each (auto Section in Data)
	{
		Output << "[" << Section.first << "]" << endl;
		for each (auto Parameter in Section.second)
		{
			Output << Parameter.first << " = " << Parameter.second << endl;
		}
		Output << endl;
	}
}

int IniParser::GetInt(string Section, string Key) const
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

double IniParser::GetDouble(string Section, string Key) const
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

string IniParser::GetString(string Section, string Key) const
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

void IniParser::SetInt(string Section, string Key, int Value)
{
	Data[Section][Key] = std::to_string(Value);
}

void IniParser::SetDouble(string Section, string Key, double Value)
{
	Data[Section][Key] = std::to_string(Value);
}

void IniParser::SetString(string Section, string Key, string Value)
{
	Data[Section][Key] = Value;
}

IniParser::~IniParser()
{

}
#pragma once

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <regex>

using namespace std;

class IniParser
{
	// If you have a better way to create multidimensional associative arrays, just let me know about it
	unordered_map<string, unordered_map<string, string>> Data;

	// INI filename
	string FileName;

	// Regex detecting a comment line
	const string CommentRegEx = R"(^(\s*(;|#).*)?$)";

	// Regex detecting a section line
	const string SectionRegEx = R"(^(\[([a-zA-Z_0-9]*)\]){1}\s*((;|#).*)?$)";

	// Regex detecting a key-value line
	const string KeyValueRegEx = R"(^([a-zA-Z_0-9]+){1}\s*=\s*([/a-zA-Z_0-9\.\-]+){1}\s*((;|#).*)?$)";

public:
	// Default constructor which creates a brand new INI file in memory
	IniParser();

	// Constructor which reads content from specified INI file
	IniParser(string FileName);

	// Dump file from disk (with comments, empty lines etc.) just to debug our class
	void PrintFile() const;

	// Parser which reads INI file into memory
	void Parse();

	// Dump our INI file from memory (only with sections and key-value pairs)
	void Dump(ostream &Output = cout) const;

	// Destructor. Pretty self-descripting
	~IniParser();



	// Getter for integer values
	int GetInt(string Section, string Key) const;

	// Getter for double values
	double GetDouble(string Section, string Key) const;

	// Getter for string values
	string GetString(string Section, string Key) const;



	// Setter for integer values
	void SetInt(string Section, string Key, int Value);

	// Setter for double values
	void SetDouble(string Section, string Key, double Value);

	// Setter for string values
	void SetString(string Section, string Key, string Value);
};

#pragma once

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <regex>
#include "exceptions.h"

class IniParser
{
	// If you have a better way to create multidimensional associative arrays, just let me know about it
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> Data;

	// INI filename
	std::string FileName;

	// Regex detecting a comment line
	const std::string CommentRegEx = R"(^(\s*(;|#).*)?$)";

	// Regex detecting a section line
	const std::string SectionRegEx = R"(^(\[([a-zA-Z_0-9]*)\]){1}\s*((;|#).*)?$)";

	// Regex detecting a key-value line
	const std::string KeyValueRegEx = R"(^([a-zA-Z_0-9]+){1}\s*=\s*([/a-zA-Z_0-9\.\-]+){1}\s*((;|#).*)?$)";

public:
	// Default constructor which creates a brand new INI file in memory
	IniParser() = default;

	// Constructor which reads content from specified INI file
	IniParser(std::string FileName);

	// Dump file from disk (with comments, empty lines etc.) just to debug our class
	void PrintFile() const;

	// Parser which reads INI file into memory
	void Parse();

	// Dump our INI file from memory (only with sections and key-value pairs)
	void Dump(std::ostream &Output = std::cout) const;

	// Destructor. Pretty self-descripting
	~IniParser() = default;



	// Getter for integer values
	int GetInt(std::string Section, std::string Key) const;

	// Getter for double values
	double GetDouble(std::string Section, std::string Key) const;

	// Getter for string values
	std::string GetString(std::string Section, std::string Key) const;



	// Setter for integer values
	void SetInt(std::string Section, std::string Key, int Value);

	// Setter for double values
	void SetDouble(std::string Section, std::string Key, double Value);

	// Setter for string values
	void SetString(std::string Section, std::string Key, std::string Value);
};

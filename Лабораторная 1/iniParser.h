#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <regex>
#include "exceptions.h"

class IniParser
{
	// If you have a better way to create multidimensional associative arrays, just let me know about it
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

	// INI filename
	std::string fileName;

	// Regex detecting a comment line
	const std::string commentRegex = R"(^(\s*(;|#).*)?$)";

	// Regex detecting a section line
	const std::string sectionRegex = R"(^(\[([a-zA-Z_0-9]*)\]){1}\s*((;|#).*)?$)";

	// Regex detecting a key-value line
	const std::string keyValueRegex = R"(^([a-zA-Z_0-9]+){1}\s*=\s*([/a-zA-Z_0-9\.\-]+){1}\s*((;|#).*)?$)";

public:
	// Default constructor which creates a brand new INI file in memory
	IniParser() = default;

	// Constructor which reads content from specified INI file
	IniParser(std::string fileName);

	// Dump file from disk (with comments, empty lines etc.) just to debug our class
	void PrintFile() const throw(FileNotFound);

	// Parser which reads INI file into memory
	void Parse() throw(FileNotFound, DummyContent);

	// Dump our INI file from memory (only with sections and key-value pairs)
	void Dump(std::ostream &output = std::cout) const throw();

	// Destructor. Pretty self-descripting
	~IniParser() = default;

	// Getter
	template <class T>
	T GetValue(std::string section, std::string key) const throw(ParameterNotFound, BadIniCast);

	// Setter
	template <class T>
	void SetValue(std::string section, std::string key, T value) throw();
};
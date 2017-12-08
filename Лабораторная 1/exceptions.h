#pragma once

class FileNotFound : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "File not found";
	}
};

class DummyContent : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Dummy content in INI file";
	}
};

class ParameterNotFound : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Can't find section or key in INI file";
	}
};

class BadIniCast : public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Can't convert value to specified type";
	}
};
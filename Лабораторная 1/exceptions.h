#pragma once

using namespace std;

class FileNotFound : public exception
{
	virtual const char* what() const throw()
	{
		return "File not found";
	}
};

class DummyContent : public exception
{
	virtual const char* what() const throw()
	{
		return "Dummy content in INI file";
	}
};

class ParameterNotFound : public exception
{
	virtual const char* what() const throw()
	{
		return "Can't find section or key in INI file";
	}
};

class BadIniCast : public exception
{
	virtual const char* what() const throw()
	{
		return "Can't convert value to specified type";
	}
};
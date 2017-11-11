#pragma once
#include <exception>

namespace Wav
{

	class WavError : public std::exception
	{	
	public:
		virtual const char* what() const throw()
		{
			return "Something bad happens with WAV file";
		}
	};

	class IOError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Input/output error";
		}
	};

	class BadFormat : public WavError
	{
		virtual const char* what() const throw()
		{
			return "File format is malformed";
		}
	};

	class UnsupportedFormat : public WavError
	{
		virtual const char* what() const throw()
		{
			return "File format is unsupported";
		}
	};

	class BadParams : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Parameters are malformed";
		}
	};

	class DataSizeError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Data size is bad";
		}
	};	

}
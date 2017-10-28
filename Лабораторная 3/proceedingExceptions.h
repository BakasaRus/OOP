#pragma once

namespace Wav
{

	class WavError : public std::exception
	{	
		virtual const char* what() const throw()
		{
			return "Something bad happens with WAV file";
		}
	};

	class IOError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class BadFormat : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class UnsupportedFormat : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class BadParams : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class DataSizeError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};	

}
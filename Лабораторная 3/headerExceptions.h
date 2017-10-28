#pragma once
#include "proceedingExceptions.h"

namespace Wav
{

	class HeaderRiffError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class HeaderFilesizeError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class HeaderWaveError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class HeaderFmtError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class HeaderNotPcm : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class HeaderSubchunk1Error : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class HeaderBytesRateError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class HeaderBlockAlignError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

	class HeaderSubchunk2SizeError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Test error";
		}
	};

}
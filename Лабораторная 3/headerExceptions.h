#pragma once
#include "proceedingExceptions.h"

namespace Wav
{

	class HeaderRiffError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "RIFF part is incorrect";
		}
	};

	class HeaderFilesizeError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Filesize in file and calculated filesize are different";
		}
	};

	class HeaderWaveError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "WAVE part is incorrect";
		}
	};

	class HeaderFmtError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "FMT part is incorrect";
		}
	};

	class HeaderNotPcm : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Files format except PCM are not supported";
		}
	};

	class HeaderSubchunk1Error : public WavError
	{
		virtual const char* what() const throw()
		{
			return "PCM format means that subchunk1 should contain 16 bytes";
		}
	};

	class HeaderByteRateError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Byte rate in file and calculated byte rate are different";
		}
	};

	class HeaderBlockAlignError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Block align in file and calculated block align are different";
		}
	};

	class HeaderSubchunk2SizeError : public WavError
	{
		virtual const char* what() const throw()
		{
			return "Raw data size in file and calculated raw data size are different";
		}
	};

}
#pragma once
#include <string>
#include <vector>
#include "wavHeader.h"
#include "proceedingExceptions.h"
#include "headerExceptions.h"

namespace Wav
{

	typedef std::vector<std::vector<short>> PcmData;

	class WavData
	{
	public:
		WavData() = default;
		WavData(std::string _filename) throw();
		void CreateFromFile(std::string _filename) throw();
		std::string GetDescription() const throw();
		void ConvertStereoToMono() throw(BadParams);
		void Save() const throw();
		void SaveAs(std::string _filename) const throw(BadParams, IOError);

		unsigned short GetChanCount() const throw();
		unsigned short GetSampleRate() const throw();
		unsigned short GetBitsPerSample() const throw();
		bool IsStereo() const throw();


		// Not necessary, but interesting to implement

		void ApplyReverb(double delaySeconds, double decay) throw(BadParams);
		void ChangeSampleRate(int rate) throw();
		void CutBegin(int sec) throw();
		void CutEnd(int sec) throw();

	private:
		std::string filename;
		WavHeader header;
		PcmData data;
		const size_t headerSize = sizeof(WavHeader);

		void ReadHeader() throw(BadFormat);
		bool IsHeaderCorrect(size_t fileSize) const throw(HeaderRiffError, HeaderWaveError, HeaderFmtError, 
														  HeaderNotPcm, HeaderSubchunk1Error, HeaderByteRateError, 
														  HeaderBlockAlignError, HeaderSubchunk2SizeError);
		void ReadData() throw(UnsupportedFormat, BadFormat, IOError);
	};

}
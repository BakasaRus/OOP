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
		WavData(std::string _filename);
		void CreateFromFile(std::string _filename);
		std::string GetDescription() const;
		void ConvertStereoToMono();
		void Save() const;
		void SaveAs(std::string _filename) const;

		unsigned short GetChanCount() const;
		unsigned short GetSampleRate() const;
		unsigned short GetBitsPerSample() const;
		bool IsStereo() const;


		// Not necessary, but interesting to implement

		void ApplyReverb(double delaySeconds, double decay);
		/*
		void ChangeSampleRate(int rate);
		void CutBegin(int sec);
		void CutEnd(int sec);
		*/

	private:
		std::string filename;
		WavHeader header;
		PcmData data;

		void ReadHeader();
		bool IsHeaderCorrect(size_t fileSize) const;
		void ReadData();
	};

}
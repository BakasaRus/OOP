#include <sstream>
#include <fstream>
#include "wavData.h"

namespace Wav
{
	WavData::WavData(std::string _filename)
	{
		CreateFromFile(filename);
	}

	void WavData::CreateFromFile(std::string _filename)
	{
		filename = _filename;
		ReadHeader();
		ReadData();
	}

	std::string WavData::GetDescription() const
	{
		std::stringstream Result;
		Result << "-------------------------" << std::endl
			<< " audioFormat   " << header.audioFormat << std::endl
			<< " numChannels   " << header.numChannels << std::endl
			<< " sampleRate    " << header.sampleRate << std::endl
			<< " bitsPerSample " << header.bitsPerSample << std::endl
			<< " byteRate      " << header.byteRate << std::endl
			<< " blockAlign    " << header.blockAlign << std::endl
			<< " chunkSize     " << header.chunkSize << std::endl
			<< " subchunk1Size " << header.subchunk1Size << std::endl
			<< " subchunk2Size " << header.subchunk2Size << std::endl
			<< "-------------------------" << std::endl;
		return Result.str();
	}

	void WavData::ConvertStereoToMono()
	{
		if (header.numChannels != 2) {
			throw BadParams();
			return;
		}

		int samplesCountPerChan = (header.subchunk2Size / sizeof(short)) / header.numChannels;

		// Verify that all channels have the same number of samples.
		for (size_t ch = 0; ch < header.numChannels; ch++) {
			if (data[ch].size() != (size_t)samplesCountPerChan) {
				throw BadParams();
				return;
			}
		}

		PcmData tempMono;

		tempMono.resize(1);
		std::vector<short>& mono = tempMono[0];
		mono.resize(samplesCountPerChan);

		// Mono channel is an arithmetic mean of all (two) channels.
		for (size_t i = 0; i < samplesCountPerChan; i++) {
			mono[i] = (data[0][i] + data[1][i]) / 2;
		}

		data = tempMono;

		header.numChannels = 1;
		header.subchunk2Size /= 2;
		header.chunkSize = 4 + (8 + header.subchunk1Size) + (8 + header.subchunk2Size);
	}

	void WavData::ApplyReverb(double delaySeconds, double decay)
	{
		if (header.numChannels < 1) {
			throw BadParams();
			return;
		}

		int samplesCountPerChan = (header.subchunk2Size / sizeof(short)) / header.numChannels;

		// Verify that all channels have the same number of samples.
		for (size_t ch = 0; ch < header.numChannels; ch++) {
			if (data[ch].size() != (size_t)samplesCountPerChan) {
				throw BadParams();
				return;
			}
		}

		int delaySamples = (int)(delaySeconds * header.sampleRate);


		for (size_t ch = 0; ch < header.numChannels; ch++) {
			std::vector<double> tmp;
			tmp.resize(data[ch].size());

			// Convert signal from short to float
			for (size_t i = 0; i < samplesCountPerChan; i++) {
				tmp[i] = data[ch][i];
			}

			// Add a reverb
			for (size_t i = 0; i < samplesCountPerChan - delaySamples; i++) {
				tmp[i + delaySamples] += decay * tmp[i];
			}

			// Find maximum signal's magnitude
			double max_magnitude = 0.0f;
			for (size_t i = 0; i < samplesCountPerChan - delaySamples; i++) {
				if (abs(tmp[i]) > max_magnitude) {
					max_magnitude = abs(tmp[i]);
				}
			}

			// Signed short can keep values from -32768 to +32767,
			// After reverb, usually there are values large 32000.
			// So we must scale all values back to [ -32768 ... 32768 ]
			double norm_coef = 30000.0f / max_magnitude;
			printf("max_magnitude = %.1f, coef = %.3f\n", max_magnitude, norm_coef);

			// Scale back and transform floats to shorts.
			for (size_t i = 0; i < samplesCountPerChan; i++) {
				data[ch][i] = (short)(norm_coef * tmp[i]);
			}
		}
	}

	void WavData::ReadHeader()
	{
		std::ifstream f(filename, std::ios::binary);
		f.read((char*)&header, sizeof(WavHeader));

		if (!f.good()) {
			// can't read header, because the file is too small.
			throw BadFormat();
			return;
		}

		f.seekg(0, std::ios::end);
		size_t fileSize = f.tellg();

		if (!IsHeaderCorrect(fileSize)) {
			throw BadFormat();
			return;
		}
	}

	bool WavData::IsHeaderCorrect(size_t fileSize) const
	{
		// Go to wav_header.h for details

		if (header.chunkId[0] != 0x52 ||
			header.chunkId[1] != 0x49 ||
			header.chunkId[2] != 0x46 ||
			header.chunkId[3] != 0x46)
		{
			throw HeaderRiffError();
			return false;
		}

		if (header.chunkSize != fileSize - 8) {
			throw HeaderFilesizeError();
			return false;
		}

		if (header.format[0] != 0x57 ||
			header.format[1] != 0x41 ||
			header.format[2] != 0x56 ||
			header.format[3] != 0x45)
		{
			throw HeaderWaveError();
			return false;
		}

		if (header.subchunk1Id[0] != 0x66 ||
			header.subchunk1Id[1] != 0x6d ||
			header.subchunk1Id[2] != 0x74 ||
			header.subchunk1Id[3] != 0x20)
		{
			throw HeaderFmtError();
			return false;
		}

		if (header.audioFormat != 1) {
			throw HeaderNotPcm();
			return false;
		}

		if (header.subchunk1Size != 16) {
			throw HeaderSubchunk1Error();
			return false;
		}

		if (header.byteRate != header.sampleRate * header.numChannels * header.bitsPerSample / 8) {
			throw HeaderBytesRateError();
			return false;
		}

		if (header.blockAlign != header.numChannels * header.bitsPerSample / 8) {
			throw HeaderBlockAlignError();
			return false;
		}

		if (header.subchunk2Id[0] != 0x64 ||
			header.subchunk2Id[1] != 0x61 ||
			header.subchunk2Id[2] != 0x74 ||
			header.subchunk2Id[3] != 0x61)
		{
			throw HeaderFmtError();
			return false;
		}

		if (header.subchunk2Size != fileSize - 44)
		{
			throw HeaderSubchunk2SizeError();
			return false;
		}

		return true;
	}

	void WavData::ReadData()
	{
		if (header.bitsPerSample != 16) {
			// Only 16-bit samples is supported.
			throw UnsupportedFormat();
			return;
		}

		std::ifstream f(filename, std::ios::binary);
		if (!f.good()) {
			throw BadFormat();
			return;
		}
		f.seekg(44);

		int samplesPerChan = (header.subchunk2Size / sizeof(short)) / header.numChannels;

		// 1. Reading all PCM data from file to a single vector.
		std::vector<short> allChannels;
		allChannels.resize(header.numChannels * samplesPerChan);
		f.read((char*)allChannels.data(), header.subchunk2Size);
		size_t readBytes = (int)f.tellg() - 44;
		if (readBytes != header.subchunk2Size) {
			printf("extract_data_int16() read only %zu of %u\n", readBytes, header.subchunk2Size);
			throw IOError();
			return;
		}


		// 2. Put all channels to its own vector.
		data.resize(header.numChannels);
		for (size_t ch = 0; ch < data.size(); ch++) {
			data[ch].resize(samplesPerChan);
		}
		for (int ch = 0; ch < header.numChannels; ch++) {
			std::vector<short>& chdata = data[ch];
			for (size_t i = 0; i < samplesPerChan; i++) {
				chdata[i] = allChannels[header.numChannels * i + ch];
			}
		}
	}

	void WavData::Save() const
	{
		SaveAs(filename);
	}

	void WavData::SaveAs(std::string _filename) const
	{
		if (header.numChannels < 1) {
			throw BadParams();
			return;
		}

		int samplesCountPerChan = (header.subchunk2Size / sizeof(short)) / header.numChannels;

		// Verify that all channels have the same number of samples.
		for (size_t ch = 0; ch < header.numChannels; ch++) {
			if (data[ch].size() != (size_t)samplesCountPerChan) {
				throw BadParams();
				return;
			}
		}
		std::vector<short> allChannels;
		allChannels.resize(header.numChannels * samplesCountPerChan);

		for (int ch = 0; ch < header.numChannels; ch++) {
			const std::vector<short>& chdata = data[ch];
			for (size_t i = 0; i < samplesCountPerChan; i++) {
				allChannels[header.numChannels * i + ch] = chdata[i];
			}
		}

		std::ofstream f(_filename, std::ios::binary);
		f.write((char*)&header, sizeof(WavHeader));
		f.write((char*)allChannels.data(), sizeof(short) * allChannels.size());
		if (!f.good()) {
			throw IOError();
			return;
		}
	}

	unsigned short WavData::GetChanCount() const
	{
		return header.numChannels;
	}

	unsigned short WavData::GetSampleRate() const
	{
		return header.sampleRate;
	}

	unsigned short WavData::GetBitsPerSample() const
	{
		return header.bitsPerSample;
	}

	bool WavData::IsStereo() const
	{
		return header.numChannels == 2;
	}
}
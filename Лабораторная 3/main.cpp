#include <iostream>
#include "wavData.h"

int main()
{
	try 
	{
		Wav::WavData wav1;
		wav1.CreateFromFile("0.wav");
		std::cout << wav1.GetDescription() << std::endl;
		/*
		wav1.ConvertStereoToMono();
		wav1.ApplyReverb(1.5, 0.6f);
		std::cout << wav1.GetDescription() << std::endl;
		wav1.SaveAs("0-mono.wav");
		*/

		/*
		Wav::WavData wav2("secret-speech.wav");
		std::cout << wav2.GetDescription() << std::endl;
		int sampleRate = wav2.GetSampleRate();
		int chanCount = wav2.GetChanCount();
		bool isStereo = wav2.IsStereo();
		wav2.ChangeSampleRate(22050);

		// Cut first 10 seconds and last 20 seconds
		wav2.CutBegin(10.0);
		wav2.CutEnd(20.0);

		wav2.Save(); // Overwrite the original file.

		*/

	}
	catch (/*Wav::Test& we*/ std::exception& e) 
	{
		std::cerr << e.what();
	}

	system("pause");
	return 0;
}
#include <iostream>
#include "wavData.h"

int main()
{
	try 
	{
		Wav::WavData wav1;
		wav1.CreateFromFile("molly.wav");
		std::cout << wav1.GetDescription() << std::endl;
		wav1.ConvertStereoToMono();
		wav1.ApplyReverb(1.5, 0.6f);
		std::cout << wav1.GetDescription() << std::endl;
		wav1.SaveAs("0-mono.wav");
		throw Wav::DataSizeError();
	}
	catch (Wav::WavError& we) 
	{
		std::cerr << we.what() << std::endl;
	}

	return 0;
}
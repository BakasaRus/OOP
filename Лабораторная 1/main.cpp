#include "iniParser.h"

int main()
{
	try
	{
		IniParser Ini("input_files/lab01_input_correct.ini");
		Ini.Dump();

		Ini.SetInt("Important", "Meaning", 42);
		Ini.SetDouble("Important", "Pi", 3.14159265);
		Ini.SetString("Important", "Code", "BlaBlaBla_1622");

		Ini.SetInt("Section_1_Ints", "Zero", 59);

		std::cout << Ini.GetInt("Section_1_Ints", "Zero") << std::endl;

		std::ofstream ClearIni("Test.ini");

		Ini.Dump(ClearIni);
	}
	catch (DummyContent& e)
	{
		std::cerr << e.what();
	}
	
	return 0;
}
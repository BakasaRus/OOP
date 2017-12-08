#include "iniParser.h"

int main()
{
	IniParser ini("input_files/lab01_input_correct_self.ini");
	ini.Dump();

	ini.SetValue<int>("Important", "Meaning", 42);
	ini.SetValue<double>("Important", "Pi", 3.14159265);
	ini.SetValue<std::string>("Important", "Code", "BlaBlaBla_1622");

	ini.SetValue<int>("Section_1_Ints", "Zero", 59);

	int temp = ini.GetValue<int>("Section_1_Ints", "Zero");
	std::cout << temp << std::endl;

	std::ofstream clearIni("Test.ini");

	ini.Dump(clearIni);

	return 0;
}
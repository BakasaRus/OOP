#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>  

#include "iniParser.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	IniParser Ini("input_files/lab01_input_correct.ini");
	Ini.Dump();

	Ini.SetInt("Important", "Meaning", 42);
	Ini.SetDouble("Important", "Pi", 3.14159265);
	Ini.SetString("Important", "Code", "BlaBlaBla_1622");

	Ini.SetInt("Section_1_Ints", "Zero", 59);

	cout << Ini.GetInt("Section_1_Ints", "Zero") << endl;

	ofstream ClearIni("Test.ini");

	Ini.Dump(ClearIni);

	system("pause");
	return 0;
}
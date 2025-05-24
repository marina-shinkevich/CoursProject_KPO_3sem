#pragma once
#include <fstream>
#include "Parm.h"
#include "Error.h"
namespace Out //работа с протоколом
{
	struct OUT //протокол
	{
		wchar_t outfile[PARM_MAX_SIZE]; //имя файла протокола
		std::ofstream* stream;			//входной поток протокола
	};

	static const OUT INITOUT = { L"",NULL };		//структура для начальной инициализации LOG
	OUT getout(wchar_t outfile[]);					// сформировать структуру LOG
	void WriteLine(OUT out, const char* c, ...); //вывести в протокол конкатенацию строк
	void WriteLine(OUT out, wchar_t* c, ...); //вывести в протокол конкатенацию строк
	void Close(OUT out); //закрыть протокол
}

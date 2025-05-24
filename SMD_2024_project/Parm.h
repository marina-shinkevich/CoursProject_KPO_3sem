#pragma once
#include "tchar.h"
#define PARM_IN L"-in:"   //ключ для файла исходного файла
#define PARM_OUT L"-out:" //ключ для файла объектного файлы
#define PARM_LOG L"-log:" //ключ для файла журнала

#define PARM_MAX_SIZE 300 //максимальная длина строки параметров
#define PARM_OUT_DEFAULT_EXT L".asm" //Расширение файла объектного кода по умоланию
#define PARM_LOG_DEFAULT_EXT L".log" //расширение файла протокола по умолчанию

namespace Parm // обработка входных параметров
{
	struct PARM //входные параметры
	{
		wchar_t in[PARM_MAX_SIZE];// -in:  имя файла исходного кода
		wchar_t out[PARM_MAX_SIZE];// -out: имя файла объектного кода
		wchar_t log[PARM_MAX_SIZE];// -log: имя файла протокола
		bool showMfst;
		bool showTables;
		bool showITables;
	};

	PARM getparm(int argc, _TCHAR* argv[]);
}
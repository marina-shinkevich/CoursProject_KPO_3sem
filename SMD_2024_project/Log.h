#pragma once
#include <fstream>
#include "In.h"
#include "Parm.h"
#include "LexAnalysis.h"
#include "Error.h"

namespace Log //работа с протоколом
{
	struct LOG //протокол
	{
		wchar_t logfile[PARM_MAX_SIZE]; //им€ файла протокола
		std::ofstream* stream;			//входной поток протокола
	};

	static const LOG INITLOG = { L"",NULL };		//структура дл€ начальной инициализации LOG
	LOG getlog(wchar_t logfile[]);					// сформировать структуру LOG
	void WriteLine(LOG log, const char* c, ...); //вывести в протокол конкатенацию строк
	void WriteLine(LOG log, wchar_t* c, ...); //вывести в протокол конкатенацию строк
	void WriteLog(LOG log); //вывести в протокол заголовок
	void WriteParm(LOG log, Parm::PARM parm); //вывести в протокол информацию о входных параметрах
	void WriteIn(LOG log, In::IN in); //вывести в протокол информацию о входном потоке
	void WriteError(LOG log, Error::ERROR error); //вывести в протокол информацию об ошибке
	void LogTables(LOG log, LEX::LEX t); //¬ывод таблиц лексем
	void LogIDTables(LOG log, LEX::LEX t); //¬ывод таблиц идентификаторов
	void Close(LOG log); //закрыть протокол
}
#include "Parm.h"
#include "Error.h"
#include "Out.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include "time.h"

namespace Out //работа с протоколом
{
	OUT getout(wchar_t outfile[])
	{
		// инициализирует структуру OUT и открывает файл для записи
		OUT out;
		out.stream = new std::ofstream;
		out.stream->open(outfile);
		if (!out.stream->is_open())
			throw ERROR_THROW(112);
		wcscpy_s(out.outfile, outfile);
		return out;
	}
	void WriteLine(OUT out, const char* c, ...)
	{
		//принимает неограниченное колич. чтрок
		const char** temp = &c;
		while (*temp != "")
		{
			*(out.stream) << *temp;//записываем строку
			temp++;
		}
		*(out.stream) << *temp;
	}
	void WriteLine(OUT out, wchar_t* c, ...)
	{
		size_t   i; //для хранения количества сконвертированных символов
		wchar_t** temp = &c;
		while (*temp != L"")
		{
			char* ttmp = new char[sizeof(*temp)]; //временный буфер для строки char
			wcstombs_s(&i, ttmp, sizeof ttmp + 1, *temp, sizeof ttmp + 1); //конвертация
			*(out.stream) << ttmp;
			temp++;
		}
	}
	void Close(OUT out)
	{
		out.stream->close();
	}
}
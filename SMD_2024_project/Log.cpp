#include "Parm.h"
#include "Error.h"
#include "Log.h"
#include "In.h"
#include <fstream>
#pragma warning (disable : 4996)
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <cstdarg>
#include "time.h"



namespace Log //работа с протоколом
{
	LOG getlog(wchar_t logfile[])
	{ //создание и открытие файла для записи
		LOG out;
		out.stream = new std::ofstream;
		out.stream->open(logfile);//открываем
		if (!out.stream->is_open())
			throw ERROR_THROW(112);
		wcscpy_s(out.logfile, logfile);
		return out;
	}
	void WriteLine(LOG log, const char* c, ...)
	{
		const char** temp = &c;
		while (*temp != "")
		{
			*(log.stream) << *temp;//запись текущей строки в лог-файл
			temp++;
		}
		*(log.stream) << std::endl;
	}
	void WriteLine(LOG log, wchar_t* c, ...)
	{
		size_t   i;//для хранения длины строки
		wchar_t** temp = &c;
		while (*temp != L"")
		{
			//преобразование широкой строки в обычную строку 
			char* ttmp = new char[sizeof(*temp)];
			wcstombs_s(&i, ttmp, sizeof ttmp + 1, *temp, sizeof ttmp + 1);
			*(log.stream) << ttmp;
			temp++;
		}
		*(log.stream) << std::endl;
	}

	void WriteLog(LOG log)//запись даты и времени
	{
		*(log.stream) << "................... Протокол ...................\n\tДата: ";
		time_t time_sec = time(NULL);
		tm time_date;
		localtime_s(&time_date, &time_sec);
		(time_date.tm_mday < 10) ? (*(log.stream) << '0' << time_date.tm_mday << '.') : (*(log.stream) << time_date.tm_mday << '.');
		(time_date.tm_mon + 1 > 9) ? (*(log.stream) << time_date.tm_mon + 1 << '.') : (*(log.stream) << '0' << time_date.tm_mon + 1 << '.');
		*(log.stream) << time_date.tm_year + 1900 << ' ';
		(time_date.tm_hour > 9) ? (*(log.stream) << time_date.tm_hour << ':') : (*(log.stream) << '0' << time_date.tm_hour << ':');
		(time_date.tm_min > 9) ? (*(log.stream) << time_date.tm_min << ':') : (*(log.stream) << '0' << time_date.tm_min << ':');
		(time_date.tm_sec > 9) ? (*(log.stream) << time_date.tm_sec << ':') : (*(log.stream) << '0' << time_date.tm_sec << ':');
		*(log.stream) << "...................\n";

	}
	void WriteParm(LOG log, Parm::PARM parm)
	{
		//вывод параметров и имена файлов для этих параметров


		*log.stream << "................... Параметры...................\n";

		int i = 0;
		char ch;
		*log.stream << "-log: ";
		while ((ch = wctob(parm.log[i++])) != EOF)
			*log.stream << ch;
		i = 0;
		*log.stream << "\n-out: ";
		while ((ch = wctob(parm.out[i++])) != EOF)
			*log.stream << ch;
		i = 0;
		*log.stream << "\n-in: ";
		while ((ch = wctob(parm.in[i++])) != EOF)
			*log.stream << ch;
		*(log.stream) << std::endl;
	}
	void WriteIn(LOG log, In::IN in)
	{
		*(log.stream) << "................... Исходные данные ...................\n";
		*(log.stream) << "Количество символов: " << in.size;
		*(log.stream) << "\nКоличество лексем: " << in.lexems.size();
		*(log.stream) << "\nКоличество строк: " << in.lines << std::endl;
	}
	void WriteError(LOG log, Error::ERROR error)
	{
		//вывод ошибки в консоль
		std::cout << "Ошибка " << error.id << ": " << error.message;
		if (error.inext.col != -1 && error.inext.line != -1)
			std::cout << ", строка " << error.inext.line << ", столбец " << error.inext.col << std::endl;

		if (log.stream != NULL)
		{
			//вывод ошибки в файл
			*log.stream << "\nОшибка " << error.id << ": " << error.message;
			if (error.inext.col != -1 && error.inext.line != -1)
				*(log.stream) << ", строка " << error.inext.line << ", столбец " << error.inext.col << std::endl;
			else
				*(log.stream) << std::endl;
		}


	}

	void LogTables(LOG log, LEX::LEX t) {

		*log.stream << std::setw(9 + 15 + 15 + 15 + 25) << std::left << "......................//////Таблица лексем//////........................................." << std::endl;
		*log.stream << std::setw(9) << std::left << "Позиция"
			<< std::setw(15) << std::left << "Номер строки"
			<< std::setw(15) << std::left << "Номер столбца"
			<< std::setw(15) << std::left << "Лексема"
			<< std::setw(25) << std::left << "Индекс таблицы идентификаторов"
			<< std::endl;
		*log.stream << std::setw(2) << std::left << ".............................................................................................."
			<< std::endl;

		for (int i = 0; i < t.lextable.size; i++)
		{
			LT::Entry a = LT::GetEntry(t.lextable, i);
			*log.stream << std::setw(9) << std::left << i
				<< std::setw(15) << std::left << a.sn // Номер строки
				<< std::setw(15) << std::left << a.cn // Номер столбца
				<< std::setw(15) << std::left << a.lexema // Лексема
				<< std::setw(25) << std::left << a.idxTI // Индекс в таблице идентификаторов
				<< std::endl;
		}
	}
	void LogIDTables(LOG log, LEX::LEX t) {

		*log.stream << std::setw(9 + 16 + 15 + 15 + 15) << std::left << "\n\n...................////////Таблица идентификаторов////////..................." << std::endl;
		*log.stream << std::setw(9) << std::left << "Позиция"
			<< std::setw(25) << std::left << "Имя"
			<< std::setw(25) << std::left << "Вид"
			<< std::setw(25) << std::left << "Тип данных"
			<< std::setw(20) << std::left << "Значение"
			

			<< std::endl;
		*log.stream << std::setw(2) << std::left << ".............................................................................................."
			<< std::endl;


		//// Перебор таблицы идентификаторов
		for (int i = 0; i < t.idtable.size; i++)
		{

			std::string type, datatype, data;
			IT::Entry b = IT::GetEntry(t.idtable, i);
			char* buff;
			// // Определение типа идентификатора
			switch (b.idtype) {
			case IT::V:
				type = "Переменная";
				break;
			case IT::F:
				type = "Функция";
				break;
			case IT::L:
				type = "Литерал";
				break;
			case IT::P:
				type = "Параметр";
				break;
			}
			//тип данных идентификатора
			switch (b.iddatatype) {
			case IT::INT:
				datatype = "Число";
				buff = new char[10];
				_itoa(b.value.vint, buff, 10);//преобразует целое число в строку
				data = buff;
				delete[]buff;
				break;
			case IT::STR:
				datatype = "Строка";
				if (b.value.vstr.len == 0)
					data = "Пустая строка";
				else
					data = b.value.vstr.str;
				break;

			case IT::CHR:
				datatype = "Символ";
				data = b.value.vchar;
				break;

			}


			*log.stream << std::setw(9) << std::left << i
				<< std::setw(25) << std::left << b.id
				<< std::setw(25) << std::left << type
				<< std::setw(25) << std::left << datatype
				<< std::setw(20) << std::left << data
				<< std::endl;
		}
	}

	void Close(LOG log)
	{
		log.stream->close();
	}
}
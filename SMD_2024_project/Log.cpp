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



namespace Log //������ � ����������
{
	LOG getlog(wchar_t logfile[])
	{ //�������� � �������� ����� ��� ������
		LOG out;
		out.stream = new std::ofstream;
		out.stream->open(logfile);//���������
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
			*(log.stream) << *temp;//������ ������� ������ � ���-����
			temp++;
		}
		*(log.stream) << std::endl;
	}
	void WriteLine(LOG log, wchar_t* c, ...)
	{
		size_t   i;//��� �������� ����� ������
		wchar_t** temp = &c;
		while (*temp != L"")
		{
			//�������������� ������� ������ � ������� ������ 
			char* ttmp = new char[sizeof(*temp)];
			wcstombs_s(&i, ttmp, sizeof ttmp + 1, *temp, sizeof ttmp + 1);
			*(log.stream) << ttmp;
			temp++;
		}
		*(log.stream) << std::endl;
	}

	void WriteLog(LOG log)//������ ���� � �������
	{
		*(log.stream) << "................... �������� ...................\n\t����: ";
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
		//����� ���������� � ����� ������ ��� ���� ����������


		*log.stream << "................... ���������...................\n";

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
		*(log.stream) << "................... �������� ������ ...................\n";
		*(log.stream) << "���������� ��������: " << in.size;
		*(log.stream) << "\n���������� ������: " << in.lexems.size();
		*(log.stream) << "\n���������� �����: " << in.lines << std::endl;
	}
	void WriteError(LOG log, Error::ERROR error)
	{
		//����� ������ � �������
		std::cout << "������ " << error.id << ": " << error.message;
		if (error.inext.col != -1 && error.inext.line != -1)
			std::cout << ", ������ " << error.inext.line << ", ������� " << error.inext.col << std::endl;

		if (log.stream != NULL)
		{
			//����� ������ � ����
			*log.stream << "\n������ " << error.id << ": " << error.message;
			if (error.inext.col != -1 && error.inext.line != -1)
				*(log.stream) << ", ������ " << error.inext.line << ", ������� " << error.inext.col << std::endl;
			else
				*(log.stream) << std::endl;
		}


	}

	void LogTables(LOG log, LEX::LEX t) {

		*log.stream << std::setw(9 + 15 + 15 + 15 + 25) << std::left << "......................//////������� ������//////........................................." << std::endl;
		*log.stream << std::setw(9) << std::left << "�������"
			<< std::setw(15) << std::left << "����� ������"
			<< std::setw(15) << std::left << "����� �������"
			<< std::setw(15) << std::left << "�������"
			<< std::setw(25) << std::left << "������ ������� ���������������"
			<< std::endl;
		*log.stream << std::setw(2) << std::left << ".............................................................................................."
			<< std::endl;

		for (int i = 0; i < t.lextable.size; i++)
		{
			LT::Entry a = LT::GetEntry(t.lextable, i);
			*log.stream << std::setw(9) << std::left << i
				<< std::setw(15) << std::left << a.sn // ����� ������
				<< std::setw(15) << std::left << a.cn // ����� �������
				<< std::setw(15) << std::left << a.lexema // �������
				<< std::setw(25) << std::left << a.idxTI // ������ � ������� ���������������
				<< std::endl;
		}
	}
	void LogIDTables(LOG log, LEX::LEX t) {

		*log.stream << std::setw(9 + 16 + 15 + 15 + 15) << std::left << "\n\n...................////////������� ���������������////////..................." << std::endl;
		*log.stream << std::setw(9) << std::left << "�������"
			<< std::setw(25) << std::left << "���"
			<< std::setw(25) << std::left << "���"
			<< std::setw(25) << std::left << "��� ������"
			<< std::setw(20) << std::left << "��������"
			

			<< std::endl;
		*log.stream << std::setw(2) << std::left << ".............................................................................................."
			<< std::endl;


		//// ������� ������� ���������������
		for (int i = 0; i < t.idtable.size; i++)
		{

			std::string type, datatype, data;
			IT::Entry b = IT::GetEntry(t.idtable, i);
			char* buff;
			// // ����������� ���� ��������������
			switch (b.idtype) {
			case IT::V:
				type = "����������";
				break;
			case IT::F:
				type = "�������";
				break;
			case IT::L:
				type = "�������";
				break;
			case IT::P:
				type = "��������";
				break;
			}
			//��� ������ ��������������
			switch (b.iddatatype) {
			case IT::INT:
				datatype = "�����";
				buff = new char[10];
				_itoa(b.value.vint, buff, 10);//����������� ����� ����� � ������
				data = buff;
				delete[]buff;
				break;
			case IT::STR:
				datatype = "������";
				if (b.value.vstr.len == 0)
					data = "������ ������";
				else
					data = b.value.vstr.str;
				break;

			case IT::CHR:
				datatype = "������";
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
#include "LT.h"
#include "Error.h"
#include <fstream>

#include <iostream>
#include <stdlib.h>
#include <cstdarg>

namespace LT
{
	LexTable Create(int a)
	{
		if (a > LT_MAXSIZE)
		{
			throw ERROR_THROW(131);
		}
		LexTable out;
		out.maxsize = a;
		out.size = 0;
		out.table = new Entry[a];
		return out;
	}

	void Add(	//�������� ������ � ������� ������
		LexTable& lextable,		//��������� ������� ������
		Entry entry	//������ ������� ������
	)
	{
		if (lextable.size + 1 > lextable.maxsize)
			throw ERROR_THROW(132);
		lextable.table[lextable.size++] = entry;
	}

	Entry GetEntry(	//�������� ������ ������� ������
		LexTable& lextable, //���������
		int n //����� ���������� ������
	)
	{
		if (lextable.size > lextable.maxsize)
			throw ERROR_THROW(135);
		return lextable.table[n];
	};


	void Delete(LexTable& lextable) {
		delete& lextable;
	};
}
#pragma once
#include "LT.h"
#define IN_MAX_LEN_TEXT 1024*1024 //���� ������ ��������� ���� 1 ��
#define IN_CODE_ENDL '\n' //������ ����� ������
#define DIVIDE ';' //�����������
#define MAX_LEXEM_LENGTH 25 //������������ ����� �������
#include <list>
//������� �������� ������� ����������, ������ = ���(Windows-1251) �������//�������� IN::F - ����������� ������, IN::T - ����������� ������, IN::I - ������������(�� �������)
#define IN_CODE_TABLE {\
IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::S, IN::N, IN::F, IN::F, IN::T,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::S, IN::F, IN::D, IN::O, IN::F, IN::L, IN::L, IN::C, IN::L, IN::L,\
IN::L, IN::L, IN::L, IN::L, IN::F, IN::L, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,\
IN::T, IN::T, IN::L, IN::L, IN::L, IN::L, IN::L, IN::F, IN::F, IN::T, IN::T, IN::T, IN::T, IN::T,\
IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,\
IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::L, IN::L, IN::L, IN::L, IN::T, IN::F, IN::T,\
IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,\
IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::L, IN::F, IN::L,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
 IN::F, IN::F, IN::F,\
}


namespace In
{
	struct lexem
	{
		int line;
		int col;
		unsigned char* lexem;
	};

	struct IN //�������� ������
	{
		//D = double comma O - octotorp, comment
		enum { T = 1024, F = 2048, L = 256, C = 128, N = 64, D = 32, O = 16, S = 0 }; //S - space t - ���������� ������ F- ������������, C-comma, L - Single lexem
		int size;		//������ ��������� ����
		int lines;		//���������� �����
		int ignor;		//���-�� ����������������� ��������
		

		std::list<lexem> lexems;//�������� ��� (Windows - 1251)

	};

	IN getin(wchar_t infile[]);  //������ � ��������� ������� �����
}
#pragma once
#include "LexAnalysis.h"
#include "Out.h"

namespace CG {
	void Generate(LEX::LEX t, Out::OUT o);						//�������� �����-�� � Generator
	void Head(std::ofstream* stream, LEX::LEX t);				//���� ������������ ��������� , ������ .....
	void Constants(std::ofstream* stream, LEX::LEX t);			//���� ��������
	void Data(std::ofstream* stream, LEX::LEX t);				//���� ������
	void WriteFunctions(std::ofstream* stream, LEX::LEX t);		//���� �������
	void Code(std::ofstream* stream, LEX::LEX t); 				//���� ����
}
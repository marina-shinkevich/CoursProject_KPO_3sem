#pragma once
#include "LexAnalysis.h"
#include "Out.h"

namespace CG {
	void Generate(LEX::LEX t, Out::OUT o);						//передача парам-ов в Generator
	void Head(std::ofstream* stream, LEX::LEX t);				//блок перечесления библиотек , модель .....
	void Constants(std::ofstream* stream, LEX::LEX t);			//блок констант
	void Data(std::ofstream* stream, LEX::LEX t);				//блок данных
	void WriteFunctions(std::ofstream* stream, LEX::LEX t);		//блок функций
	void Code(std::ofstream* stream, LEX::LEX t); 				//блок кода
}
#pragma once
#include "LT.h"
#define IN_MAX_LEN_TEXT 1024*1024 //макс размер исходного кода 1 мб
#define IN_CODE_ENDL '\n' //символ конца строки
#define DIVIDE ';' //разделитель
#define MAX_LEXEM_LENGTH 25 //максимальная длина лексемы
#include <list>
//таблица проверки входной информации, индекс = код(Windows-1251) символа//значения IN::F - запрещенный символ, IN::T - разрешенный символ, IN::I - игнорировать(не вводить)
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

	struct IN //исходные данные
	{
		//D = double comma O - octotorp, comment
		enum { T = 1024, F = 2048, L = 256, C = 128, N = 64, D = 32, O = 16, S = 0 }; //S - space t - допустимый символ F- недопустимый, C-comma, L - Single lexem
		int size;		//размер исходного кода
		int lines;		//количество строк
		int ignor;		//кол-во проигнорированных символов
		

		std::list<lexem> lexems;//Исходный код (Windows - 1251)

	};

	IN getin(wchar_t infile[]);  //ввести и проверить входной поток
}
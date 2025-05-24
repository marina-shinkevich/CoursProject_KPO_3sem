#include "CodeGenerator.h"
#include <stack>

namespace CG {
	void InvokeExpressions(std::ofstream* stream, LEX::LEX t, int start, int end, int salt = 0)
	{
		for (int i = start; i < end; i++)
		{
			//разобрать строку выражения
			if (t.lextable.table[i].lexema == LEX_EQUAL && t.lextable.table[i].data == '=')
			{
				*stream << '\n' << "; String #" << t.lextable.table[i].sn << " :"; // вывод строки

				for (int j = -1; t.lextable.table[i + j].lexema != LEX_SEMICOLON; j++) // 
				{
					*stream << t.lextable.table[i + j].lexema;
				}
				*stream << '\n';
				int pos = 0;																		// позиция в строке
				bool isArguments = false;															// флаг аргументов
				IT::Entry* func, * save = nullptr;													// функция, сохранение
				IT::Entry* recipent = &t.idtable.table[t.lextable.table[i - 1].idxTI];
				while (true)
				{
					pos++;
					if (t.lextable.table[i + pos].lexema == LEX_SEMICOLON || t.lextable.table[i + pos].lexema == '!') // 
					{
						if (recipent->iddatatype != IT::CHR)										// если не char
						{
							*stream << "pop " << recipent->id << '\n';								// то выталкиваем в переменную
						}
						else
						{
							*stream << "pop eax\nmov " << recipent->id << ", al" << '\n';			// иначе в eax
						}
						break;
					}
					else if (t.lextable.table[i + pos].lexema == LEX_LITERAL || t.lextable.table[i + pos].lexema == LEX_ID)
					{
						if (t.idtable.table[t.lextable.table[i + pos].idxTI].idtype != IT::F)
						{
							switch (t.idtable.table[t.lextable.table[i + 1].idxTI].iddatatype)
							{
							case (IT::CHR): {
								*stream << "movzx eax, " << t.idtable.table[t.lextable.table[i + pos].idxTI].id << '\n';
								*stream << "push eax " << '\n';
								break;
							}
							case (IT::STR): {
								if (t.idtable.table[t.lextable.table[i + pos].idxTI].idtype == IT::L)
									*stream << "push offset " << t.idtable.table[t.lextable.table[i + pos].idxTI].id << '\n';
								else
									*stream << "push " << t.idtable.table[t.lextable.table[i + pos].idxTI].id << '\n';
								break;
							}
							case (IT::INT): {
								*stream << "push " << t.idtable.table[t.lextable.table[i + pos].idxTI].id << '\n';
								break;
							}
							}
							save = &t.idtable.table[t.lextable.table[i + pos].idxTI];
						}
						else
						{
							isArguments = true;
							if (t.idtable.table[t.lextable.table[i + pos].idxTI].isExternal == false)
								*stream << "invoke " << "$" << t.idtable.table[t.lextable.table[i + pos].idxTI].id;
							else
								*stream << "invoke " << t.idtable.table[t.lextable.table[i + pos].idxTI].id;

							pos++;
							while (t.lextable.table[i + pos].lexema != '@') {
								*stream << ", " << t.idtable.table[t.lextable.table[i + pos].idxTI].id;
								pos++;
							}
							*stream << '\n';
							*stream << "push eax ;результат функции";
							*stream << '\n';
						}
					}
					else if (t.lextable.table[i + pos].lexema == LEX_OPERATOR) {
						*stream << "pop ebx" << '\n';
						*stream << "pop eax" << '\n';
						switch (t.lextable.table[i + pos].data)
						{
						case '+': {
							*stream << "add eax, ebx " << '\n';
							break;
						}
						case '-': {
							*stream << "sub eax, ebx" << '\n';
							break;
						}
						case '*': {
							*stream << "mul ebx" << '\n';
							break;
						}
						case ':': {
							*stream << "push edx ; сохраняем данные регистра edx" << '\n';
							*stream << "mov edx, 0" << '\n';
							*stream << "TEST  EBX, EBX" << '\n';
							*stream << "JZ    div_by_0" << '\n';
							*stream << "div ebx" << '\n';
							*stream << "pop edx" << '\n';
							break;
						}
						case '%': {
							*stream << "push edx ; сохраняем данные регистра edx" << '\n';
							*stream << "mov edx, 0" << '\n';
							*stream << "TEST  EBX, EBX" << '\n';
							*stream << "JZ    div_by_0" << '\n';
							*stream << "div ebx" << '\n';
							*stream << "mov eax, edx" << '\n';
							*stream << "pop edx" << '\n';
							break;
						}

						case '/': {
							*stream << "push ecx ; сохраняем данные регистра ecx" << '\n';
							*stream << "mov ecx, ebx" << '\n';
							*stream << "SHL eax, cl" << '\n';
							*stream << "pop ecx" << '\n';
							break;
						}
						case '\\': {
							*stream << "push ecx ; сохраняем данные регистра ecx" << '\n';
							*stream << "mov ecx, ebx" << '\n';
							*stream << "SHR eax, cl" << '\n';
							*stream << "pop ecx" << '\n';
							break;
						}

						}
						*stream << "push eax" << '\n';
					}
				}
			}
			//разобрать ретурн
			else if (t.lextable.table[i].lexema == LEX_RETURN) {
				if (t.idtable.table[t.lextable.table[i + 1].idxTI].iddatatype == IT::CHR)
				{
					*stream << "\nmovzx eax, " << t.idtable.table[t.lextable.table[i + 1].idxTI].id << '\n';
				}
				else
				{
					*stream << "\nmov eax, " << t.idtable.table[t.lextable.table[i + 1].idxTI].id << '\n';
				}
			}
			//разобрать вывод
			else if (t.lextable.table[i].lexema == LEX_PRINT) {
				//pos++;
				switch (t.idtable.table[t.lextable.table[i + 1].idxTI].iddatatype)
				{
				case (IT::CHR): {
					*stream << "push eax\n";
					*stream << "movzx eax, " << t.idtable.table[t.lextable.table[i + 1].idxTI].id << '\n';
					*stream << "push eax\n";
					*stream << "CALL outputSymb" << '\n';
					*stream << "pop eax\n" << '\n';
					break;
				}
				case (IT::STR): {
					*stream << "\npush ";
					if (t.idtable.table[t.lextable.table[i + 1].idxTI].idtype == IT::L)
						*stream << "offset " << t.idtable.table[t.lextable.table[i + 1].idxTI].id << '\n';
					else
						*stream << t.idtable.table[t.lextable.table[i + 1].idxTI].id << '\n';

					*stream << "CALL outputLine" << '\n';
					break;
				}
				case (IT::INT): {
					*stream << "\npush ";
					*stream << t.idtable.table[t.lextable.table[i + 1].idxTI].id << '\n';
					*stream << "CALL outputIntNum" << '\n';
					break;
				}
				}
			}
			//TODO: разобрать цикл
			else if (t.lextable.table[i].lexema == LEX_UNTIL) {
				int pos = 1;
				int st;
				int whileNumber = 0;
				int leftSquareCount = 0;
				bool getExpressionParams = true;
				IT::Entry* firstId = nullptr, * secondId = nullptr;
				std::string oper;
				while (true) {

					switch (t.lextable.table[i + pos].lexema)
					{
					case LEX_LEFTHESIS: {
						if (getExpressionParams)
						{
							whileNumber = i + salt;
							*stream << "\nWhile" << whileNumber << "Start: " << '\n';

						}
						break;
					}
					case LEX_RIGHTHESIS: {
						if (getExpressionParams)
						{
							st = i + 1;
						}
						break;
					}
					case LEX_ID: {
						if (getExpressionParams)
						{
							if (firstId == nullptr)
								firstId = &t.idtable.table[t.lextable.table[i + pos].idxTI];
							else
								secondId = &t.idtable.table[t.lextable.table[i + pos].idxTI];

						}break;
					}
					case LEX_LITERAL: {
						if (getExpressionParams)
						{
							if (firstId == nullptr)
								firstId = &t.idtable.table[t.lextable.table[i + pos].idxTI];
							else
								secondId = &t.idtable.table[t.lextable.table[i + pos].idxTI];

						}break;
					}
									// TODO: разобрать операторы
					case LEX_BOOL_OPERATOR: {
						if (getExpressionParams) // если идет разбор выражения
						{
							switch (t.lextable.table[i + pos].data)
							{
							
						

							case '<': {
								oper = "ja";
								break;
							}
							case '>': {
								oper = "jl";
								break;
							}
							}

						}break;
					}

					case LEX_LEFT_SQUAREBRACE: {
						if (getExpressionParams)
						{
							if (firstId->iddatatype == IT::CHR)
								*stream << "movzx eax, " << firstId->id << '\n';
							else
								*stream << "mov eax, " << firstId->id << '\n';
							if (secondId->iddatatype == IT::CHR)
								*stream << "movzx ebx, " << secondId->id << '\n';
							else
								*stream << "mov ebx, " << secondId->id << '\n';
							*stream << "cmp eax, ebx" << '\n';
							*stream << oper << " While" << whileNumber << "End" << '\n';
						}
						getExpressionParams = false;
						leftSquareCount++;

						break;
					}

					case LEX_RIGHT_SQUAREBRACE: {
						leftSquareCount--;
						break;
					}
					}
					//закончился наш цикл
					if (leftSquareCount == 0 && !getExpressionParams)
					{

						InvokeExpressions(stream, t, st, i + pos, salt + 1);
						*stream << "jmp While" << whileNumber << "Start" << '\n';
						*stream << "While" << whileNumber << "End: " << '\n';
						i += pos;
						break;
					}
					pos++;

				}
			}
			else if (t.lextable.table[i].lexema == LEX_IF)
			{
				int pos = 1;
				int st;
				int whileNumber = 0;
				int leftSquareCount = 0;
				bool getExpressionParams = true;
				IT::Entry* firstId = nullptr, * secondId = nullptr;
				std::string oper;
				while (true) {

					switch (t.lextable.table[i + pos].lexema)
					{
					case LEX_LEFTHESIS: {
						if (getExpressionParams)
						{
							whileNumber = i + salt;
							*stream << "\nIf" << whileNumber << "Start: " << '\n';

						}
						break;
					}
					case LEX_RIGHTHESIS: {
						if (getExpressionParams)
						{
							st = i + 1;
						}
						break;
					}
					case LEX_ID: {
						if (getExpressionParams)
						{
							if (firstId == nullptr)
								firstId = &t.idtable.table[t.lextable.table[i + pos].idxTI];
							else
								secondId = &t.idtable.table[t.lextable.table[i + pos].idxTI];

						}break;
					}
					case LEX_LITERAL: {
						if (getExpressionParams)
						{
							if (firstId == nullptr)
								firstId = &t.idtable.table[t.lextable.table[i + pos].idxTI];
							else
								secondId = &t.idtable.table[t.lextable.table[i + pos].idxTI];

						}break;
					}

					case LEX_BOOL_OPERATOR: {
						if (getExpressionParams)
						{
							switch (t.lextable.table[i + pos].data)
							{
					

							case '<': {
								oper = "ja";//первый больше второго
								break;
							}

							case '>': {
								oper = "jl";//первый меньше второго
								break;
							}
							}

						}break;
					}

					case LEX_LEFT_SQUAREBRACE: {
						if (getExpressionParams)
						{
							if (firstId->iddatatype == IT::CHR)
								*stream << "movzx eax, " << firstId->id << '\n';
							else
								*stream << "mov eax, " << firstId->id << '\n';
							if (secondId->iddatatype == IT::CHR)
								*stream << "movzx ebx, " << secondId->id << '\n';
							else
								*stream << "mov ebx, " << secondId->id << '\n';
							*stream << "cmp eax, ebx" << '\n';
							*stream << oper << " If" << whileNumber << "End" << '\n';
						}
						getExpressionParams = false;
						leftSquareCount++;

						break;
					}

					case LEX_RIGHT_SQUAREBRACE: {
						leftSquareCount--;
						break;
					}
					}
					//закончился наш цикл
					if (leftSquareCount == 0 && !getExpressionParams)
					{

						InvokeExpressions(stream, t, st, i + pos, salt + 1);
						//*stream << "jmp If" << whileNumber << "Start" << '\n';
						*stream << "If" << whileNumber << "End: " << '\n';
						i += pos;
						break;
					}
					pos++;

				}
			}
		}
	}

	void Initialize(IT::Entry wher, IT::Entry from, std::ofstream* stream) {
		*stream << '\n';
		*stream << "push" << from.id;


		*stream << "pop" << wher.id;
	}

	void Head(std::ofstream* stream, LEX::LEX t) {

		*stream << ".586\n";
		*stream << "\t.model flat, stdcall\n";
		*stream << "\tincludelib libucrt.lib\n";
		*stream << "\tincludelib kernel32.lib\n";
		*stream << "\tincludelib ../Debug/SMD_2024Lib.lib\n";

		*stream << "\tExitProcess PROTO :DWORD\n\n";
		for (int i = 0; i < t.idtable.size; i++)
		{
			if (t.idtable.table[i].idtype == IT::F)
			{	//Если библиотечная
				if (t.idtable.table[i].isExternal == true)
				{
					*stream << "\n\t" << t.idtable.table[i].id << " PROTO";
					int pos = 1;
					bool commaFlag = false;
					while (true)
					{
						if (t.lextable.table[t.idtable.table[i].idxfirstLE + pos].lexema == LEX_ID
							&&
							t.idtable.table[t.lextable.table[t.idtable.table[i].idxfirstLE + pos].idxTI].idtype == IT::P)
						{
							if (commaFlag)
							{
								*stream << ',';
							}
							commaFlag = true;
							switch (t.idtable.table[t.lextable.table[t.idtable.table[i].idxfirstLE + pos].idxTI].iddatatype)
							{
							case IT::INT: {
								*stream << " :DWORD ";
								break;
							}
							case IT::CHR: {
								*stream << " :BYTE";
								break;
							}
							case IT::STR: {
								*stream << " :DWORD";
								break;
							}
							}
						}
						if (t.lextable.table[t.idtable.table[i].idxfirstLE + pos].lexema == LEX_RIGHTHESIS)
							break;
						pos++;
					}
				}
			}
		}
		*stream << "\noutputIntNum PROTO :DWORD";
		*stream << "\noutputSymb PROTO :BYTE";
		*stream << "\noutputLine PROTO :DWORD\n";

		*stream << "\n.stack 4096\n";
	}
	void Constants(std::ofstream* stream, LEX::LEX t) {
		*stream << ".const\n";
		*stream << "divideOnZeroExeption BYTE \"Попытка деления на ноль.\", 0  ;STR, для вывода ошибки при делении на ноль\n";
		for (int i = 0; i < t.idtable.size; i++)
		{
			if (t.idtable.table[i].idtype == IT::L)
			{
				*stream << "\t" << t.idtable.table[i].id;
				switch (t.idtable.table[i].iddatatype) {
				case IT::INT: {
					*stream << " DWORD " << t.idtable.table[i].value.vint << " ;INT";
					break;
				}
				case IT::CHR: {
					*stream << " BYTE '" << t.idtable.table[i].value.vchar << "' ;CHR";
					break;
				}
				case IT::STR: {
					if (strlen(t.idtable.table[i].value.vstr.str) > 2)
						*stream << " BYTE " << t.idtable.table[i].value.vstr.str << ", 0 " << " ;STR";
					else
						*stream << " BYTE " << "0 " << " ;STR";
					break;
				}
				}
				*stream << '\n';
			}
		}
	}
	void Data(std::ofstream* stream, LEX::LEX t) {
		*stream << ".data\n";
		for (int i = 0; i < t.idtable.size; i++) {
			if (t.idtable.table[i].idtype == IT::V)
			{
				*stream << "\t" << t.idtable.table[i].id;
				switch (t.idtable.table[i].iddatatype) {
				case IT::INT: {
					*stream << " DWORD 0 ;INT";
					break;
				}
				case IT::CHR: {
					*stream << " BYTE 0 ;CHR";
					break;
				}
				case IT::STR: {
					*stream << " DWORD 0 ;STR";
					break;
				}
				}
				*stream << '\n';
			}
		}
	}

	void WriteFunctions(std::ofstream* stream, LEX::LEX t)
	{
		for (int i = 0; i < t.idtable.size; i++)
		{
			if (t.idtable.table[i].idtype == IT::F)
			{	//если не библиотечная
				if (t.idtable.table[i].isExternal == false)
				{

					*stream << "$" << t.idtable.table[i].id << " PROC uses ebx ecx edi esi ";
					int pos = 1;
					int retsize = 0;
					bool commaFlag = false;
					while (true)
					{
						//запись параметров
						if (t.lextable.table[t.idtable.table[i].idxfirstLE + pos].lexema == LEX_ID
							&&
							t.idtable.table[t.lextable.table[t.idtable.table[i].idxfirstLE + pos].idxTI].idtype == IT::P)
						{
							*stream << ',';
							if (commaFlag)
							{
								*stream << ',';
							}
							commaFlag = false;
							*stream << "\t" << t.idtable.table[t.lextable.table[t.idtable.table[i].idxfirstLE + pos].idxTI].id;
							switch (t.idtable.table[t.lextable.table[t.idtable.table[i].idxfirstLE + pos].idxTI].iddatatype)
							{
							case IT::INT: {
								*stream << ": DWORD ";
								retsize += 4;
								break;
							}
							case IT::CHR: {
								*stream << ": BYTE";
								retsize += 1;
								break;
							}
							case IT::STR: {
								*stream << ": DWORD";
								retsize += 4;
								break;
							}
							}
						}
						if (t.lextable.table[t.idtable.table[i].idxfirstLE + pos].lexema == LEX_RIGHTHESIS)
							break;
						pos++;
					}
					int start = t.idtable.table[i].idxfirstLE + pos, end;
					while (t.lextable.table[t.idtable.table[i].idxfirstLE + pos].lexema != LEX_RETURN)
						pos++;
					pos += 4;
					end = t.idtable.table[i].idxfirstLE + pos;
					InvokeExpressions(stream, t, start, end);
					//TODO: проверить
					*stream << "ret";//<< retsize;
					*stream << "\n" << "$" << t.idtable.table[i].id << " ENDP\n\n";
				}
			}
		}
	}

	void Code(std::ofstream* stream, LEX::LEX t) {							// сегмент кода - исполняемый код 
		*stream << "\n.code\n";												// сегмент кода - исполняемый код 

		WriteFunctions(stream, t);

		*stream << "main PROC\n";
		int i = 0, st;
		while (t.lextable.table[i].lexema != LEX_MAIN)
			i++;
		st = i;
		while (t.lextable.table[i].lexema != LEX_RETURN)
			i++;
		i += 4;
		InvokeExpressions(stream, t, st, i);

		*stream << "\tjmp endPoint\n";
		*stream << "\tdiv_by_0:\n";
		*stream << "\tpush offset divideOnZeroExeption\nCALL outputLine\n";
		*stream << "\endPoint:\n";
		*stream << "\tinvoke\t\tExitProcess, eax\n";
		*stream << "main ENDP\n";
		*stream << "end main";
	}

	//std::ofstream("../SVY-2023ASM/SVY-2023ASM.asm");
	void Generate(LEX::LEX t, Out::OUT o) {
		std::ofstream* out = o.stream;
		Head(out, t);
		Constants(out, t);
		Data(out, t);
		Code(out, t);
	}
}
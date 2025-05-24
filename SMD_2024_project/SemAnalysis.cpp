#pragma once
#include "SemAnalysis.h"
#include "LexAnalysis.h"


namespace Semantic {
	void doAnalyse(LEX::LEX t) {
		operands(t);//Проверяет корректность операций и использования операторов.
		parameters(t);//Проверяет корректность передаваемых параметров при вызове функций.
		returns(t);//Проверяет корректность возвращаемых значений в функциях.
		

	};

	void operands(LEX::LEX t) {
		for (int i = 0; i < t.lextable.size; i++) {
			if (t.lextable.table[i].lexema == LEX_OPERATOR)//Если текущая лексема — это оператор 
			{
				switch (t.lextable.table[i].data)
				{
				case '=':
				{
					int pos = -1;//используется для прохода влево и вправо по выражению.
					
					int datatype = -1;// запоминает тип данных первого встреченного операнда
					while (t.lextable.table[i + pos].lexema != LEX_SEMICOLON) {
						if (t.lextable.table[i + pos].lexema == LEX_LITERAL || t.lextable.table[i + pos].lexema == LEX_ID)
						{
							if (datatype == -1)
								datatype = t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype;//если тип ещё не определён, код получает тип данных текущего операнда из таблицы идентификаторов
							else
							
								if (datatype != t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype)
									throw ERROR_THROW_IN(704, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)

									//если функция то пропускай  параметры функции
									if (t.idtable.table[t.lextable.table[i + pos].idxTI].idtype == IT::F) {
										while (t.lextable.table[i + pos].lexema != LEX_RIGHTHESIS)
											pos++;
									}
						}
						
						if (datatype == IT::STR && t.lextable.table[i + pos].lexema == LEX_OPERATOR && pos != 0 && t.lextable.table[i + pos].data != '+') {
							//это означает, что для строк используется недопустимый оператор (например, -, *, /).Генерируется ошибка 
							throw ERROR_THROW_IN(708, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						}
						if (datatype == IT::CHR && t.lextable.table[i + pos].lexema == LEX_OPERATOR && pos != 0 && t.lextable.table[i + pos].data != '+' && t.lextable.table[i + pos].data != '-') {
							//Если все условия выполняются, это означает, что для символов используется недопустимый оператор (например, *, /).Генерируется ошибка 
							throw ERROR_THROW_IN(710, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						}
						pos++;
					}
					i += pos - 1;
					break;
				}
				}
			}
			//проверка аргументов while
			if (t.lextable.table[i].lexema == LEX_UNTIL) {
				int pos = 2;
				while (true) {
					
					if (t.lextable.table[i + pos].lexema == LEX_LITERAL || t.lextable.table[i + pos].lexema == LEX_ID)
					{
					//	Из таблицы идентификаторов(t.idtable) извлекается информация о типе данных текущей лексемы.Проверяется, равен ли тип данных IT::STR (строка)
						if (t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype == IT::STR)
							
							throw ERROR_THROW_IN(709, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
					}
					pos++;
					
					// Лексема — закрывающая скобка, Пройдено более 6 лексем
					if (t.lextable.table[i + pos].lexema == LEX_RIGHTHESIS || pos > 6)
						break;
				}
			}
		}
	}

	//проверка передаваемых в функцию параметров
	void parameters(LEX::LEX t) {
		for (int i = 0; i < t.lextable.size; i++) {
			//Если текущая лексема — PRINT, а следующая лексема (i + 1) является идентификатором функции (IT::F), выбрасывается ошибка:
			if (t.lextable.table[i].lexema == LEX_PRINT && t.idtable.table[t.lextable.table[i + 1].idxTI].idtype == IT::F)
				throw ERROR_THROW_IN(701, t.lextable.table[i + 1].sn, t.lextable.table[i + 1].cn)
				
				if (t.lextable.table[i].lexema == LEX_ID && t.idtable.table[t.lextable.table[i].idxTI].idtype == IT::F && t.lextable.table[i - 3].lexema != LEX_EXPORT && t.lextable.table[i - 3].lexema != LEX_DECLARE)
				{
					short* types = new short[256];
					int typesLen = 0;
					//где объявлена функция в LT
					int funcDeclarePos = t.idtable.table[t.lextable.table[i].idxTI].idxfirstLE;//указывает, где в лексической таблице начинается объявление функции
					while (t.lextable.table[++funcDeclarePos].lexema != LEX_RIGHTHESIS)//пока не встретится закрывающая круглая скобка (LEX_RIGHTHESIS),
					{
						
						if (t.lextable.table[funcDeclarePos].lexema == LEX_ID || t.lextable.table[funcDeclarePos].lexema == LEX_LITERAL)
							types[typesLen++] = t.idtable.table[t.lextable.table[funcDeclarePos].idxTI].iddatatype;//записывает тип данных этого параметра в массив types, чтобы позже сравнить с параметрами при вызове функции.
					}
					int pos = 1;
					int paramCount = 0;
					while (t.lextable.table[i + pos].lexema != LEX_RIGHTHESIS) {//Цикл продолжается, пока не встретится закрывающая круглая скобка (LEX_RIGHTHESIS), которая завершает список параметров функции.
						if (t.lextable.table[i + pos].lexema == LEX_ID || t.lextable.table[i + pos].lexema == LEX_LITERAL)// текущая лексема является параметром вызова функции.
							//проверяет, соответствует ли тип параметра вызова функции его ожидаемому типу.
							if (t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype != types[paramCount++])
								//Если типы не совпадают, выбрасывается ошибка 703
								throw ERROR_THROW_IN(703, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
								pos++;
					}
					if (paramCount != typesLen)//Проверяет, соответствует ли количество фактически переданных параметров вызова функции количеству, ожидаемому в объявлении.
						throw ERROR_THROW_IN(702, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)
						
						i += pos;
				}
		}
	}


	//возврат функции!!!!
	void returns(LEX::LEX t) {
		for (int i = 0; i < t.lextable.size; i++) {
			//Текущая лексема — идентификатор, Тип идентификатора — функция, За 3 лексемы до идентификатора есть ключевое слово declare, что подтверждает объявление функции
			if (t.lextable.table[i].lexema == LEX_ID && t.idtable.table[t.lextable.table[i].idxTI].idtype == IT::F && t.lextable.table[i - 3].lexema == LEX_DECLARE)
			{
				int pos = 1;
				//Эта строка извлекает тип данных идентификатора, связанного с текущей лексемой, и сохраняет его в переменную retType.
				short retType = t.idtable.table[t.lextable.table[i].idxTI].iddatatype;
				while (t.lextable.table[i + pos].lexema != LEX_RETURN) {//ищет лексему LEX_RETURN, указывающую на оператор возврата.
					pos++;
				}
				pos++;
			
				if ((t.lextable.table[i + pos].lexema == LEX_ID || t.lextable.table[i + pos].lexema == LEX_LITERAL) && t.idtable.table[t.lextable.table[i + pos].idxTI].idtype != IT::F && (short)t.idtable.table[t.lextable.table[i + pos].idxTI].iddatatype != retType)
					throw ERROR_THROW_IN(701, t.lextable.table[i + pos].sn, t.lextable.table[i + pos].cn)//типы не совпадают, выбрасывается ошибка 
			}
		}
	}
}

#pragma once
#include <iostream>

#include "Out.h"
#include "Log.h"
#include "In.h"
#include "IT.h"
#include "LT.h"
namespace FST
{
	struct RELATION //ребро:символ -> вершина графа переходов КА
	{
		char symbol; //символ перехода
		short nnode; //номер смежной вершины
		RELATION(
			char c = 0x00, // символ перехода
			short ns = NULL //новое состояние
		);
	};
	struct NODE	//вершина графа переходов
	{
		short n_relation;	//количество Инцидентные ребер 
		RELATION* relations; //инцидентные ребра
		NODE();
		NODE(
			short n,		//количество Инцидентные ребер
			RELATION rel, ... //список ребер
		);
	};
	struct FST	//недетерминированный конечный автомат
	{
		char* string; //цепочка (строка, завершается 0х00)
		short position; //текущая позиция в цепочке
		short nstates; //количество состояний автомата
		NODE* nodes;	//граф переходов: [0] - начальное состояние, [nstate-1] - конечное
		short* rstates; //возможные состояния автомата на данной позиции
		FST(
			char* s, //цепочка(строка, завершается 0х00)
			short ns, //количество состояний автомата
			NODE n, ...	//список состояний (граф переходов)
		);

	};
	bool execute(FST& fst); //выполнить автомат
	void LexAnalyzer(In::IN in, Out::OUT out, Log::LOG log, LT::LexTable& a, IT::IdTable& b); //анализатор лексем
}
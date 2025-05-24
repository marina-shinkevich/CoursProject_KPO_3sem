#include "FST.h"
#include "Error.h"
#include <cstdlib>
#include <iomanip>
#include <vector>
#include "LT.h"
#include "IT.h"
#include <stack>



struct libfuncs {
	std::string name;
	IT::IDDATATYPE params[255];
	int parcount = 0;
	IT::IDDATATYPE retval;
};

FST::RELATION::RELATION(char c, short ns)
{//переход между состояниями
	symbol = c;//символ перехода
	nnode = ns;//следующего состояния
}

FST::NODE::NODE() //по умолчанию
{
	//состояние конечного автомата с заданным числом переходов и их списком.
	n_relation = 0;
	RELATION* relations = NULL;
};
FST::NODE::NODE(short n, RELATION rel, ...) //с параметрами
{
	n_relation = n; //количество переходов из этого состояни
	RELATION* p = &rel;
	relations = new RELATION[n];// массив переходов 
	for (short i = 0; i < n; i++)
		relations[i] = p[i];
};
FST::FST::FST(char* s, short ns, NODE n, ...)
{
	string = s;
	nstates = ns;
	nodes = new NODE[ns];// массив состояний 
	NODE* p = &n;
	for (int k = 0; k < ns; k++)
		nodes[k] = p[k];
	rstates = new short[nstates];
	rstates[0] = 0;
	position = -1;
};
bool step(FST::FST& fst, short*& rstates, int length) //один шаг автомата
{
	bool rc = false;
	bool contFlag = false;
	std::swap(rstates, fst.rstates); // смена массивов
	for (short i = 0; i < fst.nstates; i++) //цикл по всем состояниям
	{
		if (rstates[i] == fst.position) //если возможная позиция текущая равна текущей позиции
			for (short j = 0; j < fst.nodes[i].n_relation; j++) //до количеста инцендентных ребер
			{
				if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) //если нашли такой символ
				{
					if (fst.position == length)//если последний символ в строке, то ищем есть ли второй узел(второй узел для выхода)
					{
						contFlag = true;
						continue;
					}
					else
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					}
				};
			};
		if ((rc == false) && contFlag) //если не нашли второй узел то возвращаемся к первому
		{
			for (short j = 0; j < fst.nodes[i].n_relation; j++) //до количеста инцендентных ребер
			{
				if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) //если нашли такой символ
				{

					fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
					rc = true;
				};
			};
		}
	};
	return rc;
}
struct col
{

public:
	col(void)
	{
		column = 1;
	}
	void reset_col()
	{
		column = 1;
	}
	void set_col(int a)
	{
		column = a;
	}
	int get_col()
	{
		return column;
	}
private:
	int column;
} colum;
bool FST::execute(FST& fst) //выполнить распознование цепочки
{

	fst.rstates[0] = 0;
	fst.position = -1;
	colum.reset_col();
	short* rstates = new short[fst.nstates];
	memset(rstates, 0xff, sizeof(short) * fst.nstates); //инициализируем пустотой (ff)
	short lstring = strlen(fst.string); //длина цепочки
	bool rc = true;
	for (short i = 0; i < lstring && rc; i++) //по длине цепочки
	{
		fst.position++;
		rc = step(fst, rstates, lstring); //Для каждого символа строки выполняется один шаг автомата
		colum.set_col(i);
	}
	delete[] rstates;
	return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);//true, если строка успешно распознана
}
void FST::LexAnalyzer(In::IN in, Out::OUT out, Log::LOG log, LT::LexTable& lextable, IT::IdTable& idtable) //выполнить распознование цепочки
{
	libfuncs libs[6];
	int libslen = 6;
	libs[1].name = "ascii";
	libs[1].params[0] = IT::CHR;
	libs[1].parcount = 1;
	libs[1].retval = IT::INT;

	libs[2].name = "Symb";
	libs[2].params[0] = IT::INT;
	libs[2].parcount = 1;
	libs[2].retval = IT::CHR;

	libs[3].name = "getCurrentDate";
	libs[3].params[0] = IT::INT;
	libs[3].parcount = 1;
	libs[3].retval = IT::INT;

	libs[4].name = "getCurrentMinuts";
	libs[4].params[0] = IT::INT;
	libs[4].parcount = 1;
	libs[4].retval = IT::INT;

	libs[5].name = "getCurrentHour";
	libs[5].params[0] = IT::INT;
	libs[5].parcount = 1;
	libs[5].retval = IT::INT;

	char* str = new char[255];

#pragma region "FSTS"

	FST l_IntNum(
		str,
		7, //количество состояний
		NODE(1, RELATION('I', 1)),
		NODE(1, RELATION('n', 2)),
		NODE(1, RELATION('t', 3)),
		NODE(1, RELATION('N', 4)),
		NODE(1, RELATION('u', 5)),
		NODE(1, RELATION('m', 6)),
		NODE()
	);
	FST l_until(
		str,
		6, //количество состояний
		NODE(1, RELATION('W', 1)),
		NODE(1, RELATION('h', 2)),
		NODE(1, RELATION('i', 3)),
		NODE(1, RELATION('l', 4)),
		NODE(1, RELATION('e', 5)),
		NODE()
	);
	FST l_if(
		str,
		3, //количество состояний
		NODE(1, RELATION('i', 1)),
		NODE(1, RELATION('f', 2)),
		NODE()
	);


	FST l_Out(
		str,
		4, //количество состояний
		NODE(1, RELATION('O', 1)),
		NODE(1, RELATION('u', 2)),
		NODE(1, RELATION('t', 3)),
		NODE()
	);

	FST l_Symb(
		str,
		5, //количество состояний
		NODE(1, RELATION('S', 1)),
		NODE(1, RELATION('y', 2)),
		NODE(1, RELATION('m', 3)),
		NODE(1, RELATION('b', 4)),
		NODE()
	);


	FST l_numberLiteral(
		str,
		2, //количество состояний
		NODE(20,

			RELATION('0', 0),
			RELATION('1', 0),
			RELATION('2', 0),
			RELATION('3', 0),
			RELATION('4', 0),
			RELATION('5', 0),
			RELATION('6', 0),
			RELATION('7', 0),
			RELATION('8', 0),
			RELATION('9', 0),

			RELATION('0', 1),
			RELATION('1', 1),
			RELATION('2', 1),
			RELATION('3', 1),
			RELATION('4', 1),
			RELATION('5', 1),
			RELATION('6', 1),
			RELATION('7', 1),
			RELATION('8', 1),
			RELATION('9', 1)
		),
		NODE()
	);
	FST l_SymbLiteral(
		str,
		4,
		NODE(1, RELATION('\'', 1)),
		NODE(256,
			RELATION((char)39, 2),
			RELATION((char)0, 2),
			RELATION((char)1, 2),
			RELATION((char)2, 2),
			RELATION((char)3, 2),
			RELATION((char)4, 2),
			RELATION((char)5, 2),
			RELATION((char)6, 2),
			RELATION((char)7, 2),
			RELATION((char)8, 2),
			RELATION((char)9, 2),
			RELATION((char)10, 2),
			RELATION((char)11, 2),
			RELATION((char)12, 2),
			RELATION((char)13, 2),
			RELATION((char)14, 2),
			RELATION((char)15, 2),
			RELATION((char)16, 2),
			RELATION((char)17, 2),
			RELATION((char)18, 2),
			RELATION((char)19, 2),
			RELATION((char)20, 2),
			RELATION((char)21, 2),
			RELATION((char)22, 2),
			RELATION((char)23, 2),
			RELATION((char)24, 2),
			RELATION((char)25, 2),
			RELATION((char)26, 2),
			RELATION((char)27, 2),
			RELATION((char)28, 2),
			RELATION((char)29, 2),
			RELATION((char)30, 2),
			RELATION((char)31, 2),
			RELATION((char)32, 2),
			RELATION((char)33, 2),
			RELATION((char)34, 2),
			RELATION((char)35, 2),
			RELATION((char)36, 2),
			RELATION((char)37, 2),
			RELATION((char)38, 2),
			RELATION((char)40, 2),
			RELATION((char)41, 2),
			RELATION((char)42, 2),
			RELATION((char)43, 2),
			RELATION((char)44, 2),
			RELATION((char)45, 2),
			RELATION((char)46, 2),
			RELATION((char)47, 2),
			RELATION((char)48, 2),
			RELATION((char)49, 2),
			RELATION((char)50, 2),
			RELATION((char)51, 2),
			RELATION((char)52, 2),
			RELATION((char)53, 2),
			RELATION((char)54, 2),
			RELATION((char)55, 2),
			RELATION((char)56, 2),
			RELATION((char)57, 2),
			RELATION((char)58, 2),
			RELATION((char)59, 2),
			RELATION((char)60, 2),
			RELATION((char)61, 2),
			RELATION((char)62, 2),
			RELATION((char)63, 2),
			RELATION((char)64, 2),
			RELATION((char)65, 2),
			RELATION((char)66, 2),
			RELATION((char)67, 2),
			RELATION((char)68, 2),
			RELATION((char)69, 2),
			RELATION((char)70, 2),
			RELATION((char)71, 2),
			RELATION((char)72, 2),
			RELATION((char)73, 2),
			RELATION((char)74, 2),
			RELATION((char)75, 2),
			RELATION((char)76, 2),
			RELATION((char)77, 2),
			RELATION((char)78, 2),
			RELATION((char)79, 2),
			RELATION((char)80, 2),
			RELATION((char)81, 2),
			RELATION((char)82, 2),
			RELATION((char)83, 2),
			RELATION((char)84, 2),
			RELATION((char)85, 2),
			RELATION((char)86, 2),
			RELATION((char)87, 2),
			RELATION((char)88, 2),
			RELATION((char)89, 2),
			RELATION((char)90, 2),
			RELATION((char)91, 2),
			RELATION((char)92, 2),
			RELATION((char)93, 2),
			RELATION((char)94, 2),
			RELATION((char)95, 2),
			RELATION((char)96, 2),
			RELATION((char)97, 2),
			RELATION((char)98, 2),
			RELATION((char)99, 2),
			RELATION((char)100, 2),
			RELATION((char)101, 2),
			RELATION((char)102, 2),
			RELATION((char)103, 2),
			RELATION((char)104, 2),
			RELATION((char)105, 2),
			RELATION((char)106, 2),
			RELATION((char)107, 2),
			RELATION((char)108, 2),
			RELATION((char)109, 2),
			RELATION((char)110, 2),
			RELATION((char)111, 2),
			RELATION((char)112, 2),
			RELATION((char)113, 2),
			RELATION((char)114, 2),
			RELATION((char)115, 2),
			RELATION((char)116, 2),
			RELATION((char)117, 2),
			RELATION((char)118, 2),
			RELATION((char)119, 2),
			RELATION((char)120, 2),
			RELATION((char)121, 2),
			RELATION((char)122, 2),
			RELATION((char)123, 2),
			RELATION((char)124, 2),
			RELATION((char)125, 2),
			RELATION((char)126, 2),
			RELATION((char)127, 2),
			RELATION((char)128, 2),
			RELATION((char)129, 2),
			RELATION((char)130, 2),
			RELATION((char)131, 2),
			RELATION((char)132, 2),
			RELATION((char)133, 2),
			RELATION((char)134, 2),
			RELATION((char)135, 2),
			RELATION((char)136, 2),
			RELATION((char)137, 2),
			RELATION((char)138, 2),
			RELATION((char)139, 2),
			RELATION((char)140, 2),
			RELATION((char)141, 2),
			RELATION((char)142, 2),
			RELATION((char)143, 2),
			RELATION((char)144, 2),
			RELATION((char)145, 2),
			RELATION((char)146, 2),
			RELATION((char)147, 2),
			RELATION((char)148, 2),
			RELATION((char)149, 2),
			RELATION((char)150, 2),
			RELATION((char)151, 2),
			RELATION((char)152, 2),
			RELATION((char)153, 2),
			RELATION((char)154, 2),
			RELATION((char)155, 2),
			RELATION((char)156, 2),
			RELATION((char)157, 2),
			RELATION((char)158, 2),
			RELATION((char)159, 2),
			RELATION((char)160, 2),
			RELATION((char)161, 2),
			RELATION((char)162, 2),
			RELATION((char)163, 2),
			RELATION((char)164, 2),
			RELATION((char)165, 2),
			RELATION((char)166, 2),
			RELATION((char)167, 2),
			RELATION((char)168, 2),
			RELATION((char)169, 2),
			RELATION((char)170, 2),
			RELATION((char)171, 2),
			RELATION((char)172, 2),
			RELATION((char)173, 2),
			RELATION((char)174, 2),
			RELATION((char)175, 2),
			RELATION((char)176, 2),
			RELATION((char)177, 2),
			RELATION((char)178, 2),
			RELATION((char)179, 2),
			RELATION((char)180, 2),
			RELATION((char)181, 2),
			RELATION((char)182, 2),
			RELATION((char)183, 2),
			RELATION((char)184, 2),
			RELATION((char)185, 2),
			RELATION((char)186, 2),
			RELATION((char)187, 2),
			RELATION((char)188, 2),
			RELATION((char)189, 2),
			RELATION((char)190, 2),
			RELATION((char)191, 2),
			RELATION((char)192, 2),
			RELATION((char)193, 2),
			RELATION((char)194, 2),
			RELATION((char)195, 2),
			RELATION((char)196, 2),
			RELATION((char)197, 2),
			RELATION((char)198, 2),
			RELATION((char)199, 2),
			RELATION((char)200, 2),
			RELATION((char)201, 2),
			RELATION((char)202, 2),
			RELATION((char)203, 2),
			RELATION((char)204, 2),
			RELATION((char)205, 2),
			RELATION((char)206, 2),
			RELATION((char)207, 2),
			RELATION((char)208, 2),
			RELATION((char)209, 2),
			RELATION((char)210, 2),
			RELATION((char)211, 2),
			RELATION((char)212, 2),
			RELATION((char)213, 2),
			RELATION((char)214, 2),
			RELATION((char)215, 2),
			RELATION((char)216, 2),
			RELATION((char)217, 2),
			RELATION((char)218, 2),
			RELATION((char)219, 2),
			RELATION((char)220, 2),
			RELATION((char)221, 2),
			RELATION((char)222, 2),
			RELATION((char)223, 2),
			RELATION((char)224, 2),
			RELATION((char)225, 2),
			RELATION((char)226, 2),
			RELATION((char)227, 2),
			RELATION((char)228, 2),
			RELATION((char)229, 2),
			RELATION((char)230, 2),
			RELATION((char)231, 2),
			RELATION((char)232, 2),
			RELATION((char)233, 2),
			RELATION((char)234, 2),
			RELATION((char)235, 2),
			RELATION((char)236, 2),
			RELATION((char)237, 2),
			RELATION((char)238, 2),
			RELATION((char)239, 2),
			RELATION((char)240, 2),
			RELATION((char)241, 2),
			RELATION((char)242, 2),
			RELATION((char)243, 2),
			RELATION((char)244, 2),
			RELATION((char)245, 2),
			RELATION((char)246, 2),
			RELATION((char)247, 2),
			RELATION((char)248, 2),
			RELATION((char)249, 2),
			RELATION((char)250, 2),
			RELATION((char)251, 2),
			RELATION((char)252, 2),
			RELATION((char)253, 2),
			RELATION((char)254, 2),
			RELATION((char)255, 2)),
		NODE(1, RELATION('\'', 3)),
		NODE());

	FST l_LineLiteral(
		str,
		3, //количество состояний
		NODE(1, RELATION('\"', 1)),
		NODE(256,
			RELATION('\"', 2),
			RELATION((char)0, 1),
			RELATION((char)1, 1),
			RELATION((char)2, 1),
			RELATION((char)3, 1),
			RELATION((char)4, 1),
			RELATION((char)5, 1),
			RELATION((char)6, 1),
			RELATION((char)7, 1),
			RELATION((char)8, 1),
			RELATION((char)9, 1),
			RELATION((char)10, 1),
			RELATION((char)11, 1),
			RELATION((char)12, 1),
			RELATION((char)13, 1),
			RELATION((char)14, 1),
			RELATION((char)15, 1),
			RELATION((char)16, 1),
			RELATION((char)17, 1),
			RELATION((char)18, 1),
			RELATION((char)19, 1),
			RELATION((char)20, 1),
			RELATION((char)21, 1),
			RELATION((char)22, 1),
			RELATION((char)23, 1),
			RELATION((char)24, 1),
			RELATION((char)25, 1),
			RELATION((char)26, 1),
			RELATION((char)27, 1),
			RELATION((char)28, 1),
			RELATION((char)29, 1),
			RELATION((char)30, 1),
			RELATION((char)31, 1),
			RELATION((char)32, 1),
			RELATION((char)33, 1),
			RELATION((char)35, 1),
			RELATION((char)36, 1),
			RELATION((char)37, 1),
			RELATION((char)38, 1),
			RELATION((char)39, 1),
			RELATION((char)40, 1),
			RELATION((char)41, 1),
			RELATION((char)42, 1),
			RELATION((char)43, 1),
			RELATION((char)44, 1),
			RELATION((char)45, 1),
			RELATION((char)46, 1),
			RELATION((char)47, 1),
			RELATION((char)48, 1),
			RELATION((char)49, 1),
			RELATION((char)50, 1),
			RELATION((char)51, 1),
			RELATION((char)52, 1),
			RELATION((char)53, 1),
			RELATION((char)54, 1),
			RELATION((char)55, 1),
			RELATION((char)56, 1),
			RELATION((char)57, 1),
			RELATION((char)58, 1),
			RELATION((char)59, 1),
			RELATION((char)60, 1),
			RELATION((char)61, 1),
			RELATION((char)62, 1),
			RELATION((char)63, 1),
			RELATION((char)64, 1),
			RELATION((char)65, 1),
			RELATION((char)66, 1),
			RELATION((char)67, 1),
			RELATION((char)68, 1),
			RELATION((char)69, 1),
			RELATION((char)70, 1),
			RELATION((char)71, 1),
			RELATION((char)72, 1),
			RELATION((char)73, 1),
			RELATION((char)74, 1),
			RELATION((char)75, 1),
			RELATION((char)76, 1),
			RELATION((char)77, 1),
			RELATION((char)78, 1),
			RELATION((char)79, 1),
			RELATION((char)80, 1),
			RELATION((char)81, 1),
			RELATION((char)82, 1),
			RELATION((char)83, 1),
			RELATION((char)84, 1),
			RELATION((char)85, 1),
			RELATION((char)86, 1),
			RELATION((char)87, 1),
			RELATION((char)88, 1),
			RELATION((char)89, 1),
			RELATION((char)90, 1),
			RELATION((char)91, 1),
			RELATION((char)92, 1),
			RELATION((char)93, 1),
			RELATION((char)94, 1),
			RELATION((char)95, 1),
			RELATION((char)96, 1),
			RELATION((char)97, 1),
			RELATION((char)98, 1),
			RELATION((char)99, 1),
			RELATION((char)100, 1),
			RELATION((char)101, 1),
			RELATION((char)102, 1),
			RELATION((char)103, 1),
			RELATION((char)104, 1),
			RELATION((char)105, 1),
			RELATION((char)106, 1),
			RELATION((char)107, 1),
			RELATION((char)108, 1),
			RELATION((char)109, 1),
			RELATION((char)110, 1),
			RELATION((char)111, 1),
			RELATION((char)112, 1),
			RELATION((char)113, 1),
			RELATION((char)114, 1),
			RELATION((char)115, 1),
			RELATION((char)116, 1),
			RELATION((char)117, 1),
			RELATION((char)118, 1),
			RELATION((char)119, 1),
			RELATION((char)120, 1),
			RELATION((char)121, 1),
			RELATION((char)122, 1),
			RELATION((char)123, 1),
			RELATION((char)124, 1),
			RELATION((char)125, 1),
			RELATION((char)126, 1),
			RELATION((char)127, 1),
			RELATION((char)128, 1),
			RELATION((char)129, 1),
			RELATION((char)130, 1),
			RELATION((char)131, 1),
			RELATION((char)132, 1),
			RELATION((char)133, 1),
			RELATION((char)134, 1),
			RELATION((char)135, 1),
			RELATION((char)136, 1),
			RELATION((char)137, 1),
			RELATION((char)138, 1),
			RELATION((char)139, 1),
			RELATION((char)140, 1),
			RELATION((char)141, 1),
			RELATION((char)142, 1),
			RELATION((char)143, 1),
			RELATION((char)144, 1),
			RELATION((char)145, 1),
			RELATION((char)146, 1),
			RELATION((char)147, 1),
			RELATION((char)148, 1),
			RELATION((char)149, 1),
			RELATION((char)150, 1),
			RELATION((char)151, 1),
			RELATION((char)152, 1),
			RELATION((char)153, 1),
			RELATION((char)154, 1),
			RELATION((char)155, 1),
			RELATION((char)156, 1),
			RELATION((char)157, 1),
			RELATION((char)158, 1),
			RELATION((char)159, 1),
			RELATION((char)160, 1),
			RELATION((char)161, 1),
			RELATION((char)162, 1),
			RELATION((char)163, 1),
			RELATION((char)164, 1),
			RELATION((char)165, 1),
			RELATION((char)166, 1),
			RELATION((char)167, 1),
			RELATION((char)168, 1),
			RELATION((char)169, 1),
			RELATION((char)170, 1),
			RELATION((char)171, 1),
			RELATION((char)172, 1),
			RELATION((char)173, 1),
			RELATION((char)174, 1),
			RELATION((char)175, 1),
			RELATION((char)176, 1),
			RELATION((char)177, 1),
			RELATION((char)178, 1),
			RELATION((char)179, 1),
			RELATION((char)180, 1),
			RELATION((char)181, 1),
			RELATION((char)182, 1),
			RELATION((char)183, 1),
			RELATION((char)184, 1),
			RELATION((char)185, 1),
			RELATION((char)186, 1),
			RELATION((char)187, 1),
			RELATION((char)188, 1),
			RELATION((char)189, 1),
			RELATION((char)190, 1),
			RELATION((char)191, 1),
			RELATION((char)192, 1),
			RELATION((char)193, 1),
			RELATION((char)194, 1),
			RELATION((char)195, 1),
			RELATION((char)196, 1),
			RELATION((char)197, 1),
			RELATION((char)198, 1),
			RELATION((char)199, 1),
			RELATION((char)200, 1),
			RELATION((char)201, 1),
			RELATION((char)202, 1),
			RELATION((char)203, 1),
			RELATION((char)204, 1),
			RELATION((char)205, 1),
			RELATION((char)206, 1),
			RELATION((char)207, 1),
			RELATION((char)208, 1),
			RELATION((char)209, 1),
			RELATION((char)210, 1),
			RELATION((char)211, 1),
			RELATION((char)212, 1),
			RELATION((char)213, 1),
			RELATION((char)214, 1),
			RELATION((char)215, 1),
			RELATION((char)216, 1),
			RELATION((char)217, 1),
			RELATION((char)218, 1),
			RELATION((char)219, 1),
			RELATION((char)220, 1),
			RELATION((char)221, 1),
			RELATION((char)222, 1),
			RELATION((char)223, 1),
			RELATION((char)224, 1),
			RELATION((char)225, 1),
			RELATION((char)226, 1),
			RELATION((char)227, 1),
			RELATION((char)228, 1),
			RELATION((char)229, 1),
			RELATION((char)230, 1),
			RELATION((char)231, 1),
			RELATION((char)232, 1),
			RELATION((char)233, 1),
			RELATION((char)234, 1),
			RELATION((char)235, 1),
			RELATION((char)236, 1),
			RELATION((char)237, 1),
			RELATION((char)238, 1),
			RELATION((char)239, 1),
			RELATION((char)240, 1),
			RELATION((char)241, 1),
			RELATION((char)242, 1),
			RELATION((char)243, 1),
			RELATION((char)244, 1),
			RELATION((char)245, 1),
			RELATION((char)246, 1),
			RELATION((char)247, 1),
			RELATION((char)248, 1),
			RELATION((char)249, 1),
			RELATION((char)250, 1),
			RELATION((char)251, 1),
			RELATION((char)252, 1),
			RELATION((char)253, 1),
			RELATION((char)254, 1),
			RELATION((char)255, 1)
		),
		NODE()
	);


	FST l_itendificator(
		str,
		3, //количество состояний
		NODE(106,
			RELATION('a', 1),
			RELATION('b', 1),
			RELATION('c', 1),
			RELATION('d', 1),
			RELATION('e', 1),
			RELATION('f', 1),
			RELATION('g', 1),
			RELATION('h', 1),
			RELATION('i', 1),
			RELATION('j', 1),
			RELATION('k', 1),
			RELATION('l', 1),
			RELATION('m', 1),
			RELATION('n', 1),
			RELATION('o', 1),
			RELATION('p', 1),
			RELATION('q', 1),
			RELATION('r', 1),
			RELATION('s', 1),
			RELATION('t', 1),
			RELATION('u', 1),
			RELATION('v', 1),
			RELATION('w', 1),
			RELATION('x', 1),
			RELATION('y', 1),
			RELATION('z', 1),
			RELATION('A', 1),
			RELATION('B', 1),
			RELATION('C', 1),
			RELATION('D', 1),
			RELATION('E', 1),
			RELATION('F', 1),
			RELATION('G', 1),
			RELATION('H', 1),
			RELATION('I', 1),
			RELATION('J', 1),
			RELATION('K', 1),
			RELATION('L', 1),
			RELATION('M', 1),
			RELATION('N', 1),
			RELATION('O', 1),
			RELATION('P', 1),
			RELATION('Q', 1),
			RELATION('R', 1),
			RELATION('S', 1),
			RELATION('T', 1),
			RELATION('U', 1),
			RELATION('V', 1),
			RELATION('W', 1),
			RELATION('X', 1),
			RELATION('Y', 1),
			RELATION('Z', 1),
			RELATION('_', 1),
			RELATION('a', 2),
			RELATION('b', 2),
			RELATION('c', 2),
			RELATION('d', 2),
			RELATION('e', 2),
			RELATION('f', 2),
			RELATION('g', 2),
			RELATION('h', 2),
			RELATION('i', 2),
			RELATION('j', 2),
			RELATION('k', 2),
			RELATION('l', 2),
			RELATION('m', 2),
			RELATION('n', 2),
			RELATION('o', 2),
			RELATION('p', 2),
			RELATION('q', 2),
			RELATION('r', 2),
			RELATION('s', 2),
			RELATION('t', 2),
			RELATION('u', 2),
			RELATION('v', 2),
			RELATION('w', 2),
			RELATION('x', 2),
			RELATION('y', 2),
			RELATION('z', 2),
			RELATION('A', 2),
			RELATION('B', 2),
			RELATION('C', 2),
			RELATION('D', 2),
			RELATION('E', 2),
			RELATION('F', 2),
			RELATION('G', 2),
			RELATION('H', 2),
			RELATION('I', 2),
			RELATION('J', 2),
			RELATION('K', 2),
			RELATION('L', 2),
			RELATION('M', 2),
			RELATION('N', 2),
			RELATION('O', 2),
			RELATION('P', 2),
			RELATION('Q', 2),
			RELATION('R', 2),
			RELATION('S', 2),
			RELATION('T', 2),
			RELATION('U', 2),
			RELATION('V', 2),
			RELATION('W', 2),
			RELATION('X', 2),
			RELATION('Y', 2),
			RELATION('Z', 2),
			RELATION('_', 2)
		),
		NODE(126,
			RELATION('a', 1),
			RELATION('b', 1),
			RELATION('c', 1),
			RELATION('d', 1),
			RELATION('e', 1),
			RELATION('f', 1),
			RELATION('g', 1),
			RELATION('h', 1),
			RELATION('i', 1),
			RELATION('j', 1),
			RELATION('k', 1),
			RELATION('l', 1),
			RELATION('m', 1),
			RELATION('n', 1),
			RELATION('o', 1),
			RELATION('p', 1),
			RELATION('q', 1),
			RELATION('r', 1),
			RELATION('s', 1),
			RELATION('t', 1),
			RELATION('u', 1),
			RELATION('v', 1),
			RELATION('w', 1),
			RELATION('x', 1),
			RELATION('y', 1),
			RELATION('z', 1),
			RELATION('A', 1),
			RELATION('B', 1),
			RELATION('C', 1),
			RELATION('D', 1),
			RELATION('E', 1),
			RELATION('F', 1),
			RELATION('G', 1),
			RELATION('H', 1),
			RELATION('I', 1),
			RELATION('J', 1),
			RELATION('K', 1),
			RELATION('L', 1),
			RELATION('M', 1),
			RELATION('N', 1),
			RELATION('O', 1),
			RELATION('P', 1),
			RELATION('Q', 1),
			RELATION('R', 1),
			RELATION('S', 1),
			RELATION('T', 1),
			RELATION('U', 1),
			RELATION('V', 1),
			RELATION('W', 1),
			RELATION('X', 1),
			RELATION('Y', 1),
			RELATION('Z', 1),
			RELATION('_', 1),
			RELATION('0', 1),
			RELATION('1', 1),
			RELATION('2', 1),
			RELATION('3', 1),
			RELATION('4', 1),
			RELATION('5', 1),
			RELATION('6', 1),
			RELATION('7', 1),
			RELATION('8', 1),
			RELATION('9', 1),
			RELATION('a', 2),
			RELATION('b', 2),
			RELATION('c', 2),
			RELATION('d', 2),
			RELATION('e', 2),
			RELATION('f', 2),
			RELATION('g', 2),
			RELATION('h', 2),
			RELATION('i', 2),
			RELATION('j', 2),
			RELATION('k', 2),
			RELATION('l', 2),
			RELATION('m', 2),
			RELATION('n', 2),
			RELATION('o', 2),
			RELATION('p', 2),
			RELATION('q', 2),
			RELATION('r', 2),
			RELATION('s', 2),
			RELATION('t', 2),
			RELATION('u', 2),
			RELATION('v', 2),
			RELATION('w', 2),
			RELATION('x', 2),
			RELATION('y', 2),
			RELATION('z', 2),
			RELATION('A', 2),
			RELATION('B', 2),
			RELATION('C', 2),
			RELATION('D', 2),
			RELATION('E', 2),
			RELATION('F', 2),
			RELATION('G', 2),
			RELATION('H', 2),
			RELATION('I', 2),
			RELATION('J', 2),
			RELATION('K', 2),
			RELATION('L', 2),
			RELATION('M', 2),
			RELATION('N', 2),
			RELATION('O', 2),
			RELATION('P', 2),
			RELATION('Q', 2),
			RELATION('R', 2),
			RELATION('S', 2),
			RELATION('T', 2),
			RELATION('U', 2),
			RELATION('V', 2),
			RELATION('W', 2),
			RELATION('X', 2),
			RELATION('Y', 2),
			RELATION('Z', 2),
			RELATION('_', 2),
			RELATION('0', 2),
			RELATION('1', 2),
			RELATION('2', 2),
			RELATION('3', 2),
			RELATION('4', 2),
			RELATION('5', 2),
			RELATION('6', 2),
			RELATION('7', 2),
			RELATION('8', 2),
			RELATION('9', 2)
		),
		NODE()
	);

	FST l_Line(
		str,
		5, //количество состояний
		NODE(1, RELATION('L', 1)),
		NODE(1, RELATION('i', 2)),
		NODE(1, RELATION('n', 3)),
		NODE(1, RELATION('e', 4)),
		NODE()
	);

	FST l_Func(
		str,
		5, //количество состояний
		NODE(1, RELATION('F', 1)),
		NODE(1, RELATION('u', 2)),
		NODE(1, RELATION('n', 3)),
		NODE(1, RELATION('c', 4)),
		NODE()
	);
	FST l_Def(
		str,
		4, //количество состояний
		NODE(1, RELATION('D', 1)),
		NODE(1, RELATION('e', 2)),
		NODE(1, RELATION('f', 3)),
		NODE()
	);
	FST l_Get(
		str,
		4, //количество состояний
		NODE(1, RELATION('G', 1)),
		NODE(1, RELATION('e', 2)),
		NODE(1, RELATION('t', 3)),
		NODE()
	);
	FST l_Show(
		str,
		5, //количество состояний
		NODE(1, RELATION('S', 1)),
		NODE(1, RELATION('h', 2)),
		NODE(1, RELATION('o', 3)),
		NODE(1, RELATION('w', 4)),
		NODE()
	);
	FST l_prints(
		str,
		5, //количество состояний
		NODE(1, RELATION('s', 1)),
		NODE(1, RELATION('a', 2)),
		NODE(1, RELATION('y', 3)),
		NODE(1, RELATION('s', 4)),
		NODE()
	);

	FST l_main(
		str,
		5, //количество состояний
		NODE(1, RELATION('M', 1)),
		NODE(1, RELATION('a', 2)),
		NODE(1, RELATION('i', 3)),
		NODE(1, RELATION('n', 4)),
		NODE()
	);
	FST l_conditional(
		str,
		3, //количество состояний
		NODE(1, RELATION('i', 1)),
		NODE(1, RELATION('f', 2)),
		NODE()
	);
	FST l_semicolon(
		str,
		2, //количество состояний
		NODE(1, RELATION(';', 1)),
		NODE()
	);
	FST l_comma(
		str,
		2, //количество состояний
		NODE(1, RELATION(',', 1)),
		NODE()
	);
	FST l_braceleft(
		str,
		2, //количество состояний
		NODE(1, RELATION('{', 1)),
		NODE()
	);
	FST l_braceright(
		str,
		2, //количество состояний
		NODE(1, RELATION('}', 1)),
		NODE()
	);
	FST l_lefthesis(
		str,
		2, //количество состояний
		NODE(1, RELATION('(', 1)),
		NODE()
	);
	FST l_cycleStart(
		str,
		2, //количество состояний
		NODE(1, RELATION('[', 1)),
		NODE()
	);
	FST l_cycleEnd(
		str,
		2, //количество состояний
		NODE(1, RELATION(']', 1)),
		NODE()
	);
	FST l_conditionalStart(
		str,
		2, //количество состояний
		NODE(1, RELATION('[', 1)),
		NODE()
	);
	FST l_conditionalEnd(
		str,
		2, //количество состояний
		NODE(1, RELATION(']', 1)),
		NODE()
	);
	FST l_righthesis(
		str,
		2, //количество состояний
		NODE(1, RELATION(')', 1)),
		NODE()
	);

	FST l_verb(
		str,
		2, //количество состояний
		NODE(8, RELATION('+', 1), RELATION('-', 1), RELATION('*', 1),
			RELATION('/', 1), RELATION(':', 1), RELATION('\\', 1), RELATION('%', 1), RELATION('=', 1)),
		NODE()
	);
	FST l_boolVerb(
		str,
		2, //количество состояний
		NODE(4, RELATION('!', 1), RELATION('<', 1), RELATION('>', 1)),
		NODE()
	);


#pragma endregion

#pragma region "Структура checker и её массив автоматов"
	struct Checker {
		FST* chain;
		IT::IDDATATYPE iddatatype;

		char lexName;
		Checker(FST* f, char c, IT::IDDATATYPE t) {
			chain = f;
			lexName = c;
			iddatatype = t;
		}
	};
	//связь лексем и конечного автомата
	Checker checkArr[] = {
		Checker(&l_IntNum,LEX_INTEGER,IT::INT),
		Checker(&l_Symb,LEX_STRING,IT::CHR),

		Checker(&l_Line,LEX_CHAR,IT::STR),
		Checker(&l_Func,LEX_FUNCTION,(IT::IDDATATYPE)FALSYNUMBER),

		Checker(&l_Def,LEX_DECLARE,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_until,LEX_UNTIL,(IT::IDDATATYPE)FALSYNUMBER),

		Checker(&l_if,LEX_IF,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_Get,LEX_RETURN,(IT::IDDATATYPE)FALSYNUMBER),

		Checker(&l_Out,LEX_EXPORT,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_main,LEX_MAIN,(IT::IDDATATYPE)FALSYNUMBER),
		//
		Checker(&l_Show,LEX_PRINT,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_braceleft,LEX_LEFTBRACE,(IT::IDDATATYPE)FALSYNUMBER),

		Checker(&l_braceright,LEX_RIGHTBRACE,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_lefthesis,LEX_LEFTHESIS,(IT::IDDATATYPE)FALSYNUMBER),

		Checker(&l_righthesis,LEX_RIGHTHESIS,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_cycleStart,LEX_LEFT_SQUAREBRACE,(IT::IDDATATYPE)FALSYNUMBER),

		Checker(&l_cycleEnd,LEX_RIGHT_SQUAREBRACE,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_comma,LEX_COMMA,(IT::IDDATATYPE)FALSYNUMBER),

		Checker(&l_semicolon,LEX_SEMICOLON,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_verb,LEX_PLUS,(IT::IDDATATYPE)FALSYNUMBER),
		//
		Checker(&l_boolVerb,LEX_BOOL_OPERATOR,(IT::IDDATATYPE)FALSYNUMBER),
		Checker(&l_itendificator,LEX_ID,(IT::IDDATATYPE)FALSYNUMBER),

		Checker(&l_numberLiteral,LEX_LITERAL,IT::INT),
		Checker(&l_LineLiteral,LEX_LITERAL,IT::STR),

		Checker(&l_SymbLiteral,LEX_LITERAL,IT::CHR),
		//len = 22
	};
	const int checkArrLen = 25;


#pragma endregion

#pragma region "Флаги"
	std::string scopeName;
	std::vector <std::string> scopeStack;
	int literalCount = 0;
	scopeStack.push_back("\0");
	int scopeCount = 0;
	int scopeNumber = 0; // aka currentScope
	int openedBrace = 0;
	bool isDeclare = false;
	bool isExported = false;
	bool isRequireBodyFunc = false;
	bool isMain = false;
	bool isFunction = false;
	bool isFunctionName = false;
	bool isFunctionParam = false;
	bool isParametr = false;
	bool isLiteral = false;
	bool wasMain = false;
	bool isBoleanExpression = false;
	int hesisCounter = 0;
	IT::IDDATATYPE dataType = (IT::IDDATATYPE)FALSYNUMBER;
	IT::IDTYPE type = (IT::IDTYPE)FALSYNUMBER;
#pragma endregion

	int libPos = 0;
	std::stack<IT::IDDATATYPE> params;
	bool parmStackShouldWrite = false;


	int sizze = in.lexems.size();
#pragma region "перебор"
	for (int i = 0; i < sizze; i++)
	{
		In::lexem lex = in.lexems.front();
		in.lexems.pop_front();

		int len = strlen((const char*)(lex.lexem)); //длина строки ktrctvs
#pragma region "запись лексемы в str"
		for (int k = 0; k < len; k++)
		{
			str[k] = lex.lexem[k];
		}
		str[len] = '\0';
#pragma endregion

		bool executedFlag = false;	// флаг окончания перебора цепочек

		for (int j = 0; j < checkArrLen; j++)
		{
			if (execute(*checkArr[j].chain))
			{
				executedFlag = true;
#pragma region "Установка флага"
				LT::Entry lexTableObject(checkArr[j].lexName, lex.line, lex.col, 0xffffffff);
				//нет until (уже есть)
				//проверяем что за лексема
				switch (checkArr[j].lexName)
				{
					//если встретили declare
				case LEX_DECLARE: {
					isDeclare = true;
					type = IT::V;
					break;
				}
				case LEX_UNTIL: {
					isBoleanExpression = true;
					break;
				}
				case LEX_BOOL_OPERATOR: {
					lexTableObject.lexema = LEX_BOOL_OPERATOR;
					break;
				}


									  //если встретили uint/symbol
				case LEX_TYPE: {
					if (checkArr[j].iddatatype == IT::INT)
					{
						dataType = IT::INT;
					}
					else if (checkArr[j].iddatatype == IT::STR)
					{
						dataType = IT::STR;
					}
					else if (checkArr[j].iddatatype == IT::CHR)
					{
						dataType = IT::CHR;
					}
					else
					{
						//дропай ошибку
						dataType = (IT::IDDATATYPE)FALSYNUMBER;
					}
					break;
				}
							 //функция
				case LEX_FUNCTION: {
					type = IT::F;
					break;
				}
								 //вывод
				case LEX_PRINT: {
					break;
				}
							  //идентификатор
				case LEX_ID: {
					//область видимости
					if (strlen(str) > ID_CURRENT_MAXSIZE)
					{
						str[ID_CURRENT_MAXSIZE] = '\0';
					}
					std::string scope;
					//если функция, то не учитываем скоуп
					if (IT::IsId(idtable, str) != TI_NULLIDX && IT::GetEntry(idtable, IT::IsId(idtable, str)).idtype == (IT::IDTYPE::F))
					{
						scope += str;
					}
					else {
						if (!isExported)
						{
							for (int j = 0; j < scopeStack.size(); j++) {
								scope = scope + scopeStack.at(j);
							}
						}
						scope += str;
					}
					//если нет такого итендфикатора
					if (IT::IsId(idtable, (char*)scope.c_str()) == TI_NULLIDX)
					{
						if (isExported) {

							bool isOk = false;
							libPos = 0;

							for (; libPos < libslen; libPos++) {
								if (libs[libPos].name == str)
								{
									isOk = true;
									break;
								}
							}
							if (!isOk)
								throw ERROR_THROW_IN(705, lex.line, lex.col)
							else {
								if (dataType != libs[libPos].retval)
									throw ERROR_THROW_IN(707, lex.line, lex.col)
									parmStackShouldWrite = true;
							}
						}
						//если не параметр и не объявлен
						if (type != (IT::IDTYPE::P) && !isDeclare && !isExported) {
							//то мы пытаемся работать с необъявленным идентификатором
							throw ERROR_THROW_IN(121, lex.line, lex.col)
						}
						if (dataType != (IT::IDDATATYPE)FALSYNUMBER && type != (IT::IDTYPE)FALSYNUMBER)
						{
							if (type == IT::F)
							{
								scopeName = str;
							}


							//добавляем в таблицу 
							IT::Entry idTableObject(lextable.size, scope.c_str(), dataType, type, isExported);
							IT::Add(idtable, idTableObject);
							lexTableObject.idxTI = idtable.size - 1;
							//снимаем флаг объявлённости и экспорта
							isDeclare = false;
							isExported = false;

							//записываем в стек параметры импортируемой функции
							if (parmStackShouldWrite && type == IT::P) {
								params.push(dataType); //проверка параметров экспортируемой функции
							}
							//зануляем тип и вид
							if (!isFunctionParam)
								type = (IT::IDTYPE)FALSYNUMBER;
							dataType = (IT::IDDATATYPE)FALSYNUMBER;
						}
						else //дропаем ошибку если тип или вид не определён
							throw ERROR_THROW_IN(122, lex.line, lex.col);
					}
					else {
						if (isDeclare || isExported)
							throw ERROR_THROW_IN(700, lex.line, lex.col);
						//если в таблице идентификаторов есть запись - ссылаемся на неё
						lexTableObject.idxTI = IT::IsId(idtable, (char*)scope.c_str());
					}
					break;
				}
						   //экспорт
				case LEX_EXPORT: {
					isExported = true;
					break;
				}
							   //левая скобка
				case LEX_LEFTHESIS: {
					//если предыдущий - имя функции
					if (lextable.size != 0
						&&
						lextable.table[lextable.size - 1].idxTI != LT_TI_NULLIDX
						&&
						idtable.table[lextable.table[lextable.size - 1].idxTI].idtype == IT::F)
					{
						//если функцию только объявили
						if (lextable.table[lextable.size - 4].lexema == LEX_EXPORT || lextable.table[lextable.size - 4].lexema == LEX_DECLARE)
						{
							isFunctionParam = true;
							type = IT::P;
							scopeStack.push_back(scopeName);
						}
						//если функцию вызвали
						else {
							isFunctionParam = false;
						}
					}
					if (lextable.size != 0
						&&
						lextable.table[lextable.size - 1].idxTI != LT_TI_NULLIDX
						&&
						idtable.table[lextable.table[lextable.size - 1].idxTI].idtype != IT::F)
						throw ERROR_THROW_IN(602, lex.line, lex.col)

						break;
				}
								  //правая скобочка
				case LEX_RIGHTHESIS: {
					if (isBoleanExpression) {
						isBoleanExpression = false;
					}
					if (parmStackShouldWrite)
					{
						int counter = 0;
						for (int q = libs[libPos].parcount - 1; q >= 0; q--)
						{
							if (params.size() != libs[libPos].parcount - counter)
								throw ERROR_THROW_IN(706, lex.line, lex.col)
								if (params.top() != libs[libPos].params[q])
									throw ERROR_THROW_IN(706, lex.line, lex.col)

									counter++;
							params.pop();
						}



						parmStackShouldWrite = false;
					}


					//если параметры функции шли
					if (isFunctionParam)
					{
						//убираем область видимости
						scopeStack.pop_back();
						//больше не параметры функции
						isFunctionParam = false;
						//закончили смотреть параметры, сейчас ожидаем тело функции
						isRequireBodyFunc = true;
						//тип больше не параметр
						type = (IT::IDTYPE)FALSYNUMBER;


					}
					//аргументы функции, то что передаётся в объявленную
					else
						type = (IT::IDTYPE)FALSYNUMBER;
					break;
				}
								   //левая фигурная
				case LEX_LEFTBRACE: {
					hesisCounter++;
					//если предыдущая лексема закрытсая скобочка и ожидаем тело функции
					if (lextable.table[lextable.size - 1].lexema == LEX_RIGHTHESIS && isRequireBodyFunc)
						scopeStack.push_back(scopeName.c_str());
					//если main
					else if (lextable.table[lextable.size - 1].lexema == LEX_MAIN)
						scopeStack.push_back("Main");
					//не будет тела функции
					else
						isRequireBodyFunc = false;
					break;
				}
								  //правая фигурная
				case LEX_RIGHTBRACE: {
					hesisCounter--;
					if (isRequireBodyFunc)
					{
						scopeStack.pop_back();
						isRequireBodyFunc = false;
					}
					//else
						//throw ERROR_THROW_IN(127, lex.line, lex.col);
					break;
				}
								   //main
				case LEX_MAIN: {
					if (!wasMain) {
						//ожидеам тело функции
						isRequireBodyFunc = true;
						//встречался ли main
						wasMain = true;
						break;
					}
					else
						throw ERROR_THROW_IN(125, lex.line, lex.col);
				}
							 //литерал
				case LEX_LITERAL: {
					//количество литералов
					literalCount++;
					lexTableObject.idxTI = idtable.size;




					bool continueFlag = true;

					if (checkArr[j].iddatatype == IT::INT)
					{
						unsigned int out = strtoul(str, NULL, 10);
						int positionInTable = IT::IsLX(idtable, checkArr[j].iddatatype, out);
						if (positionInTable != TI_NULLIDX)
						{
							continueFlag = false;
							lexTableObject.idxTI = positionInTable;
						}
					}
					if (checkArr[j].iddatatype == IT::CHR)
					{
						int positionInTable = IT::IsLX(idtable, checkArr[j].iddatatype, str[1]);
						if (positionInTable != TI_NULLIDX)
						{
							continueFlag = false;
							lexTableObject.idxTI = positionInTable;
						}
					}
					if (checkArr[j].iddatatype == IT::STR)
					{

						int positionInTable = IT::IsLX(idtable, IT::STR, str);
						if (positionInTable != TI_NULLIDX)
						{
							continueFlag = false;
							lexTableObject.idxTI = positionInTable;
						}
					}

					//если ретурним значит определили
					if (lextable.table[lextable.size - 1].lexema == LEX_RETURN)
					{
						continueFlag = true;
						lexTableObject.idxTI = idtable.size;
					}
					//если такого литерала нет
					if (continueFlag)
					{
						//формируем имя литерала
						std::string scope;
						for (int j = scopeStack.size() - 1; j >= 0; j--) {
							scope = scope + scopeStack.at(j);
						}
						scope += "$LEX";
						char* t = new char[6];
						_itoa_s(literalCount, t, 6, 10);
						scope += t;

						if (checkArr[j].iddatatype == IT::INT)
						{
							unsigned int out = strtoul(str, NULL, 10);
							IT::Entry ttmp(lextable.size - 1, scope.c_str(), checkArr[j].iddatatype, IT::L, out);
							IT::Add(idtable, ttmp);
						}
						else if (checkArr[j].iddatatype == IT::STR)
						{
							IT::Entry ttmp(lextable.size - 1, scope.c_str(), checkArr[j].iddatatype, IT::L, str);
							IT::Add(idtable, ttmp);
						}
						else if (checkArr[j].iddatatype == IT::CHR)
						{
							IT::Entry ttmp(lextable.size - 1, scope.c_str(), checkArr[j].iddatatype, IT::L, (char)str[1]);
							IT::Add(idtable, ttmp);
						}

						else ERROR_THROW_IN(122, lex.line, lex.col);
					}
					break;
				}


				}
#pragma region "Если знаковая лексема"
				if (lexTableObject.lexema == LEX_MINUS || lexTableObject.lexema == LEX_LEFTBRACE || lexTableObject.lexema == LEX_RIGHTBRACE || lexTableObject.lexema == LEX_LEFTHESIS || lexTableObject.lexema == LEX_RIGHTHESIS)
					lexTableObject.data = str[0];
#pragma endregion
#pragma region "Если булевая лексема"
				if (lexTableObject.lexema == LEX_BOOL_OPERATOR)
					lexTableObject.data = str[0];
#pragma endregion

#pragma endregion
				LT::Add(lextable, lexTableObject);
				break;
			}
		}
		//если не распознали цепочку
		if (!executedFlag)
			throw ERROR_THROW_IN(120, lex.line, lex.col);
	}
	//если не было main
	if (!wasMain)
		throw ERROR_THROW(124);
	// если не совпадают скобки
	if (hesisCounter != 0)
		throw ERROR_THROW(116);
#pragma endregion
}
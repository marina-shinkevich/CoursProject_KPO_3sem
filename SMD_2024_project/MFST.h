#include <stack>
#include "GRB.h"
#include "LT.h"
#include "Error.h"
#include "LexAnalysis.h"
#include "Log.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

template <typename T>
struct use_container : T
{
	using T::T;
	using T::c;
};

typedef use_container<std::stack<short>> MFSTSTACK; //MFSTSTSTACK;

//typedef std::stack<short> MFSTSTACK;	//СТЕК АВТОМАТА
namespace MFST
{
	struct MfstState		//состояние автомата
	{
		short lenta_position;	//позиция на ленте
		short nrule;			//номер текущего правила
		short nrulechain;		//Номер текущей цепочки
		MFSTSTACK st;			 //стек автомата
		MfstState();
		MfstState(
			short pposition,	//позиция на ленте
			MFSTSTACK pst,		//стек автомата
			short pnrulechain);	//номер текущей цепочки, текущего правила
		MfstState(
			short pposition,	//позиция на ленте
			MFSTSTACK pst,		//стек автомата
			short pnrule,		//номер текущего правила
			short pnrulechain	//номер текущей цепокаи, текущего правила
		);
	};
	struct Mfst					//магазинный автомат
	{
		enum RC_STEP {			//код возврата функции step
			NS_OK,				//правило найдено и записано в стек
			NS_NORULE,			//не найдено правило в грамматике
			NS_NORULECHAIN,		//не найдена подходящая цепочка(в исходном коде ошибка)
			NS_ERROR,			//неизвестный нетерминальный символ грамматики
			TS_OK,				//тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,				//тек символ != вершине стека, восстановление состояния
			LENTA_END,			//текущая позиция >= LentaSize
			SURPRISE			//неожиданность
		};
		struct MfstDiagnosis	//диагностика
		{
			short lenta_position;	//позиция на ленте
			RC_STEP rc_step;		//код завершения шага
			short nrule;			//номер правила
			short nrule_chain;		//номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis(			//диагностика
				short plenta_position,	//позиция на ленте
				RC_STEP prc_step,		//код завершения шага
				short pnrule,			//номер правила
				short pnrule_chain		//номер цепочки правила
			);
		} diagnosis[MFST_DIAGN_NUMBER];	//последние самые глубокие сообщения??

		GRBALPHABET* lenta;				//перекодированная лента из lex
		short lenta_position;			//текущая позиция на ленте
		short nrule;					//номер текущего правила
		short nrulechain;				//номер текущей цепочки, текущего правила
		short lenta_size;				//размер ленты
		GRB::Greibach greibach;			//грамматика Грейбах
		LEX::LEX lex;				//результат работы лексического анализатора
		MFSTSTACK st;					//стек автомата
		bool shallWrite;
		use_container<std::stack<MfstState>> storestate;	//стек сохранения состояний
		Mfst();
		Mfst(
			LEX::LEX plex,				//результат работы лексического анализатора
			GRB::Greibach pgreibach,		//грамматика грейбах
			bool shouldWrite
		);
		char* getCSt(char* buf);		//получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25);		//лента: n символов с pos
		char* getDiagnosis(short n, char* buf);	//получить n-ю сумму диагностики или 0х00
		bool savestate(const Log::LOG& log);						//сохранить состояние автомата
		bool reststate(const Log::LOG& log);						//восстановитьсостояние автомата
		bool push_chain(						//поместить цепочку правила в стек
			GRB::Rule::Chain chain			//цепочка правила
		);
		RC_STEP step(const Log::LOG& log);						//выполнить шаг автомата
		bool start(const Log::LOG& log);						//запустить автомат
		bool savediagnosis(
			RC_STEP pprc_step				//код завершения шага
		);
		void printrules(const Log::LOG& lo);					//вывести последовательность правил

		struct Deducation					//вывод
		{
			short size;						//кол-во шагов в выводе
			short* nrules;					//номера правил в грамматике
			short* nrulechains;				//номера цепочек правил грамматики
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		} deducation;

		bool savededucation();				//сохранить дерево вывода
	};
}
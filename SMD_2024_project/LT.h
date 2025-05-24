#pragma once
#define LEXEMA_FIXSIZE 1
#define LT_MAXSIZE 4096
#define LT_TI_NULLIDX 0XFFFFFFFF
#define LEX_INTEGER 't'
#define LEX_STRING	't'
#define LEX_CHAR	't' 
#define LEX_TYPE	LEX_STRING //лексема типа(uint/symbol)
#define LEX_ID		'i'
#define LEX_UNTIL		'u'
#define LEX_IF		'o'
#define LEX_MAIN	'm'
#define LEX_LITERAL	'l'
#define LEX_FUNCTION 'f'
#define LEX_DECLARE	'd'
#define LEX_RETURN	'r'
#define LEX_EXPORT	'e'
#define LEX_PRINT	'p'
#define LEX_SEMICOLON ';'
#define LEX_COMMA	','
#define LEX_LEFTBRACE '{'
#define LEX_RIGHTBRACE	'}'

#define LEX_LEFT_SQUAREBRACE '['
#define LEX_RIGHT_SQUAREBRACE	']'

#define LEX_LEFTHESIS	'('
#define LEX_RIGHTHESIS	')'
#define LEX_PLUS		'v'
#define LEX_MINUS		'v'
#define LEX_STAR		'v'
#define LEX_DIRSLASH	'v'
#define LEX_EQUAL		'v'
#define LEX_OPERATOR	'v'
#define LEX_BOOL_OPERATOR 'b'
#define EMPTY_DATA		'\0'
#define SLUSH_DATA		'\n'

namespace LT
{
	struct Entry //строка таблицы лексем
	{
		char lexema;//лексема
		int sn; //номер строки в исходном тексте
		int cn; //номер столбца в исходном тексте
		int idxTI;	//индекс идентификатора в таблице
		char data;

		Entry(
			char lexema,
			int sn, //номер строки в исходном тексте
			int cn, //номер столбца в исходном тексте
			int idxTI,	//индекс идентификатора в таблице
			char symbol = EMPTY_DATA
		) {
			this->lexema = lexema;
			this->sn = sn;
			this->cn = cn;
			this->idxTI = idxTI;
			this->data = data;
		};

		Entry() = default;
	};
	struct LexTable //экземпл€р таблицы лексем
	{
		int maxsize; //емкость <LT_MAXSIZE
		int size; //текущий размер таблицы лексем
		Entry* table;	//массив строк
	};
	LexTable Create(//создать таблицу лексем
		int size//емкость таблицы лексем < ƒ≈_№‘„џЎя”
	);
	void Add(	//добавить строку в таблицу лексем
		LexTable& lextable,		//экземпл€р таблицы лексем
		Entry entry	//строка таблицы лексем
	);
	Entry GetEntry(	//получить строку таблицы лексем
		LexTable& lextable, //экземпл€р
		int n //номер получаемой строки
	);


	void Delete(LexTable& lextable);	//удалить таблицу лексем
}
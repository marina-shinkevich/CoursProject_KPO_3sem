#pragma once

#define ID_MAXSIZE		26			//макс кол-во символов в идентиф
#define ID_CURRENT_MAXSIZE		20 //макс кол-во символов в идентиф

#define	TI_MAXSIZE		4096 //макс кол-во строк в табл идентификаторов
#define TI_INT_DEFAULT	0x00000000	//значение int по дефолту
#define	TI_STR_DEFAULT	0x00		//значение string по дефолту
#define	TI_NULLIDX		0xffffffff	//нет элемента таблицы идентификаторов
#define	TI_STR_MAXSIZE	255

#define FALSYNUMBER 17	//используй как нуль

namespace IT	//таблица идентификаторов
{
	enum IDDATATYPE { INT = 1, STR = 2, CHR = 3, BOOL = 4 };//таблица данных идентификаторов fls - empty
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };//типы идентификаторов - переменная функция параметр литерал

	struct Entry //строка таблицы идентификаторов
	{
		int idxfirstLE; //индекс первой строки в таблице лексем
		char id[ID_MAXSIZE]; //идентификатор
		bool isExternal; //флаг подключения
		IDDATATYPE iddatatype; //тип данных
		IDTYPE idtype; //тип идентификатора
		union {
			unsigned int vint; //значение integer
			char vchar;//значение sting
			struct
			{
				char len;//кол-во символов в string
				char* str;// [TI_STR_MAXSIZE] ;//символы стринг
			} vstr;//значение sting
		} value; //значение идентификатора
		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;
			if (iddatatype == CHR)
			{
				this->value.vchar = '\0';
			}
			if (iddatatype == INT)
			{
				this->value.vint = 0;
			}
			if (iddatatype == STR)
			{
				this->value.vstr.len = 0;
				this->value.vstr.str = nullptr;
			}


#pragma region "Writedown id name"
			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}
#pragma endregion

		};
		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, unsigned int data, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;
			this->value.vint = data;
#pragma region "Writedown id name"
			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}
#pragma endregion

		};
		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, char data, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;
#pragma region "Writedown id name"
			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}
#pragma endregion

			this->value.vchar = data;
			//if (len > TI_STR_MAXSIZE)
			//{
			//	//error
			//	//std::cout << "TI STR MAXSIZE ERROR" << std::endl;
			//}
			//else
			//{
			//	//this->value.vstr.str = new char[len];
			//	for (int i = 1; i < len; i++)
			//	{
			//		this->value.vstr.str[i - 1] = data[i];
			//	}
			//	this->value.vstr.str[len - 1] = '\0';
			//}
		};
		Entry(int idxfirstLE, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, char* data, bool e = false) {
			this->isExternal = e;
			this->idxfirstLE = idxfirstLE;
			this->iddatatype = iddatatype;
			this->idtype = idtype;
#pragma region "Writedown id name"
			int len = 0;
			for (int i = 0; id[i] != '\0' && i < ID_MAXSIZE; i++)
			{
				len++;
				this->id[i] = id[i];
			}
			if (len >= ID_MAXSIZE)
				this->id[ID_MAXSIZE - 1] = '\0';
			else
			{
				this->id[len] = '\0';
			}
#pragma endregion

			len = 0;
			for (int i = 1; data[i] != '\"'; i++)
			{
				len++;
			}
			this->value.vstr.len = len + 3;
			this->value.vstr.str = new char[len + 3];

			for (int i = 0; i < len + 3; i++)
			{
				this->value.vstr.str[i] = data[i];
			}
			this->value.vstr.str[len + 3 - 1] = '\0';
			this->value.vstr.len = len + 2;

		};
		Entry() = default;
	};
	struct IdTable		//экземпляр таблицы ид
	{
		int maxsize;	//ёмкость таблицы
		int size;		//текущий размер
		Entry* table;	//массив строк таблицы идентификаторов
	};
	IdTable Create(	//создать таблицу ID
		int size	//ёмкость
	);
	void Add(
		IdTable& idtable,	//экземпляр таблицы идентификаторов
		Entry entry			//строка таблицы идентификаторов
	);
	Entry GetEntry(	//получить строку таблицы идентификаторов
		IdTable& idtable,	//экземпляр таблицы идентификаторов
		int n				//строка таблицы идентификаторов
	);
	int IsId(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		char id[ID_MAXSIZE] //идентификатор
	);
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		unsigned int data
	);
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		char data
	);
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		char* data
	);

	bool isUniq(IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		char id[ID_MAXSIZE]);
	void Delete(IdTable& idtable);	//удалить таблицу идентификаторов
}
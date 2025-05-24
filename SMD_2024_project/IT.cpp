#include "IT.h"
#include <fstream>
#include <iostream>
#include <cstdarg>
#include "Error.h"



namespace IT
{
	IdTable Create(	//создать таблицу ID
		int size	//Ємкость
	) {
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(130);
		}
		IdTable out;
		out.maxsize = size;
		out.size = 0;
		out.table = new Entry[size];
		return out;
	};
	void Add(
		IdTable& idtable,	//экземпл€р таблицы идентификаторов
		Entry entry			//строка таблицы идентификаторов
	) {
		if (idtable.size + 1 > idtable.maxsize)
			throw ERROR_THROW(133);
		idtable.table[idtable.size++] = entry;
		
	};
	Entry GetEntry(	//получить строку таблицы идентификаторов
		IdTable& idtable,	//экземпл€р таблицы идентификаторов
		int n				//строка таблицы идентификаторов
	) {
		if (idtable.size + 1 > idtable.maxsize)
			throw ERROR_THROW(134);
		//add error with n
		return idtable.table[n];
	};
	int IsId(
		IdTable& idtable,	
		char id[ID_MAXSIZE] //идентификатор
	) {
		//ѕровер€ет, существует ли идентификатор в таблице, и возвращает его индекс.
		int flag = -1;
		bool bflag = false;
		for (int i = 0; i < idtable.size; i++)
		{
			if (strlen(id) != strlen(idtable.table[i].id))
				continue;
			for (int j = 0; j < strlen(id); j++)
			{
				if (idtable.table[i].id[j] != id[j])
				{
					bflag = false;
					break;
				}
				else {
					bflag = true;
				}
			}
			if (bflag)
				return i;
		}
		return TI_NULLIDX;
	};
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		unsigned int data
	) {
		int flag = -1;
		bool bflag = false;
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype != IT::L || idtable.table[i].iddatatype != type
				||
				data != idtable.table[i].value.vint)
				continue;
			else
				return i;
		}
		return TI_NULLIDX;
	};
	int IsLX(
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLID(если нет)
		IDDATATYPE type,
		char data
	) {
		int flag = -1;
		bool bflag = false;
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].iddatatype != type
				||
				data != idtable.table[i].value.vchar)
				continue;
			else
				return i;
		}
		return TI_NULLIDX;
	};
	int IsLX(//существует ли идентификатор с определенным типом данных
		IdTable& idtable,	
		IDDATATYPE type,
		char* data
	) {
		int len = 0;
		while (true) {
			if (data[len] == '\0')
				break;
			else
				len++;
		}
		int flag = -1;
		bool bflag = false;
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].iddatatype != type)
				continue;
			if (len != idtable.table[i].value.vstr.len)
				continue;
			else
			{
				bool f = false;
				for (int j = 0; j < len; j++) {
					if (idtable.table[i].value.vstr.str[j] != data[j])
					{
						f = false;
						break;
					}
					else
						f = true;

				}
				if (f)
					return i;
			}

		}
		return TI_NULLIDX;
	};





	bool isUniq( //уникален ли идентификатор в таблице
		IdTable& idtable,	//возврат: номер строки(если есть) TI_NULLIDX(если нет)
		char id[ID_MAXSIZE] //идентификатор
	) {
		bool bflag = false;
		int isQ = 0;
		for (int i = 0; i < idtable.size; i++)
		{
			if (strlen(id) != strlen(idtable.table[i].id))
				continue;
			for (int j = 0; j < strlen(id); j++)
			{
				if (idtable.table[i].id[j] != id[j])
				{
					bflag = false;
					break;
				}
				else {
					bflag = true;
				}
			}
			if (bflag)
				isQ++;
		}
		return isQ == 1;
	};


	void Delete(IdTable& idtable) {
		delete& idtable;
	};

}
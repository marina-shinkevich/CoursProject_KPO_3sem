#pragma once

#define ID_MAXSIZE		26			//���� ���-�� �������� � �������
#define ID_CURRENT_MAXSIZE		20 //���� ���-�� �������� � �������

#define	TI_MAXSIZE		4096 //���� ���-�� ����� � ���� ���������������
#define TI_INT_DEFAULT	0x00000000	//�������� int �� �������
#define	TI_STR_DEFAULT	0x00		//�������� string �� �������
#define	TI_NULLIDX		0xffffffff	//��� �������� ������� ���������������
#define	TI_STR_MAXSIZE	255

#define FALSYNUMBER 17	//��������� ��� ����

namespace IT	//������� ���������������
{
	enum IDDATATYPE { INT = 1, STR = 2, CHR = 3, BOOL = 4 };//������� ������ ��������������� fls - empty
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };//���� ��������������� - ���������� ������� �������� �������

	struct Entry //������ ������� ���������������
	{
		int idxfirstLE; //������ ������ ������ � ������� ������
		char id[ID_MAXSIZE]; //�������������
		bool isExternal; //���� �����������
		IDDATATYPE iddatatype; //��� ������
		IDTYPE idtype; //��� ��������������
		union {
			unsigned int vint; //�������� integer
			char vchar;//�������� sting
			struct
			{
				char len;//���-�� �������� � string
				char* str;// [TI_STR_MAXSIZE] ;//������� ������
			} vstr;//�������� sting
		} value; //�������� ��������������
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
	struct IdTable		//��������� ������� ��
	{
		int maxsize;	//������� �������
		int size;		//������� ������
		Entry* table;	//������ ����� ������� ���������������
	};
	IdTable Create(	//������� ������� ID
		int size	//�������
	);
	void Add(
		IdTable& idtable,	//��������� ������� ���������������
		Entry entry			//������ ������� ���������������
	);
	Entry GetEntry(	//�������� ������ ������� ���������������
		IdTable& idtable,	//��������� ������� ���������������
		int n				//������ ������� ���������������
	);
	int IsId(
		IdTable& idtable,	//�������: ����� ������(���� ����) TI_NULLID(���� ���)
		char id[ID_MAXSIZE] //�������������
	);
	int IsLX(
		IdTable& idtable,	//�������: ����� ������(���� ����) TI_NULLID(���� ���)
		IDDATATYPE type,
		unsigned int data
	);
	int IsLX(
		IdTable& idtable,	//�������: ����� ������(���� ����) TI_NULLID(���� ���)
		IDDATATYPE type,
		char data
	);
	int IsLX(
		IdTable& idtable,	//�������: ����� ������(���� ����) TI_NULLID(���� ���)
		IDDATATYPE type,
		char* data
	);

	bool isUniq(IdTable& idtable,	//�������: ����� ������(���� ����) TI_NULLID(���� ���)
		char id[ID_MAXSIZE]);
	void Delete(IdTable& idtable);	//������� ������� ���������������
}
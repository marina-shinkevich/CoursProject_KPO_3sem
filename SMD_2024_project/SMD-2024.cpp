#include "tchar.h"
#include <iostream>
#include <locale>
#include <cwchar>
#include "MFST.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "FST.h"
#include "IT.h"
#include "SemAnalysis.h"
#include "PolishNotation.h"
#include "CodeGenerator.h"
#include "LexAnalysis.h"



int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "rus");

    // ���������� ���������� ��������� � argv
    //_TCHAR* customArgv[] = {
    //    (_TCHAR*)L"-in:C:\\Users\\User\\Documents\\2����1���\\SMD-2024\\SMD_2024_project\\SMD_2024_project\\in.txt"
    //   // ���� ��� ��������� ������ ������ �������
    //};

    // ����� ����������
    //argc = 1;  // �� ������ ��� ���������: ���� � ����� � ���� -t
    //argv = customArgv;  // �������� � ��������� �������� ���������

    Log::LOG log = Log::INITLOG;
    Out::OUT out = Out::INITOUT;
    Parm::PARM parm;


    try {
        // �������� ��������� ���������
        parm = Parm::getparm(argc, argv);

        // �������������� ���� � �������� �����
        log = Log::getlog(parm.log);
        out = Out::getout(parm.out);

        // ���������� ��� � ���������
        Log::WriteLog(log);
        Log::WriteParm(log, parm);

        // ������ ������� ������
        In::IN in = In::getin(parm.in);
        Log::WriteIn(log, in);
      

        // ����������� ������
        LEX::LEX tables(LT::Create(in.lexems.size()), IT::Create(in.lexems.size()));
        FST::LexAnalyzer(in, out, log, tables.lextable, tables.idtable);
      /*  Log::LogTables(log, tables);*/
      //  Log::LogIDTables(log, tables);

      
        Log::WriteLine(log, "--------����� ������������ �������--------", "");

        // �������������� ������
        Log::WriteLine(log, "--------������ ��������������� �������--------", "");
    /*   parm.showMfst = true;*/

        if (parm.showMfst) {
            MFST_TRACE_START(log);
        }

        MFST::Mfst mfst(tables, GRB::getGreibach(), parm.showMfst);
        if (!mfst.start(log)) {
            throw ERROR_THROW(609); // ������ ��������������� �������
        }

        // ����� ������ ������� � ���, ���� ����������� ���� `-t`
        if (parm.showMfst) {
           
           
            mfst.printrules(log);  // ����� ������ � ���
        }

        mfst.savededucation();

        // ���������� ������������� ������
        Log::WriteLine(log, "--------����� ��������������� �������--------", "");

        Log::WriteLine(log, "--------������ �������������� �������--------", "");
		//������������� ������
		Semantic::doAnalyse(tables);
		Log::WriteLine(log, "--------����� �������������� �������--------", "");

		//�������� ������
        PolishNotation::DoPolish(tables, "output_lextable.txt");
        

		Log::WriteLine(log, "--------��� ����������� ���������� ��� ������--------", "");

        // ����������� ������ ������ � ���������������
       

        //��������� ����
        CG::Generate(tables, out);
        Log::WriteLine(log, "--------��� ������� ������������--------", "");
       

    }
    catch (const Error::ERROR& e) {
        // ��������� ������
        Log::WriteError(log, e);
    }

    // �������� ����� � �������� ������
    Log::Close(log);
    Out::Close(out);



    return 0;
}
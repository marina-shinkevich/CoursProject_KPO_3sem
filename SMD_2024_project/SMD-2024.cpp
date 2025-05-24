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

    // Исправляем количество элементов в argv
    //_TCHAR* customArgv[] = {
    //    (_TCHAR*)L"-in:C:\\Users\\User\\Documents\\2курс1сем\\SMD-2024\\SMD_2024_project\\SMD_2024_project\\in.txt"
    //   // Флаг для включения вывода дерева разбора
    //};

    // Число аргументов
    //argc = 1;  // Мы задали два аргумента: путь к файлу и флаг -t
    //argv = customArgv;  // Передаем в программу тестовые аргументы

    Log::LOG log = Log::INITLOG;
    Out::OUT out = Out::INITOUT;
    Parm::PARM parm;


    try {
        // Получаем параметры программы
        parm = Parm::getparm(argc, argv);

        // Инициализируем логи и выходные файлы
        log = Log::getlog(parm.log);
        out = Out::getout(parm.out);

        // Записываем лог и параметры
        Log::WriteLog(log);
        Log::WriteParm(log, parm);

        // Чтение входных данных
        In::IN in = In::getin(parm.in);
        Log::WriteIn(log, in);
      

        // Лексический анализ
        LEX::LEX tables(LT::Create(in.lexems.size()), IT::Create(in.lexems.size()));
        FST::LexAnalyzer(in, out, log, tables.lextable, tables.idtable);
      /*  Log::LogTables(log, tables);*/
      //  Log::LogIDTables(log, tables);

      
        Log::WriteLine(log, "--------Конец лексического анализа--------", "");

        // Синтаксический анализ
        Log::WriteLine(log, "--------Начало синтаксического анализа--------", "");
    /*   parm.showMfst = true;*/

        if (parm.showMfst) {
            MFST_TRACE_START(log);
        }

        MFST::Mfst mfst(tables, GRB::getGreibach(), parm.showMfst);
        if (!mfst.start(log)) {
            throw ERROR_THROW(609); // Ошибка синтаксического анализа
        }

        // Вывод дерева разбора в лог, если активирован флаг `-t`
        if (parm.showMfst) {
           
           
            mfst.printrules(log);  // Вывод дерева в лог
        }

        mfst.savededucation();

        // Сохранение промежуточных данных
        Log::WriteLine(log, "--------Конец синтаксического анализа--------", "");

        Log::WriteLine(log, "--------Начало семантического анализа--------", "");
		//семантический анализ
		Semantic::doAnalyse(tables);
		Log::WriteLine(log, "--------Конец семантического анализа--------", "");

		//польская запись
        PolishNotation::DoPolish(tables, "output_lextable.txt");
        

		Log::WriteLine(log, "--------Все анализаторы отработали без ошибок--------", "");

        // Логирование таблиц лексем и идентификаторов
       

        //генерация кода
        CG::Generate(tables, out);
        Log::WriteLine(log, "--------Код успешно сгенерирован--------", "");
       

    }
    catch (const Error::ERROR& e) {
        // Обработка ошибок
        Log::WriteError(log, e);
    }

    // Закрытие логов и выходных файлов
    Log::Close(log);
    Out::Close(out);



    return 0;
}
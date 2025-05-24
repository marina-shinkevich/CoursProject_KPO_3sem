#include "tchar.h"
#include "Error.h"
#include "Parm.h"
#include <iostream>
#include <string.h>

namespace Parm
{
	Parm::PARM getparm(int argc, _TCHAR* argv[])
	{
		setlocale(LC_CTYPE, "Russian");
		Parm::PARM out;
		out.showMfst = false;
		out.showTables = false;
		out.showITables = false;
		out.in[0] = L'\0';
		out.out[0] = L'\0';
		out.log[0] = L'\0';
		const int empty_length = wcslen(out.log) + 1;
		wchar_t wst[100];
		for (int i = 0; i < argc; i++)
		{
			/*std::wcout << argv[i][0] << '\n';*/
			if (argv[i][0] == 't' && argv[i][1] == '\0') {
				out.showMfst = true;
			}
			if (argv[i][0] == 'l' && argv[i][1] == '\0') {
				out.showTables = true;
			}
			if (argv[i][0] == 'i' && argv[i][1] == '\0') {
				out.showITables = true;
			}




			if (wcslen(argv[i]) > PARM_MAX_SIZE)
				throw ERROR_THROW(104)
				const wchar_t* in = wcsstr(argv[i], PARM_IN);
			if (in != 0)
			{
				wcscpy_s(out.in, argv[i] + wcslen(PARM_IN));
			}
			const wchar_t* out_temp = wcsstr(argv[i], PARM_OUT);
			if (out_temp != 0)
			{
				wcscpy_s(out.out, argv[i] + wcslen(PARM_OUT));
			}
			const wchar_t* log = wcsstr(argv[i], PARM_LOG);
			if (log != 0)
			{
				wcscpy_s(out.log, argv[i] + wcslen(PARM_LOG));
			}
		}
		if (wcslen(out.in) == 0)
			throw ERROR_THROW(100)
		else
		{
			if (wcslen(out.out) == 0)
			{
				wchar_t a[PARM_MAX_SIZE];
				a[0] = '\0';
				wcsncat_s(a, out.in, wcslen(out.in));
				wcsncat_s(a, PARM_OUT_DEFAULT_EXT, wcslen(PARM_OUT_DEFAULT_EXT));
				wcscpy_s(out.out, a);
			}
			if (wcslen(out.log) == 0)
			{
				wchar_t a[PARM_MAX_SIZE];
				a[0] = '\0';
				wcsncat_s(a, out.in, wcslen(out.in));
				wcsncat_s(a, PARM_LOG_DEFAULT_EXT, wcslen(PARM_LOG_DEFAULT_EXT));
				wcscpy_s(out.log, a);
			}
		}
		return out;
	}
}
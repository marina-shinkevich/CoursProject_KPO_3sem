#pragma once

#include "Error.h"
#include "In.h"
#include "IT.h" 
#include "LT.h"

namespace LEX
{
	struct LEX
	{
		IT::IdTable  idtable;
		LT::LexTable lextable;

		LEX(
			LT::LexTable _lextable, IT::IdTable  _idtable) {
			idtable = _idtable;
			lextable = _lextable;
		}
		LEX() {}
	};
}
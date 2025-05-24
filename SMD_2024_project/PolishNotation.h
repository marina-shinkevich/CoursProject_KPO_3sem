#pragma once
#include "LexAnalysis.h"

namespace PolishNotation {
	bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void DoPolish(LEX::LEX t, const std::string& outputFilename);
	void OutputLexTableFormatted(const LT::LexTable& lextable, const std::string& filename);
}
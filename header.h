#pragma once
#include "abstract.h"


class Header :public Abstract_text {
private:
    Text text;
    int level;
public:
	Header(int _level) {text = Text(); level = _level;};
	Header(int _level, Text _text){level = _level; text = Text(_text); text.space_cleaner();};
	Header(int _level, const wchar_t *added) :Header(_level) { text+=added; text.space_cleaner();};
	unsigned long int count_symbols() const { return text.length();};
	unsigned long int count_words() const;
	int print(std::wostream& os, const Cfg_Attributes& attr) const;
	void addText(const wchar_t *added);
    int printflag() {return 2;};
};
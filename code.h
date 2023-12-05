#pragma once
#include "abstract.h"


class Code: public Abstract_text{
private:
    Text txt;   // текст неподготовлен!
public:
    Code() {txt = Text(L'\n', 1);};
    explicit Code(Text& text) {txt = Text(L"@") + text;};
    explicit Code(const wchar_t* add) {Text text(add); txt = Text(L"@") + text;}
    unsigned long int count_symbols() const {return txt.length();};
    unsigned long int count_words() const;
	int print(std::wostream& os, const Cfg_Attributes& attr) const;
	void addText(const wchar_t *added);
    int printflag() {return 1;};
};
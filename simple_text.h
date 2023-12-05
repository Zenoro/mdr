#pragma once
#include "abstract.h"


class Simple_Text:public Abstract_text{
private:
    Text txt;   //уже отформаченый текст!
public:
    Simple_Text() {txt = Text(L'\0', 1);};
    explicit Simple_Text(const Text& text) {txt = text;};
    unsigned long int count_symbols() const {return txt.length();};
    unsigned long int count_words() const;
	int print(std::wostream& os, const Cfg_Attributes& attr) const;
	void addText(const wchar_t *added);
    int printflag() {return 4;};
};

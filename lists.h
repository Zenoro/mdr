#pragma once
#include "abstract.h"

class Some_List:public Abstract_text {
    private:
        Text number;
        Text txt;
        int lvl;
        bool is_num;
    public:
        Some_List(Text txxt, int level): txt(txxt), lvl(level), is_num(false) {number = Text();};
        Some_List(const wchar_t* txxt, int level): txt(Text(txxt)), lvl(level), is_num(false) {number = Text();};
        Some_List(Text num, Text txxt, int level): number(num), txt(txxt), lvl(level), is_num(true) {};
        Some_List(const wchar_t* num, const wchar_t* txxt, int level): number(Text(num)), txt(Text(txxt)), lvl(level), is_num(true) {};
        unsigned long int count_symbols() const;
        unsigned long int count_words() const;
        int print(std::wostream& os, const Cfg_Attributes& attr) const;
        void addText(const wchar_t *added);
        int printflag() {return 3;};
};

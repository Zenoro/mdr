#pragma once
#include "text.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>

using namespace std;

class Cfg_Attributes {
    public:
    wchar_t marker;
    unsigned int terminal_w;
    unsigned int redstr_tab, extra_tab;
    bool is_in_file;
    Cfg_Attributes() {
        marker = '*'; redstr_tab = 4; extra_tab = 4; is_in_file = false;
        struct  winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        terminal_w = w.ws_col;
    };

    Cfg_Attributes(wchar_t m, unsigned int rst, unsigned int ltab, unsigned int etab, bool in_f){
        marker = m; redstr_tab = rst; extra_tab = etab; is_in_file = in_f;
        struct  winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        terminal_w = w.ws_col;
    };
};


class Abstract_text {
public:
	// virtual ~Abstract_text() {};
    virtual unsigned long int count_symbols() const = 0;
    virtual unsigned long int count_words() const = 0;
	virtual int print(std::wostream& os, const Cfg_Attributes& attr) const =0;
	virtual void addText(const wchar_t *added) =0;
    virtual int printflag() =0;
};

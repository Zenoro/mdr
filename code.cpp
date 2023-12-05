#include "code.h"
using namespace std;

unsigned long int Code::count_words() const{
    unsigned int res = 0;
    for (int i=0; i<txt.length(); i++)
        if (txt[i] == L' ')
            res++;
    return res;
};

int Code::print(std::wostream& os, const Cfg_Attributes& attr) const{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    Text buf(txt);
    buf.slice(txt.length() - attr.terminal_w);
    if (!attr.is_in_file)
        wprintf(L"\033[92m%ls\033[0m", buf.get_str());
    else
        os << buf.get_str();
    return 0;
};

void Code::addText(const wchar_t *added){
    txt+=added;
};

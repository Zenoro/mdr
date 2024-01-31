#include "code.h"
using namespace std;

unsigned long int Code::count_words() const{
    Text buf(txt);
    buf.space_cleaner();
    if (buf.is_empty()) {
        return 0;
    }
    else {
        unsigned long int res = 1;
        for (int i = 0; i < buf.length(); i++) {
            if (buf[i] == L' '){
                res++;
            }
        }
        return res;
    }
}

int Code::print(std::wostream& os, const Cfg_Attributes& attr) const{
    if (attr.terminal_w < 5) {
        cerr << "Too low terminal width (less than 5)! Can't show any code, exiting...\n";
        // delete this;
        return -1;
    }
    Text buf(txt);
    int len_of_code = txt.length() - attr.terminal_w;
    if (len_of_code > 0) { 
        buf.slice(len_of_code);
    }
    if (!attr.is_in_file) {
        wprintf(L"\033[92m%ls\033[0m", buf.get_str());
    }
    else {
        os << buf.get_str();
    }
    return 0;
}

void Code::addText(const wchar_t *added){
    txt+=added;
}

#include "lists.h"
using namespace std;

void Some_List::addText(const wchar_t *added) {
    txt+=added;
}

unsigned long int Some_List::count_symbols() const {
    return txt.length();
}

unsigned long int Some_List::count_words() const {
    Text buf(txt);
    buf.space_cleaner();
    if (buf.is_empty()) {
        return 0;
    }
    else {
        unsigned long int res = 1;
        for (int i=0; i < buf.length(); i++) {
            if (buf[i] == L' ') {
                res++;
            }
        }
        return res;
    }
}

int Some_List::print(std::wostream& os, const Cfg_Attributes& attr) const {
    Text tochka;
    Text nnumber = number;
    if (nnumber.is_empty()) {
        tochka = Text(attr.marker, 1);
        tochka += L' ';
    }
    else {
        tochka = number;
    }
    int txtlen = txt.length(); int tochkalen = tochka.length();
    int tabchik = 2*lvl + 2;
    if (tabchik + tochkalen + txtlen > attr.terminal_w) {
        if (tabchik + tochkalen + 5 > attr.terminal_w) {
            cerr << "\nToo low terminal width! Can't show text cause it\'s lower \nthan marker + tab lenghts with 3 symbols!\n";
            cerr << "Teminal width: " << attr.terminal_w << "\nMarker + tabulation lenght + minimal size of text: " << tabchik + tochkalen + 4 << '\n';
            return -1;
        }
        else {
            os << Text(L' ', tabchik);
            if (is_num && !attr.is_in_file) {
                wprintf(L"\033[94m%ls\033[0m", tochka.get_str());
            }
            else
                os << tochka;
            // вывод текста
            int cnt = attr.terminal_w - tabchik - tochkalen; // первое значение количества доступных символов на строке
            int wrdlen = 0; // длина слова
            Text tabul(L' ', tabchik + attr.extra_tab);   // пробелы перед переносом строки
            Text word;
            Text txxt = txt + Text(L" ");
            for (int ind=0; ind<txxt.length(); ind++)
                if (!isspace(txxt[ind]))
                    word += txxt[ind];
                else {
                    int wrdlen = word.length();
                    if (cnt - wrdlen - 1 >= 0) {    // слово влезло в строку c пробелом
                        os << word << L" ";
                        word = Text();
                        cnt = cnt - wrdlen - 1;
                    }
                    else if (cnt - wrdlen == 0) { // слово влезло в строку
                        os << word << endl;
                        os << tabul;
                        word = Text();
                        cnt = attr.terminal_w - tabul.length();
                    }
                    else if ((cnt - wrdlen == -1) || (cnt <= 2)) {    // перенос слова с 1 символом на строках
                        os << L"\n";
                        os << tabul << word << L" ";
                        word = Text();
                        cnt = attr.terminal_w - tabul.length()- wrdlen - 1;
                    }
                    else {
                        Text online=word, afterline=word;
                        online.slice(0, cnt-1);
                        online += L"-";
                        os << online;
                        os << L'\n' << tabul;
                        afterline.slice(cnt-1, afterline.length());
                        if (afterline.is_empty()){
                            cnt = attr.terminal_w - tabul.length();
                        }
                        else {
                            os << afterline << L" ";
                            cnt = attr.terminal_w - tabul.length() - afterline.length() - 1;
                        }
                        word = Text();
                    }
                }
            return 0;
        }
    }
    else {
        os<< Text(L' ', tabchik); 
        if (is_num && !attr.is_in_file){
            wprintf(L"\033[94m%ls\033[0m", tochka.get_str());
        }
        else{
            os << tochka;
        }
        os << txt;
        return 0;
    }
}

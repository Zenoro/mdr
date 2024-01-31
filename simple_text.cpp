#include "simple_text.h"
using namespace std;

void Error_msg(int terminal_length) {
    wcout << L'\n' << Text('-', terminal_length)<< L'\n';
    cerr << "Too low terminal width! Word length is more than terminal width\n";
    cerr << "Terminal length: " << terminal_length << '\n';
}


unsigned long int Simple_Text::count_words() const{
	Text buf(txt);
	buf.space_cleaner();
	if (buf.is_empty()){
		return 0;
	}
	else {
		unsigned long int res = 1;
		for (int i = 0; i < buf.length(); i++){
			if (buf[i] == L' ') {
				res++;
			}
		}
		return res;
	}
}

int Simple_Text::print(std::wostream& os, const Cfg_Attributes& attr) const{
    int fst_str_flg = 0, cnt;
    if (attr.terminal_w < attr.redstr_tab + 4) {
        Error_msg(attr.terminal_w);
        return -1;
    }
    else {
        if (txt[0] != L'\n'){
            os << Text(L' ', attr.redstr_tab);
            cnt = attr.terminal_w - attr.redstr_tab;
        }
        else if (txt[0] == L'\n') {
            os << txt[0];
            return 4;
        }
        Text word;
        Text txtt = txt + Text(L" "); 
        for (int i=0; i<txtt.length(); i++) {
            if (!isspace(txtt[i])) {
                word += Text(txtt[i], 1);
            }
            else {
                int wrdlen = word.length();
                if (wrdlen > attr.terminal_w || cnt < 0) {
                    Error_msg(attr.terminal_w);
                    return -1;
                }
                else if (txtt[i] == L'\n'){
                    os << txtt[i];
                }
                else if (cnt - wrdlen - 1 >= 0) {    // слово нормально вмещается с пробелом
                    os << word;
                    os << L" ";
                    word = Text();
                    cnt = cnt - wrdlen - 1;

                }
                else if (cnt-wrdlen == 0) { // слово влезло в строку
                    os << word << endl;
                    word = Text();
                    cnt = attr.terminal_w;
                }
                else if ((cnt - wrdlen == -1) || (cnt <= 2)) {    // один символ в переносной или на предыдущей строке
                    os << L'\n';
                    os << word << L" ";
                    word = Text();
                    cnt = attr.terminal_w - wrdlen - 1;
                }
                else {  // дефолтный перенос слов на разные строки
                    Text online=word, afterline=word;
                    online.slice(0, cnt-1);
                    online += L"-";
                    os << online;
                    os << L'\n';
                    afterline.slice(cnt-1, afterline.length());
                    if (afterline.is_empty()) {
                        cnt = attr.terminal_w;
                    }
                    else {
                        os << afterline << L" ";
                        cnt = attr.terminal_w - afterline.length() - 1;
                    }
                    word = Text();
                }
            }
        }
        return 4;
    }
}

void Simple_Text::addText(const wchar_t *added){
    Text h(added);
    h.space_cleaner();
    txt+=L' ';
    txt+=h;
}

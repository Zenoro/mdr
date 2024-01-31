#include "header.h"
using namespace std;

int Header::print(std::wostream& os, const Cfg_Attributes& attr) const {
	int zfill = (attr.terminal_w - 2*level - count_symbols()) / 2;
	if (attr.terminal_w > count_symbols() + level) {
		Text cuphead(L'#', attr.terminal_w);
		if (level > 0){
			os << endl;
		}
		os << cuphead << endl;
		if (level > 0) {
			os << Text(L'#', level);
		}
		os << Text (L' ', zfill);

		if (!attr.is_in_file){
			wprintf(L"\033[33m%ls\033[0m", text.get_str());
		}
		else {
			os << text.get_str();
		}
		os << Text (L' ', attr.terminal_w - zfill - count_symbols() - 2*level);
		if (level > 0) {
			os << Text(L'#', level);
		}
		// printf("\033[33m%s\033[0m", Text('#', level).get_str());
		os << endl;
		os << cuphead << endl;
		return 0;
	}
	else {
		os << L'\n' << Text('-', attr.terminal_w);
		cerr << "\nToo low terminal width! Can't show header."<< endl << "Header length (with cells): " << this->count_symbols() + 2*level << endl;
		cerr << "Terminal length: "<< attr.terminal_w-1<<endl;
		return -1;
	}
}

unsigned long int Header::count_words() const {
	Text buf(text);
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

void Header::addText(const wchar_t *added) {
	text+=added;
}

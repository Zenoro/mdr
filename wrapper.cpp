#include "wrapper.h"
using namespace std;


void Text_viewer::read()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int int_index, space_ind, header_lvl;
    int list_lvl = 0, newline_ctr=0; Text buf;
    char status = 'n';
    if (fin != NULL) {
        is_opened = true;
        wchar_t* some_str = new wchar_t[BUFSIZE];
        while (fgetws(some_str, BUFSIZE, fin)) {
            some_str[wcscspn(some_str, L"\n")] = 0;
            switch (status) {
                case 'n':   // text after /n
                {
                    // Text n_text(some_str);
                    if (some_str[0] == L'\0'){
                        newline_ctr++;
                        break;
                    }
                    if (!buf.is_empty()){
                        buf.space_cleaner();
                        containter.push_back(new Simple_Text(buf));
                        if (newline_ctr > 1)
                            containter.push_back(new Simple_Text());
                        buf = Text();
                    }
                    if (some_str[0] != L'\0'){
                        newline_ctr = 0; 
                        if (some_str[0] == L'#') {   // header handler
                            header_lvl = 1;
                            while (some_str[header_lvl] == L'#')
                                header_lvl++;
                            Text hdr(some_str);
                            // buf.slice(header_lvl, buf.length()-header_lvl);
                            hdr.slice(header_lvl, hdr.length());
                            containter.push_back(new Header(header_lvl, hdr));
                            break;
                        }
                        else if (isdigit(some_str[0])) {    // numeric list or num+text
                            for (int_index=0; isdigit(some_str[int_index]); int_index++);
                            if (some_str[int_index] == L'.' && some_str[int_index+1] == L' ') {     // numeric list
                                status = 'l';
                                Text marker(some_str);
                                marker.slice(marker.length()-int_index-2);
                                Text txt(some_str);
                                txt.slice(int_index+2, txt.length());
                                list_lvl = 1;
                                containter.push_back(new Some_List(marker, txt, 0));
                                break;
                            }
                            else {      // text with num 
                                status = 's';
                                buf = Text(some_str);
                                // buf.space_cleaner();
                                // containter.push_back(new Simple_Text(buf));
                                break;
                            }
                        }
                        else if (some_str[0] == L'*' && some_str[1] == L' ') {  // marked list
                            status = 'l';
                            Text mrkd(some_str);
                            mrkd.slice(2, mrkd.length());
                            list_lvl = 1;
                            containter.push_back(new Some_List(mrkd, 0));
                            break;
                        }
                        else if (Text(some_str).startswith(Text(L' ',4))) {    // code
                            containter.push_back(new Code(some_str));
                            break;
                        }
                        else {      // simple text
                            status = 's';
                            buf = Text(some_str);
                            // buf.space_cleaner();
                            // containter.push_back(new Simple_Text(buf));
                            break;
                        };
                    }
                }

                case 'l':   // text after list
                {
                    if (!buf.is_empty()){
                        buf.space_cleaner();
                        containter.push_back(new Simple_Text(buf));
                        if (newline_ctr > 1)
                            containter.push_back(new Simple_Text());
                        buf = Text();
                    }
                    Text list_txt(some_str);
                    if (list_txt.startswith(L' ')) {
                        newline_ctr = 0;
                        for (space_ind=0; list_txt[space_ind] == L' '; space_ind++);
                        list_txt.left_space_cleaner();
                        if (list_txt.startswith(L'*') && list_txt[1] == L' ') {      // marked text
                            int newind = space_ind / 2;
                            list_txt.slice(2, list_txt.length());
                            if (newind >= list_lvl) {
                                newind = list_lvl;
                                list_lvl++;
                            }
                            // else list_lvl--;
                            containter.push_back(new Some_List(list_txt, newind));
                            break;
                        }
                        else if (isdigit(list_txt[0])){        // numeric list or num+text
                            newline_ctr = 0;
                            for (int_index=0; isdigit(list_txt[int_index]); int_index++);
                            if (some_str[int_index+space_ind] == L'.' && some_str[space_ind+int_index+1] == L' ') {     // numeric list
                                Text marker(list_txt);
                                marker.slice(marker.length()-int_index-2);
                                list_txt.slice(int_index+2, list_txt.length());
                                int newind = space_ind / 2;
                                if (newind >= list_lvl) {
                                    newind = list_lvl;
                                    list_lvl++;
                                }
                                // else list_lvl--;
                                containter.push_back(new Some_List(marker, list_txt, newind));
                                break;
                            }
                            else if (space_ind>=4) {    // code
                                containter.push_back(new Code(list_txt));
                                break;
                            }
                            else{   // Simple text with integer in the start+spaces (wtf)
                                status = 's';
                                buf = list_txt;
                                break;
                            }
                        }
                        else if(space_ind>=4)   // code
                        {
                            newline_ctr = 0;
                            Text res(L' ', space_ind);
                            res+=list_txt;
                            containter.push_back(new Code(res));
                            status = 'n';
                            break;
                        }
                        else {  // text (or list adder?) 
                            newline_ctr = 0;
                            buf = list_txt;
                            status = 's';
                            break;
                        }
                    }
                    else if (some_str[0] == L'\0') {
                        newline_ctr++;
                        status = 'n';
                        break;
                    }
                    else if (isdigit(some_str[0])) {    // numeric list or num+text
                        newline_ctr = 0;
                        for (int_index=0; isdigit(some_str[int_index]); int_index++);
                        if (some_str[int_index] == L'.' && some_str[int_index+1] == L' ') {     // numeric list
                            Text marker(some_str);
                            marker.slice(marker.length()-int_index-2);
                            list_txt.slice(int_index+2, list_txt.length());
                            list_lvl = 1;
                            containter.push_back(new Some_List(marker, list_txt, 0));
                            break;
                        }
                        else {
                            status = 's';
                            buf = list_txt;
                            // buf.space_cleaner();
                            // containter.push_back(new Simple_Text(buf));
                            break;
                        }
                    }
                    else if (some_str[0] == L'*' && some_str[1] == L' ') {  // marked list
                        newline_ctr = 0;
                        list_txt.slice(2, list_txt.length());
                        list_lvl = 1;
                        containter.push_back(new Some_List(list_txt, 0));
                        break;
                    }
                    else {      // simple text
                        newline_ctr = 0;
                        status = 's';
                        buf = list_txt;
                        // buf.space_cleaner();
                        // containter.push_back(new Simple_Text(buf));
                        break;
                    };
                }

                case 's': {
                    Text s_text(some_str);
                    if (some_str[0] == L'\0'){
                        status = 'n';
                        newline_ctr++;
                        break;
                    }
                    else {
                        newline_ctr = 0;
                        if (isdigit(some_str[0])) {    // numeric list or num+text
                            for (int_index=0; isdigit(some_str[int_index]); int_index++);
                            if (some_str[int_index] == '.' && some_str[int_index+1] == ' ') {     // numeric list
                                status = 'l';
                                Text marker(some_str);
                                marker.slice(marker.length() - int_index - 2);     // берем с точкой и пробелом
                                s_text.slice(int_index+2, s_text.length());     // текст без пробела вначале
                                list_lvl = 1;
                                containter.push_back(new Some_List(marker, s_text, 0));
                                break;
                            }
                            else {
                                // s_text.space_cleaner();
                                buf += L' ';
                                buf += s_text;
                                // containter.push_back(new Simple_Text(buf));
                                break;
                            }
                        }
                        else if (s_text.startswith(L'*') && some_str[1] == L' ') {  // marked list
                            status = 'l';
                            Text mrkd(some_str);
                            mrkd.slice(2, mrkd.length());
                            list_lvl = 1;
                            containter.push_back(new Some_List(mrkd, 0));
                            break;
                        }
                        else if (s_text.startswith(Text(L' ', 4))) {    // code
                            for (space_ind=0; s_text[space_ind] == L' '; space_ind++);
                            status = 'n';
                            Text res(L' ', space_ind);
                            res+= s_text;
                            containter.push_back(new Code(res));
                            break;
                        }
                        else if (s_text.startswith(Text(L"-----"))) {     // (c/t)rap
                            status = 'n';
                            // buf+=L'\n';
                            // buf+=s_text;
                            buf.space_cleaner();
                            containter.push_back(new Header(0, buf));
                            // containter.push_back(new Simple_Text(s_text));
                            buf = Text();
                            break;
                        }
                        else {
                            buf += L' ';
                            buf += s_text;
                            break;
                        }
                    }
                }
            }
        }
    if (!buf.is_empty()) {
        // if (newline_ctr > 1)
            // containter.push_back(new Simple_Text());
        buf.space_cleaner();
        containter.push_back(new Simple_Text(buf));
        buf = Text();
    }
    fclose(fin);
    }
    else {
        wcerr << endl;
        wcerr << L"Can't open file" << endl;
    }
};


int Text_viewer::print(std::wostream& os, const Cfg_Attributes& attr) {
    if (is_opened) {
        int cur=0, lst=0, status;
        unsigned int words_cnt = 0, symbols_cnt = 0;
        setlocale(LC_ALL, "ru_RU.UTF-8");
        // 1 - code
        // 2 - header
        // 3 - lists
        // 4 - simple_text
        // -1 - error happened
        for (const auto& it : containter){
            cur = it->printflag();
            if ((cur==1 || cur==3) && cur!=lst ||(lst==1 || lst==3) && cur!=lst && lst>0){
            // if (cur !=  lst && lst>0){
                os << '\n';
            }
            lst = cur;
            status = it->print(os, attr);
            if (status == -1)
                return -1;
            os << '\n';
            symbols_cnt += it->count_symbols();
            words_cnt += it->count_words();
        }
        wcerr << endl;
        wcerr << L"Words count: " << words_cnt << endl;
        wcerr << L"Symbols count: " << symbols_cnt << endl;
    }
    return 0;
};

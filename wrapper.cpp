#include "wrapper.h"
using namespace std;


void Text_viewer::read()
{
    int int_index, space_ind, header_lvl;
    int list_lvl = 0, newline_ctr=0; Text buf;
    char status = 'n';
    if (fin != nullptr) {
        is_opened = true;
        wchar_t* some_str = new wchar_t[BUFSIZE];
        while (fgetws(some_str, BUFSIZE, fin)) {
            some_str[wcscspn(some_str, L"\n")] = 0;
            Text newline(some_str);
            switch (status) {
                case 'n':   // text after /n
                {
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
                            newline.slice(header_lvl, newline.length());
                            containter.push_back(new Header(header_lvl, newline));
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
                                buf = newline;
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
                        else if (newline.startswith(Text(L' ',4))) {    // code
                            containter.push_back(new Code(some_str));
                            break;
                        }
                        else {      // simple text
                            status = 's';
                            buf = newline;
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
                    if (newline.startswith(L' ')) {
                        newline_ctr = 0;
                        for (space_ind=0; newline[space_ind] == L' '; space_ind++);
                        newline.left_space_cleaner();
                        if (newline.startswith(L'*') && newline[1] == L' ') {      // marked text
                            int newind = space_ind / 2;
                            newline.slice(2, newline.length());
                            if (newind >= list_lvl) {
                                newind = list_lvl;
                                list_lvl++;
                            }
                            containter.push_back(new Some_List(newline, newind));
                            break;
                        }
                        else if (isdigit(newline[0])){        // numeric list or num+text
                            newline_ctr = 0;
                            for (int_index=0; isdigit(newline[int_index]); int_index++);
                            if (some_str[int_index+space_ind] == L'.' && some_str[space_ind+int_index+1] == L' ') {     // numeric list
                                Text marker(newline);
                                marker.slice(marker.length()-int_index-2);
                                newline.slice(int_index+2, newline.length());
                                int newind = space_ind / 2;
                                if (newind >= list_lvl) {
                                    newind = list_lvl;
                                    list_lvl++;
                                }
                                containter.push_back(new Some_List(marker, newline, newind));
                                break;
                            }
                            else if (space_ind>=4) {    // code
                                containter.push_back(new Code(newline));
                                break;
                            }
                            else{   // Simple text with integer in the start+spaces (wtf)
                                status = 's';
                                buf = newline;
                                break;
                            }
                        }
                        else if(space_ind>=4)   // code
                        {
                            newline_ctr = 0;
                            Text res(L' ', space_ind);
                            res+=newline;
                            containter.push_back(new Code(res));
                            status = 'n';
                            break;
                        }
                        else {  // text (or list adder?) 
                            newline_ctr = 0;
                            buf = newline;
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
                            newline.slice(int_index+2, newline.length());
                            list_lvl = 1;
                            containter.push_back(new Some_List(marker, newline, 0));
                            break;
                        }
                        else {
                            status = 's';
                            buf = newline;
                            break;
                        }
                    }
                    else if (some_str[0] == L'*' && some_str[1] == L' ') {  // marked list
                        newline_ctr = 0;
                        newline.slice(2, newline.length());
                        list_lvl = 1;
                        containter.push_back(new Some_List(newline, 0));
                        break;
                    }
                    else {      // simple text
                        newline_ctr = 0;
                        status = 's';
                        buf = newline;
                        break;
                    };
                }

                case 's': {
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
                                newline.slice(int_index+2, newline.length());     // текст без пробела вначале
                                list_lvl = 1;
                                containter.push_back(new Some_List(marker, newline, 0));
                                break;
                            }
                            else {
                                buf += L' ';
                                buf += newline;
                                break;
                            }
                        }
                        else if (newline.startswith(L'*') && some_str[1] == L' ') {  // marked list
                            status = 'l';
                            Text mrkd(some_str);
                            mrkd.slice(2, mrkd.length());
                            list_lvl = 1;
                            containter.push_back(new Some_List(mrkd, 0));
                            break;
                        }
                        else if (newline.startswith(Text(L' ', 4))) {    // code
                            for (space_ind=0; newline[space_ind] == L' '; space_ind++);
                            status = 'n';
                            Text res(L' ', space_ind);
                            res+= newline;
                            containter.push_back(new Code(res));
                            break;
                        }
                        else if (newline.startswith(Text(L"-----"))) {     // (c/t)rap
                            status = 'n';
                            buf.space_cleaner();
                            containter.push_back(new Header(0, buf));
                            buf = Text();
                            break;
                        }
                        else {
                            buf += L' ';
                            buf += newline;
                            break;
                        }
                    }
                }
            }
        }
    if (!buf.is_empty()) {
        buf.space_cleaner();
        containter.push_back(new Simple_Text(buf));
        buf = Text();
    }
    fclose(fin);
    delete[] some_str;
    }
    else {
        cerr << "\nCan't open file\n";
    }
}


int Text_viewer::print(std::wostream& os) {
    if (is_opened) {
        int cur=0, lst=0, status;
        unsigned int words_cnt = 0, symbols_cnt = 0;
        // 1 - code
        // 2 - header
        // 3 - lists
        // 4 - simple_text
        // -1 - error happened
        Abstract_text* it;
        while (!containter.empty()) {
            it = containter.front();
            cur = it->printflag();
            if ((cur==1 || cur==3) && cur!=lst ||(lst==1 || lst==3) && cur!=lst && lst>0){
            // if (cur !=  lst && lst>0){
                os << L'\n';
            }
            lst = cur;
            status = it->print(os, cfg);
            if (status == -1) {
                delete it;
                while(!containter.empty()) {
                    containter.pop_front();
                    delete containter.front();
                }
                return -1;
            }
            os << L'\n';
            symbols_cnt += it->count_symbols();
            words_cnt += it->count_words();
            delete it;
            containter.pop_front();
        }
        cerr << "\nWords count: " << words_cnt << '\n';
        cerr << "Symbols count: " << symbols_cnt << '\n';
    }
    return 0;
}

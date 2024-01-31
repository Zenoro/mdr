#include "text.h"
#include <locale.h>
using namespace std;

Text::Text(const wchar_t* othr_str) {
    len = 0;
    for(len = 0; othr_str[len] != L'\0'; len++){
        //empty  body
    }
    str = new wchar_t[len+1];
    for (unsigned int i=0; i<len; i++){
        str[i] = othr_str[i];
    }
    str[len] = L'\0';
}

Text::Text(const Text& other_cp){
    len = other_cp.len;
    str = new wchar_t[len+1];
    for (int i=0; i<other_cp.len; i++){
        str[i] = other_cp.str[i];
    }
    str[len] = L'\0';
};

Text::Text(wchar_t add, unsigned long _len) {
    str = new wchar_t[_len+1];
    len = _len;
    for (unsigned int i=0; i<_len; i++){
        str[i] = add;
    }
    str[_len] = L'\0';
};

void Text::left_space_cleaner() {  //remove only first spaces
    Text* tmp = new Text(*this);
    delete[] str;
    int ind = 0;
    for (ind=0; (tmp->str[ind]== L' ' || tmp->str[ind]== L'\t'); ind++){
        // empty body
    }
    len = tmp->len - ind;
    str = new wchar_t[len+1];
    for (int i=0; i<len; i++){
        str[i] = tmp->str[i+ind];
    }
    str[len] = L'\0';
    delete tmp;
}

void Text::space_cleaner() {   //remove multiples spaces & first spaces
    Text tmp(*this);
    tmp.left_space_cleaner();
    unsigned int txtlen = tmp.len;
    delete[] str;
    wchar_t* buf = new wchar_t[txtlen];
    int ind = 0; int was_space = 0; int cnt = 0;
    for (int i=0; i<txtlen; i++)
    {
        if (tmp.str[i] != L' '){
            buf[cnt] = tmp.str[i];
            was_space = 0;
            cnt++;
        }
        else if (tmp.str[i] == L' ' && was_space == 0){
            buf[cnt] = tmp.str[i];
            was_space = 1;
            cnt++;
        }
    }
    len = cnt;
    str = new wchar_t[len+1];
    for (int i=0; i<len; i++){
        str[i] = buf[i];
    }
    str[len] = L'\0';
    delete[] buf;
}

bool Text::startswith(const Text& checker) {  // check if starts with some Text
    int chk_cnt = checker.len;
    int cnt = 0;
    for (int i=0; i< chk_cnt; i++){
        if (str[i] == checker[i]){
            cnt++;
        }
    }
    return cnt==chk_cnt;
}

bool Text::startswith(wchar_t c) {  // check if starts with some wchar_t
    return str[0]==c;
}

const Text& Text::operator+=(const wchar_t *added) {
    unsigned long added_len = wcslen(added);
    Text buffer(*this);
    delete[] str;
    str = new wchar_t[len+added_len+1];
    for (int i=0; i<len; i++){
        str[i]=buffer.str[i];
    }
    for (int i=len; i<len+added_len; i++){
        str[i] = added[i-len];
    }
    str[len+added_len] = L'\0';
    len += added_len;
    return *this;
}

const Text& Text::operator+=(const Text& obj) {
    return *this += obj.str;
}

Text& Text::operator+=(const wchar_t c) {
    Text buf(*this);
    delete[] str;
    len = len+1;
    str = new wchar_t[len+1];
    for (unsigned int i=0; i<len-1; i++){
        str[i] = buf.str[i];
    }
    str[len-1] = c;
    str[len] = L'\0';
    return *this;
}

const Text& Text::operator=(const Text& cpd) {
    len = cpd.len;
    delete[] str;
    str = new wchar_t[len+1];
    for (unsigned int i=0; i< len; i++){
        str[i] = cpd.str[i];
    }
    str[len] = L'\0';
    return *this;
}

wchar_t& Text::operator[](unsigned long ind) const {
    if (ind < len){
        return str[ind];
    }
    throw "NO ELEMENT";
}

void Text::slice(unsigned long cutter) {
    if (cutter < 0){
        cutter = 0;
    }
    else if (cutter > len){
        cutter = len;
    }
    Text buf(*this);
    len -= cutter;
    delete[] str;
    str = new wchar_t[len+1];
    for (unsigned int i=0; i<len; i++)
        str[i] = buf.str[i];
    str[len] = L'\0';
}

void Text::slice(unsigned long start, unsigned long end) {
    Text buf(*this);
    if (end>len) end=len;
    if (start>len) start=len;
    len = end - start;
    // if (len<0) len=0;
    delete[] str;
    str = new wchar_t[len+1];
    for (int i=start; i<end; i++){
        str[i-start] = buf.str[i];
    }
    str[len] = L'\0';
}

bool Text::is_empty() {
    return len == 0;
}

wostream& operator<<(wostream& out, const Text& othr) {
    out << othr.str;
    return out;
}

Text operator+(const Text& fst, const Text& scn) {
	Text tmp(fst);
    return tmp+=scn;
}

Text operator+(const Text& fst, const wchar_t scn) {
	Text tmp(fst);
    if (scn == L'\n'){
        return tmp += L'\n';
    }
    else {
        return tmp += scn;
    }
}

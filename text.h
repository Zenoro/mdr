#pragma once
#include <iostream>
#include <wchar.h>
// #include <cstring>

using namespace std;

class Text {
    private:
        wchar_t* str;
        unsigned long len;
    public:
        Text():len(0) {str = new wchar_t[1]; str[0] = L'\0';};

        Text(const wchar_t* othr_str);
        Text(const wchar_t* othr_str, unsigned long othr_len);

        Text(const Text& other_cp);

        Text(wchar_t add, unsigned long _len);

        ~Text() {
            delete[] str;
            len=0;
        }

        unsigned long length() const { return len; };   //return length of Text
        const wchar_t* get_str() const { return str; };    //return Text string

        void left_space_cleaner();  //remove only first spaces

        Text left_space_cleaner(int flag); //remove only first spaces & return the value

        void space_cleaner();   //remove multiples spaces & first spaces
        

        bool startswith(const Text& checker);  // check if starts with some Text

        bool startswith(wchar_t c);  // check if starts with some wchar_t

        const Text& operator+=(const wchar_t *added);

        const Text& operator+=(const Text& obj);

        Text& operator+=(const wchar_t c);

        const Text& operator=(const Text& cpd);

        wchar_t& operator[](unsigned long ind) const;

        void slice(unsigned long cutter);

        void slice(unsigned long start, unsigned long end);    // I love python 

        bool is_empty();

        friend Text operator+(const Text& fst, const Text& scn);
        friend wostream& operator<<(wostream& out, const Text& othr);
};

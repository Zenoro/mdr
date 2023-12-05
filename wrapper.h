#pragma once
#include <fstream>
#include <wchar.h>
#include <locale.h>
#include <list>
#include "lists.h"
#include "header.h"
#include "code.h"
#include "simple_text.h"
#define BUFSIZE 400
using namespace std;

class Text_viewer{
private:
    FILE* fin;
    Cfg_Attributes cfg;
    std::list<Abstract_text*> containter;
protected:
    bool is_opened;
public:
    Text_viewer(FILE* fn): fin(fn), cfg(Cfg_Attributes()) {};
    Text_viewer(FILE* fn, Cfg_Attributes config): fin(fn), cfg(config) {};

    void read();
    int print(std::wostream& os, const Cfg_Attributes& attr);
};

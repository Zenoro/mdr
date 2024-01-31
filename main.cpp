#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fstream>
#include "wrapper.h"
#include <wchar.h>
#include <locale.h>
using namespace std;


// 1. -f имя файла с разметкой,
// 3. -t отступ для второй и последующих строк списка (число пробелов),
// 4. -m символ-маркер ненумерованного списка,
// 5. -r величина красной строки.
// 6. -h - выдача help с описанием поведения программы
// 7. -v - отображение версии программы
// extra 1. -i - корректный вывод в файл

int main(int argc, char** argv) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    char gopt;
    Cfg_Attributes terminal_output;
    FILE* filename = stdin;
    Text help;
        while ((gopt=getopt(argc, argv, "f:t:m:r:hvi"))!=-1) {
            switch (gopt) {
                case 'f':
                    filename = fopen(optarg, "r");
                    break;
                case 't':
                    terminal_output.extra_tab = atoi(optarg);
                    break;
                case 'm': {
                    wchar_t* wc = new wchar_t[1];
                    mbstowcs(wc, optarg, 1);
                    terminal_output.marker = wc[0];
                    break;
                }
                case 'r':
                    terminal_output.redstr_tab = atoi(optarg);
                    break;
                case 'i': {
                    terminal_output.is_in_file = true;
                    terminal_output.terminal_w = 100;
                    break;
                }
                case 'h':
                {
                    wifstream fd("docs/help");
                    wchar_t c;
                    while (fd.get(c))
                        help+=c;
                        wcout << help;
                    fd.close();
                }
                    return 0;
                case 'v':
                    wcout<<L"version 1.0"<<endl;
                    return 0;
                case '?':
                    wcout<<L"Try \"-h\" param, you can get additional information."<<endl;
                    return -1;
            }
        }
        Text_viewer saluki(filename, terminal_output);
        saluki.read();
        saluki.print(wcout);
        return 0;
}

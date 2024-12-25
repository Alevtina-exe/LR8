#include "func.h"

int main() {
    int i;
    for (i = 0; i <= 78; i++) { 
        std::cout << "*";
    }
    std::cout << "\nДанная программа предназначена для работы с квитанциями в радиоателье." <<
        std::endl << "Выполнила Огиенко Дарья, группа 453502, вариант 15." << std::endl;
    for (i = 0; i <= 78; i++) { 
        std::cout << "*";
    }
    std::cout << std::endl;
    quittance* LIST = NULL;
    while(true) {
        int var = console_func();
        std::cout << std::endl;
        if(var == 1) LIST = create_list(LIST);
        else if(var == 2) show_quittances(LIST);
        else if(var == 3) LIST = add_quittances(LIST);
        else if(var == 4) display_par_quitt(LIST);
        else if(var == 5) del_quitt(LIST);
        else if(var == 6) change_quitt(LIST);
        else if(var == 7) sort_quitt(LIST);
        else if(var == 8) file_write(LIST);
        else if(var == 9) LIST = recover_from_file(LIST);
        else if(var == 10) elasticsearch_func(LIST);
        else return 0;
    }
    free(LIST);
}
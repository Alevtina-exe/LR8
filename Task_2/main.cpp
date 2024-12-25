#include "func.h"

int main() {
    int i;
    for (i = 0; i <= 78; i++) { 
        std::cout << "*";
    }
    std::cout << "\nДанная программа предназначена для работы с записью на покупку товара в магазине." <<
        std::endl << "Выполнила Огиенко Дарья, группа 453502, вариант 15." << std::endl;
    for (i = 0; i <= 78; i++) { 
        std::cout << "*";
    }
    queue* LIST = NULL;
    while(true) {
        int var = console_func();
        std::cout << std::endl;
        if(var == 1) LIST = create_list(LIST);
        else if(var == 2) show_queues(LIST);
        else if(var == 3) LIST = add_queues(LIST);
        else if(var == 4) del_queue(LIST);
        else if(var == 5) change_queue(LIST);
        else if(var == 6) del_rep_queue(LIST);
        else if(var == 7) file_write(LIST);
        else if(var == 8) elasticsearch_func(LIST);
        else return 0;
    }

}
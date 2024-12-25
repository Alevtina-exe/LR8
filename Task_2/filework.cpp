#include <fstream>
#include "func.h"
extern int size;
bool file = false, efile = false;
std::string filename = "struct.txt";

std::string queue_to_string(queue& Q) { //Преобразования данных о покупателе в строку
    std::string str = "Порядковый номер: " ;
    str += Q.num.str;
    str += ".\nФамилия: ";
    str += Q.surname;
    str += ".\nИмя: ";
    str += Q.name;
    str += ".\nОтчество: ";
    str += Q.midname;
    str += ".\nДата постановки на учёт: ";
    str += Q.date;
    str += ".\nАдрес: ";
    str += Q.address;
    str += ".";
    return str;
}

void file_write(queue* Q) { //Запись в файл
    if(Q == NULL) {
        std::cout << "Список покупателей не инициализирован. Выход из режима записи в текстовый файл...\n";
        return;
    }
    std::ifstream check("struct.txt");
    if(!file) { //Если запись в файл ещё не производилась
        if(check) { //проверка на наличие файла со структурами
            std::cout << "Обнаружен файл со структурами: '" << filename << "'." <<
                " Введите 1, чтобы перезаписать файл, или 0, чтобы создать новый файл:\n";
            if(!int_input(0,1)) {
                int i = 2;
                while(check) { //Поиск незанятого имени для файла
                    check.close();
                    filename = "struct(" + std::to_string(i) + ").txt";
                    check.open(filename, std::ios::in);
                    i++;
                }
            }
        }
        std::ofstream out(filename, std::ios::trunc);
        for(int i = 0; i < size; i++) {
            out << queue_to_string(Q[i]) << std::endl << std::endl;
        }
        file = true;
        efile = true;
        std::cout << "Запись в текстовый файл '" << filename << "' завершена успешно!\n\n";
        check.close();
    }
    else { //Внесение изменений, если запись в файл производилась до этого
        std::ofstream stream(filename.c_str(), std::ios::trunc);
        for(int i = 0; i < size; i++) {
            stream << queue_to_string(Q[i]) << std::endl << std::endl;
        }    
        std::cout << "Изменения внесены в файл '" << filename << "'.\n\n";
        efile = true;
        stream.close();
    }
}

void add_in_file(queue* Q, int n) { //Добавление покупателей без перезаписи
    std::ofstream stream(filename, std::ios::ate);
    for(n--; n < size; n++) {
        stream << queue_to_string(Q[n]);
    }
    stream.close();
}

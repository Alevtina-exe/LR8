#include "func.h"

bool file = false, efile = false; //Проверка открыт ли файл
std::string filename = "struct.bin"; //Имя файла

void file_write(quittance* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима записи в бинарный файл...\n";
        return;
    } 
    FILE *check = fopen("struct.bin", "r");
    if(!file) { //Если запись в файл ещё не производилась
        if(check) { //проверка на наличие файла со структурами
            std::cout << "Обнаружен файл со структурами: '" << filename << "'." <<
                " Введите 1, чтобы перезаписать файл, или 0, чтобы создать новый файл:\n";
            if(!int_input(0,1)) {
                int i = 2;
                while(check) { //Поиск незанятого имени для файла
                    fclose(check);
                    filename = "struct(" + std::to_string(i) + ").bin";
                    check = fopen(filename.c_str(),"r");
                    i++;
                }
            }
        }
        check = fopen(filename.c_str(), "wb");
        int num = fwrite(Q, sizeof(quittance), size, check);
        if(num != size) { 
            std::cout << "Возникла ошибка записи в файл! Выход из режима записи в бинарный файл...\n\n";
            remove(filename.c_str());
            return;
        }
        file = true; efile = true; 
        std::cout << "Запись в бинарный файл '" << filename << "' завершена успешно!\n\n";
        fclose(check);
    }
    else { //Внесение изменений, если запись в файл производилась до этого
        FILE* stream = fopen(filename.c_str(), "wb");
        int num = fwrite(Q, sizeof(quittance), size, stream);
        if(num != size) { 
            std::cout << "Возникла ошибка записи в файл! Выход из режима записи в бинарный файл...\n\n";
            return;
        }
        efile = true;
        std::cout << "Изменения внесены в файл '" << filename << "'.\n\n";
        fclose(stream);
    }
}

quittance* recover_from_file(quittance* Q) { //Восстановление структур из файла
    if(Q != NULL) {
        std::cout << "Внимание, Ваш список квитанций не пустой. При восстанновлении из бинарного файла старый список будет удалён.\n";
        std::cout << "Введите 1, чтобы продолжить, или 0, чтобы выйти из режима восстановления:\n";
        if(!int_input(0,1)) {
            std::cout << "Выход из режима восстановления из файла...\n\n";
            return Q;
        }
    }
    std::string name = "struct.bin";
    FILE* check = fopen(name.c_str(), "r");
    if(!check) {
        std::cout << "Бинарные файлы не обнаружены! Выход из режима восстановления из файла...\n\n";
        return Q;
    } 
    std::cout << "Обнаруженные бинарные файлы в папке:\n";
    int i = 2;
    while(check) { //Поиск файлов со структурами
        fclose(check);
        std::cout << "-" << name << ";\n";
        name = "struct(" + std::to_string(i) + ").bin"; 
        i++;
        check = fopen(name.c_str(), "r"); 
    }
    getchar();
    while(true) { //выбор файла
        std::cout << "Введите имя файла, из которого Вы хотели бы восстановить список:\n";
        getline(std::cin, name);
        check = fopen(name.c_str(), "r");
        if(check) break;
        std::cout << "Файл с заданным именем не найден!\n";
    }

    //определение количества элементов
    fseek(check, 0, SEEK_END); 
    long fileSize = ftell(check); 
    fseek(check, 0, SEEK_SET);
    size = fileSize / sizeof(quittance);

    //восстановление в структуру
    quittance* array = (quittance*)malloc(fileSize);
    int num = fread(array, sizeof(quittance), size, check);
    if(num != size) {
        std::cout << "Возникла ошибка при чтении файла! Выход из режима восстановления из файла...\n\n";
        free(array);
        return NULL;
    }
    fclose(check);
    std::cout << "Восстановленный список квитанций:\n\n";
    show_quittances(array);
    efile = true;
    return array;
}

void add_in_file(quittance* Q, int n) { //Добавление квитанций без перезаписи
    FILE* stream = fopen(filename.c_str(), "ab");
    for(; n < size; n++) {
        fwrite(&Q[n], 1, sizeof(quittance), stream);
    }
    fclose(stream);
}

void edit_in_file(quittance* Q, int n, int npar, const char* par) { //Изменение квитанций без перезаписи
    FILE* stream = fopen(filename.c_str(), "rb+");
    fseek(stream, n * sizeof(quittance), SEEK_SET);
    fwrite(&Q[n], 1, sizeof(quittance), stream);
    fclose(stream);
}
#include "func.h"

int size = 0;
bool Check = false;
bool file = false, efile = false; //Проверка открыт ли файл
std::string filename = "struct.bin"; //Имя файла

int console_func() { //Консольное меню
    std::cout << "Выберите действие из предложенного списка:\n" <<
        "1. Создание базы с квитанциями.\n" <<
        "2. Просмотр имеющихся квитанций.\n" << 
        "3. Добавление новых квитанций.\n" <<
        "4. Вывод квитанций с невыполненным заказом.\n" <<
        "5. Удаление квитанций с заданным параметром.\n" <<
        "6. Изменение внесённых квитанций.\n" <<
        "7. Упорядочение всех квитанций по дате приёма изделия.\n" <<
        "8. Запись списка квитанций/внесение изменений в бинарный файл.\n" <<
        "9. Восстановление списка квитанций из бинарного файла.\n" <<
        "10. Поиск структуры по заданному параметру при помощи elasticsearch.\n" <<
        "11. Завершение выполнения программы.\n";
    return int_input(1, 11);
}

//Вспомогательные функции
std::string par_choice(int npar) { //Ввод параметра по его номеру
    std::string par;
    if(npar == 1) {
        std::cout << "Введите наименование изделия (состоит только из букв и цифр):\n";
        par = device_name_input(0);
    }
    else if(npar == 2) {
        std::cout << "Введите серийный номер изделия (состоит из латиницы и цифр и не содержит пробелов):\n";
        par = serial_num_input();            
    }
    else if(npar == 3) {
        std::cout << "Введите имя заказчика (состоит только из букв):\n";
        par = device_name_input(1);            
    }
    else if(npar == 4) {
        std::cout <<  "Введите дату приёма изделия в ремонт (формат: ДД.ММ.ГГГГ):\n";
        par = date_input();            
    }
    else {
        std::cout << "Введите состояние готовности заказа (1 - выполнен, 0 - не выполнен):\n";
        par = std::to_string(int_input(0, 1));
        par = par == "1" ? "Выполнен" : "Не выполнен";
    }
    return par;
}

int choose_npar() { //Выбор параметра
    std::cout << "Какой параметр вы хотите указать?\n" <<
        "1. Наименование изделия.\n" <<
        "2. Серийный номер изделия.\n" <<
        "3. Имя заказчика.\n" <<
        "4. Дата приёма изделия в ремонт.\n" <<
        "5. Состояние готовности заказа.\n";
    return int_input(1, 5);

}

bool check(int npar, std::string par, quittance& Q) { //Проверка совпадения поля структуры и параметра
    if(npar == 1 && Q.device == par || 
       npar == 2 && Q.serial_num == par || 
       npar == 3 && Q.name == par || 
       npar == 4 && Q.date.str == par || 
       npar == 5 && Q.status.str_stat == par)
    {
        return true;
    } 
    else return false;
}


//Фукнции работы со структурами
quittance* create_list(quittance* Q) {
    if(Q != NULL) {
        std::cout << "Ваш список инициализирован, вы можете дополнить его или удалить ненужные квитанции.\n" <<
            "Введите 1, если хотите инициализировать список заново, или 0, чтобы выйти из режима создания списка:\n";
        if(!int_input(0,1)) {
            std::cout << "Выход из режима создания списка...\n\n";
            return Q;
        }
        else {
            free(Q);
            Q = NULL;
        }       
    }
    size = 0;
    efile = false;
    std::cout << "Выберите механизм ввода данных:\n" <<
        "1. Ввод количества квитанций.\n" <<
        "2. Ввод до появления квитанции с заданным признаком.\n" <<
        "3. Диалог о необходимости продолжить ввод.\n";
    int option = int_input(1,3);
    std::cout << "При пропуске поля(кроме статуса) реализуется автозаполение.\n";
    if(option == 1) { //Ввод по количеству
        int N;
        quittance* array = NULL;
        while(array == NULL) {
            std::cout << "Введите количество квитанций:\n";
            N = int_input();
            array = (quittance*)malloc(N * sizeof(quittance));
            if(array == NULL) {
                std::cout << "Ошибка выделения памяти! Попробуйте ввести меньшее количество квитанций.\n" <<
                    "Введите 1, чтобы попробовать заново, или 0, чтобы выйти из режима создания базы квитанций:\n";
                if(!int_input(0, 1)) {
                    std::cout << "Выход из режима создания списка...\n\n";
                    return array;
                }
            }
        }
        for(size = 0; size < N; size++) {
            std::cout << "Ввод квитанции №" << size + 1 << ":\n";
            getchar();
            quitt_input(array[size]);
            autocomplete(array, size);
        }
        std::cout << std::endl;
        return array;
    }
    else if(option == 2) { //Ввод по заданному параметру
        int npar = choose_npar();
        getchar();
        std::string par = par_choice(npar);
        quittance* array = (quittance*)malloc(sizeof(quittance)), *q = array;
        while (true) {
            if(q == NULL) {
                std::cout << "К сожалению, достигнуто максимальное количество квитанций.\n Выход из режима создания списка...\n\n";
                return array;
            }
            array = q;
            std::cout << "Ввод квитанции №" << size + 1 << ":\n";
            quitt_input(array[size]);
            autocomplete(array, size);
            size++;
            if(check(npar, par, array[size])) {
                std::cout << "Выход из режима создания списка...\n\n";
                free(q);
                return array;
            }
            q = (quittance*)realloc(array, (size + 1) * sizeof(quittance));
        }
    }
    else { //Диалог о необходимости продолжать ввод
        quittance* array = (quittance*)malloc(sizeof(quittance)), *q = array;
        getchar();
        std::cout << "При пропуске поля(кроме статуса) реализуется автозаполение.\n";
        while (true) {
            if(q == NULL) {
                std::cout << "К сожалению, достигнуто максимальное количество квитанций.\n Выход из режима создания списка...\n\n";
                return array;
            }
            array = q;
            std::cout << "Ввод квитанции №" << size + 1 << ":\n";
            quitt_input(array[size]);
            autocomplete(array, size);
            size++;
            std::cout << "Продолжить ввод? (1 - да, 0 - нет)\n";
            if(!int_input(0,1)) {
                std::cout << "Выход из режима создания списка...\n\n";
                free(q);
                return array;
            }
            getchar();
            q = (quittance*)realloc(array, (size + 1) * sizeof(quittance));
        }
    }
}

void show_quittances(quittance* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима просмотра квитанций...\n\n";
        return;
    }
    for(int i = 0; i < size; i++) {
        std::cout << "Квитанция №" << i + 1 << ":\n";
        quitt_output(Q[i]);    
        std::cout << "\n";
    }
}

quittance* add_quittances(quittance* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима добавления квитанций...\n\n";
        return Q;
    }
    int n = size;
    while (true) {
        quittance* q = (quittance*)realloc(Q, (size + 1) * sizeof(quittance));
        if(q == NULL) {
            std::cout << "К сожалению, достигнуто максимальное количество квитанций.\n Выход из режима добавления квитанций...\n\n";
            return Q;
        }
        Q = q;
        size++;
        std::cout << "Ввод квитанции №" << size << ":\n";
        getchar();
        quitt_input(Q[size - 1]);
        autocomplete(Q, size - 1);
        std::cout << "Добавить ещё одну квитанцию? (1 - да, 0 - нет):\n";
        if(!int_input(0, 1)) {
            if(file) { //Проверка записывались ли данные в файл
                std::cout << "Внести изменения в бинарный файл? (1 - да, 0 - нет):\n";
                if(int_input(0, 1)) add_in_file(Q, n);
            }
            std::cout << "Выход из режима добавления квитанций...\n\n";
            efile = false;
            return q;
        }
    }
}

void display_par_quitt(quittance* Q) { //Вывод квитанций по параметру
    std::cout << "Найденные квитанции:\n\n";
    bool have = false;
    for(int i = 0; i < size; i++) {
        if(!strcmp(Q[i].status.str_stat, "Не выполнен")) { 
            std::cout << "Квитанция №" << i + 1 << ":\n";
            quitt_output(Q[i]);
            std::cout << std::endl;
            have = true;
        } 
    }
    if(!have) std::cout << "Квитанций с заданным параметром не найдено!\n\n";
}

void del_quitt(quittance* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима удаления квитанций...\n\n";
        return;
    }

    int* num;
    int npar = choose_npar();
    getchar();
    std::string par = par_choice(npar); //Задаётся параметр
    num = (int*)malloc(sizeof(int));
    int n = 0;
    for(int i = 0; i < size; i++) {
        if(check(npar, par, Q[i])) { //Соответствие параметру - название меняется на "*"
            n++;
            num = (int*)realloc(num, n * sizeof(int));
            num[n - 1] = i + 1; //Номера удаляемых квитанций
            Q[i].device[0] = '*'; Q[i].device[1] = '\0';
        }
    }
    if(n) {
        if(n == 1) {
            std::cout << "Была удалена квитанция с номером " << num[0];
        }
        else {
            std::cout << "Были удалены квитанции с номерами " << num[0];
            for(int i = 1; i < n; i++) std::cout << ", " << num[i];
        }
        std::cout << ".\n";
        for(int i = num[0] - 1; i < size - n; i++) { //Удаление квитанций
            if(!strcmp(Q[i].device, "*")) {
                int j = i + 1;
                while(!strcmp(Q[j].device, "*")) {
                    j++;
                }
                std::swap(Q[i], Q[j]);
            }
        }
        free(num);
        size -= n;
        Q = (quittance*)realloc(Q, size * sizeof(quittance));
        if(size) {
            std::cout << "Изменённый список квитанций:\n\n";
            show_quittances(Q);
        }
        efile = false;
    }
    else std::cout << "\nКвитанций с заданным параметром не было найдено!\n\n";
}

void change_quitt(quittance* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима изменения квитанций...\n\n";
        return;
    }

    int npar = choose_npar();
    getchar();
    std::string par = par_choice(npar);
    bool change = false;

    for(int i = 0; i < size; i++) {
        if(check(npar, par, Q[i])) {
            change = true;
        }
    }
    if(!change) { //Нет покупателей с данным параметром
        std::cout << "Покупателей с заданным параметром не найдено!\n\n";
        return;
    }
    if(efile) { //Проверка были ли записи в файл
        std::cout << "Вносить изменения в бинарный файл? (1 - да, 0 - нет):\n";
        if(!int_input(0, 1)) efile = false;
    }
    std::cout << "Введите, на что заменить данный параметр:\n";
    std::string PAR = par_choice(npar);
    std::cout << "Изменённые квитанции:\n\n";
    for(int i = 0; i < size; i++) { //Замена параметров, вывод изменённых квитанций
        if(check(npar, par, Q[i])) {
            std::cout << "Квитанция №" << i + 1 << ":\n";
            if(npar == 1) strcpy(Q[i].device, PAR.c_str());
            else if(npar == 2) strcpy(Q[i].serial_num, PAR.c_str());
            else if(npar == 3) strcpy(Q[i].name, PAR.c_str());
            else if(npar == 4) strcpy(Q[i].date.str, PAR.c_str());
            else if(npar == 5) strcpy(Q[i].status.str_stat, PAR.c_str());
            if(efile) edit_in_file(Q, i, npar, par.c_str());
            quitt_output(Q[i]);
            std::cout << std::endl;
        }
    }
}

void sort_quitt(quittance* Q) { //Пузырьковая сортировка по дате приёма
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима сортировки квитанций...\n\n";
        return;
    }
    for(int i = 0; i < size; i++) { //Перевод даты в число
        std::string A = Q[i].date.str;
        Q[i].date.itgr = stoi(A.substr(0, 2)) + 30 * stoi(A.substr(3, 2)) + 365 * stoi(A.substr(6, 4));
    }
    for(int i = 0; i < size - 1; i++) {
        for(int j = 0; j < size - i - 1; j++) {
            if(Q[j].date.itgr < Q[j + 1].date.itgr) {
                std::swap(Q[j], Q[j + 1]);
            }
        }
    }
    efile = false;
    std::cout << "Список квитанций отсортирован!\n\n";
}

void autocomplete(quittance* Q, int n) {
    if(!strlen(Q[n].device)) strcpy(Q[n].device, Q[n - 1].device);
    if(!strlen(Q[n].serial_num)) strcpy(Q[n].serial_num, Q[n - 1].serial_num);
    if(!strlen(Q[n].name)) strcpy(Q[n].name, Q[n - 1].name);
    if(!strlen(Q[n].date.str)) strcpy(Q[n].date.str, Q[n - 1].date.str);
    if(!strlen(Q[n].status.str_stat)) strcpy(Q[n].status.str_stat, Q[n - 1].status.str_stat);
}

int int_input(int min, int max) { //Ввод целого числа с ограничениями
    int a;
    while (true) {
        while (!(std::cin >> a) || (std::cin.peek() != '\n')) {
            std::cin.clear();
            while (std::cin.get() != '\n');
            std::cout << "Неверный формат ввода.\n";
        }
        if (a >= min && a <= max) return a;
        else std::cout << "Неверный формат ввода.\n";
    }
}
int int_input() { //Ввод натурального числа
    int a;
    while (true) {
        while (!(std::cin >> a) || (std::cin.peek() != '\n')) {
            std::cin.clear();
            while (std::cin.get() != '\n');
            std::cout << "Неверный формат ввода.\n";
        }
        if (a > 0) return a;
        else std::cout << "Неверный формат ввода.\n";
    }
}

std::string device_name_input(bool name) { //Ввод имени клиента и названия прибора
    std::string dev;
    while (true) {
        int i;
        getline(std::cin, dev);
        for(i = 0; i < dev.length(); i++) {
            if(!isalpha(dev[i]) && (int) dev[i] > 0 && dev[i] != ' ' && (!isdigit(dev[i]) || name)) {
                std::cout << "Неверный формат ввода.\n";
                break;
            }
        }
        if(i == dev.length() && (size || (dev != "" && 
           std::count(dev.begin(), dev.end(), ' ') != dev.size()))) return dev;
    }
}

std::string serial_num_input() { //Ввод серийного номера
    std::string num; 
    while (true) {
        getline(std::cin, num);
        int i;
        for(i = 0; i < num.length(); i++) {
            if(!isalnum(num[i])) {
                std::cout << "Неверный формат ввода.\n";
                break;                
            }
            if(islower(num[i])) num[i] = toupper(num[i]);
        }
        if(i == num.length() && (size || num != "")) {
            return num;
        }
    }
}

std::string date_input() { //Ввод даты
    std::string d;
    int i;
    getline(std::cin, d);
    if(size && d == "") return d;
    if(d.size() == 10 && d[2] == '.' && d[5] == '.') { //Проверка наличия точек и размера строки
        for(i = 0; i < 10; i++) {
            if(i != 2 && i != 5 && !isdigit(d[i])) { //Проверка чтоб остальные символы были цифрами
                std::cout << "Неверный формат ввода.\n";
                return date_input();
            }
        }
        if(stoi(d.substr(6, 4)) > 2024 || stoi(d.substr(3, 2)) > 12 || !stoi(d.substr(3, 2))) {
            std::cout << "Несуществующая дата. Повторите попытку ввода.\n";
            return date_input();
        }
        int num[7]{1, 3, 5, 7, 8, 10, 12};
        bool k31 = false;
        for(i = 0; i < 7; i++) {
            if(stoi(d.substr(3, 2)) == num[i]) k31 = true;
        }
        if((stoi(d.substr(3, 2)) == 2 && stoi(d.substr(0, 2)) > 28 + 
            int(!(stoi(d.substr(6, 4)) % 4))) || stoi(d.substr(0, 2)) > 30 + k31 ||
            !stoi(d.substr(0, 2))) 
        {
            std::cout << "Несуществующая дата. Повторите попытку ввода.\n";
            return date_input();            
        }
        return d;
    }
    else {
        std::cout << "Неверный формат ввода.\n";
        return date_input();
    }
}


void quitt_input(quittance& Q) { //Ввод квитанции
    std::cout << "Наименование изделия (состоит только из букв и цифр):\n";
    strcpy(Q.device, device_name_input(0).c_str());
    std::cout << "Серийный номер изделия (состоит из латиницы и цифр и не содержит пробелов):\n";
    strcpy(Q.serial_num, serial_num_input().c_str());
    std::cout << "Имя заказчика (состоит только из букв):\n";
    strcpy(Q.name, device_name_input(1).c_str());
    std::cout <<  "Дата приёма изделия в ремонт (формат: ДД.ММ.ГГГГ):\n";
    strcpy(Q.date.str, date_input().c_str());
    std::cout << "Состояние готовности заказа (1 - выполнен, 0 - не выполнен):\n";
    Q.status.bool_stat  = int_input(0, 1);
    Q.status.bool_stat ? strcpy(Q.status.str_stat, "Выполнен") : strcpy(Q.status.str_stat, "Не выполнен");
}
void quitt_output(quittance& Q) { //Вывод квитанции
    std::cout << "Наименование изделия: " << Q.device <<
            ".\nСерийный номер изделия: " << Q.serial_num <<
            ".\nИмя заказчика: " << Q.name << 
            ".\nДата приёма изделия в ремонт: " << Q.date.str <<
            ".\nСостояние готовности заказа: " << Q.status.str_stat << ".\n";
}



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
    if(file) efile = true;
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



// Пустая функция для подавления вывода 
size_t write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) { 
    return size * nmemb; 
}
// Функция для записи данных в файл 
size_t write_to_file(void* ptr, size_t size, size_t nmemb, std::ofstream* stream) { 
    stream->write((char*)ptr, size * nmemb); 
    return size * nmemb; 
}

//Функции для обработки дат
int d_int(std::string& A) {
    return stoi(A.substr(0, 2)) + 30 * stoi(A.substr(3, 2)) + 365 * stoi(A.substr(6, 4));
}
std::string d_to_jform(const char* date) {
    std::string A = date;
    return A.substr(6) + "-" + A.substr(3, 2) + "-" + A.substr(0, 2);;
}

//Функции основного процесса(индексирование и поиск)
void delete_old_docs() { //Удаление предыдущих структур
    CURL* curl;
    CURLcode res;

    // Условие удаления всех документов
    const std::string query = "{\"query\": {\"match_all\": {}}}";

    // Инициализация cURL
    curl = curl_easy_init();
    if(curl) {
        // Установка URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9200/quittance/_delete_by_query");

        // Установка типа HTTP-запроса POST
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        // Установка тела запроса
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());

        // Установка заголовков
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Перенаправление вывода в пустую функцию 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

        // Выполнение запроса
        res = curl_easy_perform(curl);

        // Очистка
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}

std::string struct_to_JSON(quittance* Q, int n) {
    std::string Struct = "{\n    \"device\": \"";
    Struct += Q->device;
    Struct += "\",\n    \"num\": ";
    Struct += std::to_string(n);
    Struct += ",\n    \"serial_num\": \"";
    Struct += Q->serial_num;
    Struct += "\",\n    \"name\": \"";
    Struct += Q->name;
    Struct += "\",\n    \"date\": \"";
    Struct += d_to_jform(Q->date.str);
    Struct += "\",\n    \"status\": ";
    !strcmp(Q->status.str_stat ,"Выполнен") ? Struct += "true" : Struct += "false";
    Struct += ",\n    \"date_txt\": \"";
    Struct += Q->date.str;
    Struct += "\"\n}";
    return Struct;
}

void send_indexes(quittance* Q) {
    CURL *curl;
    CURLcode res;

    // Инициализация curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    // Перенаправление вывода в пустую функцию 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

    if(curl) {
        int count;
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        // Установка типа данных и заголовков
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        for(int i = 0; i < size; i++) {
            std::string url = "http://localhost:9200/quittance/_doc/" + std::to_string(i + 1);

            // Установка URL для добавления документа
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // Установка данных POST
            std::string json_data = struct_to_JSON(&Q[i], i + 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

            // Выполнение запроса
            res = curl_easy_perform(curl);
            
            // Проверка на ошибки 
            if(res != CURLE_OK) {
                std::cout << "Ошибка отправки данных!" << std::endl;
                return;
            } 
        }
        // Очистка
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void search_by_par() {
    CURL *curl;
    CURLcode res;

    std::cout << "Какой параметр вы хотите указать?\n" <<
        "1. Название прибора/имя заказчика.\n" <<
        "2. Дата приёма изделия.\n" <<
        "3. Серийный номер изделия.\n" <<
        "4. Поиск по всем параметрам.\n";
    int npar = int_input(1, 4);

    // Инициализация curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        std::string request, par = "";
        getchar();
        if(npar == 1) { //название прибора/имя заказчика
            std::cout << "Введите название прибора/имя заказчика:\n";
            while(par == "" || std::count(par.begin(), par.end(), ' ') == par.size())
                par = device_name_input(0);
            request = R"({
                "query": {
                    "multi_match": {
                    "query": ")" + par + R"(",
                    "fuzziness": "1",
                    "fields": ["name", "device"]
                    }
                }
            })";
        }
        else if(npar == 2) { //Дата
            std::cout << "Проводить поиск: 1)по месяцу/году;  2)по диапазону дат?\n";
            if(int_input(1, 2) == 1) {
                std::cout << "Введите номер месяца или год:\n";
                int date = int_input(0, 2024);
                if(date > 0 && date <= 12) {
                    request = R"({
                        "query": {
                            "bool": {
                            "filter": {
                                "script": {
                                "script": {
                                    "source": "doc['date'].value.monthValue == params.month",
                                    "params": {
                                    "month": )" + std::to_string(date) + R"(
                                    }
                                }
                                }
                            }
                            }
                        }
                    })";
                }
                else {
                    request = R"({
                        "query": {
                            "bool": {
                            "filter": {
                                "script": {
                                "script": {
                                    "source": "doc['date'].value.year == params.year",
                                    "params": {
                                    "year": )" + std::to_string(date) + R"(
                                    }
                                }
                                }
                            }
                            }
                        }
                    })";
                }
            }
            else {
                getchar();
                std::string date1 = "00.00.0000", date2 = "00.00.0000";
                while(d_int(date2) <= d_int(date1) || (date1 == "" || date2 == "")) {
                    std::cout << "Введите нижнюю границу (ДД.ММ.ГГГГ):\n";
                    date1 = date_input();
                    std::cout << "Введите верхнюю границу (ДД.ММ.ГГГГ):\n";
                    date2 = date_input();
                    if(d_int(date1) < d_int(date2)) break;
                    std::cout << "Неверно введённый диапазон!\n";
                }
                request = R"({
                    "query": {
                        "range": {
                            "date": {
                                "gte": ")" + d_to_jform(date1.c_str()) + R"(",
                                "lt": ")" + d_to_jform(date2.c_str()) + R"("
                            }
                        }
                    }
                })";
            }
        }
        else if(npar == 3) { //Серийный номер
            std::cout << "Введите серийный номер\n";
            while(par == "" || std::count(par.begin(), par.end(), ' ') == par.size())
                par = serial_num_input();
            request  = R"({
                "query": {
                    "match": {
                        "serial_num": ")" + par + R"("
                    }
                }
            })";
        }
        else{ //Все поля
            std::string str = "";
            std::cout << "Введите запрос:\n";
            while(str == "" || std::count(str.begin(), str.end(), ' ') == str.size()) {
                getline(std::cin, str);
            }
            request = R"({
                "query": {
                    "multi_match": {
                    "query": ")" + str + R"(",
                    "fuzziness": "1",
                    "fields": ["name", "device", "date_txt", "serial_num"]
                    }
                }
            })";
        }
        std::cout << std::endl;
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Установка URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9200/quittance/_search");

        // Установка типа данных
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Установка данных POST
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());

        std::ofstream file("res.json", std::ios::trunc);

        // Установка функции обратного вызова для записи данных в файл 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file); 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

        // Выполнение запроса
        res = curl_easy_perform(curl);
        
        // Проверка на ошибки
        if(res != CURLE_OK) {
            std::cout << "Ошибка поиска!" << std::endl;
        } 
        file.close();

        // Очистка
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void JSON_to_quitt() { //Преобразование JSON-вывода в квитанции
    std::string str;
    quittance Q;
    std::ifstream stream("res.json");

    getline(stream, str);
    if(stream.eof()) { //ПРоыерка найдены ли квитанции(если нет. указатель будет в конце файла)
        std::cout << "Квитанций с заданным параметром не обнаружено!\n\n";
        return;
    }

    while(!stream.eof()) { 
        //Название прибора
        stream.seekg(15, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        strcpy(Q.device, str.c_str());

        //Номер квитанции
        stream.seekg(11, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 1, 1);
        std::cout << "\nКвитанция №" << str << ":\n";     

        //Серийный номер
        stream.seekg(19, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        strcpy(Q.serial_num, str.c_str());

        //Имя заказчика
        stream.seekg(13, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        strcpy(Q.name, str.c_str());

        //Дата
        stream.seekg(13, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        std::string A = str.substr(8, 2) + "." + str.substr(5, 2) + "." + str.substr(0, 4);
        strcpy(Q.date.str, A.c_str());

        //Статус
        stream.seekg(14, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        str == "true" ? strcpy(Q.status.str_stat,"Выполнен\0") : strcpy(Q.status.str_stat,"Не выполнен\0");

        getline(stream, str);
        getline(stream, str);

        quitt_output(Q); //Вывод квитанций
        std::cout << std::endl;
    } 
}

//Функция с поиском для main'а
void elasticsearch_func(quittance* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима поиска квитанций...\n\n";
        return;
    }
    delete_old_docs();
    send_indexes(Q); //Квитанции индексируются и отправляются
    if(Check) {
        search_by_par(); //Поиск квитанций по заданному параметру
        std::cout << "Найденные по заданным параметрам покупатели:\n";
        return JSON_to_quitt(); //Вывод квитанций в нужном формате
    }
    else {
        Check = true;
        return elasticsearch_func(Q);
    }
}
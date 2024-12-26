#include "func.h"
int size = 0;
extern bool efile;
extern int del_size;

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
    std::cout << "Выберите механизм ввода данных:\n" <<
        "1. Ввод количества квитанций.\n" <<
        "2. Ввод до появления квитанции с заданным признаком.\n" <<
        "3. Диалог о необходимости продолжить ввод.\n";
    int option = int_input(1,3);
    if(option == 1) { //Ввод по количеству
        int N;
        quittance* array = NULL;
        while(array == NULL) {
            std::cout << "Введите количество квитанций:\n";
            N = int_input();
            size = N;
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
        for(int i = 0; i < N; i++) {
            std::cout << "Ввод квитанции №" << i + 1 << ":\n";
            getchar();
            quitt_input(array[i]);
        }
        std::cout << std::endl;
        return array;
    }
    else if(option == 2) { //Ввод по заданному параметру
        int npar = choose_npar();
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
            if(check(npar, par, array[size])) {
                size++;
                std::cout << "Выход из режима создания списка...\n\n";
                free(q);
                return array;
            }
            size++;
            q = (quittance*)realloc(array, (size + 1) * sizeof(quittance));
        }
    }
    else { //Диалог о необходимости продолжать ввод
        quittance* array = (quittance*)malloc(sizeof(quittance)), *q = array;
        getchar();
        while (true) {
            if(q == NULL) {
                std::cout << "К сожалению, достигнуто максимальное количество квитанций.\n Выход из режима создания списка...\n\n";
                return array;
            }
            array = q;
            std::cout << "Ввод квитанции №" << size + 1 << ":\n";
            quitt_input(array[size]);
            std::cout << "Продолжить ввод? (1 - да, 0 - нет)\n";
            if(!int_input(0,1)) {
                size++;
                return array;
            }
            getchar();
            size++;
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
        std::cout << "Добавить ещё одну квитанцию? (1 - да, 0 - нет):\n";
        if(!int_input(0, 1)) {
            if(efile) { //Проверка записывались ли данные в файл
                std::cout << "Внести изменения в бинарный файл? (1 - да, 0 - нет):\n";
                if(int_input()) add_in_file(Q, n);
                else efile = false;
            }
            std::cout << "Выход из режима добавления квитанций...\n\n";
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
    std::cout << "Осуществить поиск с возможностью частичного совпаденния при помощи elasticsearch?\n" <<
        "(1 - да, 0 - нет): ";
    int* num;
    bool el = int_input(0, 1);
    if (el) {
        num = elasticsearch_func(Q);
        std::cout << "Уверены, что хотите удалить эти квитанции? (1 - да, 0 - нет)\n";
        if(!int_input(0, 1) && size) {
            std::cout << "Выход из режима удаления квитанций...\n\n";
            return;
        }
        for(int i = 0; i < del_size; i++) {
            Q[num[i] - 1].device[0] = '*'; Q[num[i] - 1].device[1] = '\0';
        }
    }
    else {
        int npar = choose_npar();
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
        del_size = n;
    }
    if(del_size) {
        if(del_size == 1) {
            std::cout << "Была удалена квитанция с номером " << num[0];
        }
        else {
            std::cout << "Были удалены квитанции с номерами " << num[0];
            for(int i = 1; i < del_size; i++) std::cout << ", " << num[i];
        }
        std::cout << ".\n";
        for(int i = num[0] - 1; i < size - del_size; i++) { //Удаление квитанций
            if(!strcmp(Q[i].device, "*")) {
                int j = i + 1;
                while(!strcmp(Q[j].device, "*")) {
                    j++;
                }
                std::swap(Q[i], Q[j]);
            }
        }
        free(num);
        size -= del_size;
        Q = (quittance*)realloc(Q, size*sizeof(quittance));
        if(size) {
            std::cout << "Изменённый список квитанций:\n\n";
            show_quittances(Q);
        }
    }
}

void change_quitt(quittance* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима изменения квитанций...\n\n";
        return;
    }

    int npar = choose_npar();
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
        for(int j = i; j < size - 1; j++) {
            if(Q[j].date.itgr < Q[j + 1].date.itgr) {
                std::swap(Q[j], Q[j + 1]);
            }
        }
    }
    std::cout << "Список квитанций отсортирован!\n\n";
}


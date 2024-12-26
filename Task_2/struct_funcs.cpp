#include "func.h"
int size = 0;
extern bool efile;
extern int del_size;

int console_func() { //Консольное меню
    std::cout << "Выберите действия из предложенного списка:\n" <<
        "1. Создание базы покупателей.\n" <<
        "2. Просмотр списка покупателей.\n" << 
        "3. Добавление новых покупателей.\n" <<
        "4. Удаление из списка покупателей с заданным параметром.\n" <<
        "5. Изменение списка покупателей.\n" <<
        "6. Удаление повторяющихся по ФИО и адресу покупателей.\n" <<
        "7. Запись списка покупателей/внесение изменений в текстовый файл.\n" <<
        "8. Поиск структур по параметру при помощи elasticsearch.\n" <<
        "9. Завершение выполнения программы.\n";
    return int_input(1, 9);
}

//Вспомогательные функции
std::string par_choice(int npar) { //Ввод параметра по его номеру
    std::string par;
    if(npar == 1) {
        std::cout << "Введите фамилию:\n";
        par = name_input();
    }
    else if(npar == 2) {
        std::cout << "Введите имя:\n";
        par = name_input();            
    }
    else if(npar == 3) {
        std::cout << "Введите отчество:\n";
        par = name_input();            
    }
    else if(npar == 4) {
        std::cout <<  "Введите дату постановки на учёт (формат: ДД.ММ.ГГГГ):\n";
        par = date_input();            
    }
    else {
        par = "ул. ";
        std::cout << "Введите адрес:\nулица: ";
        par += name_input();
        par += ", д. ";
        std::cout << "дом:\n";
        par += std::to_string(int_input()).c_str();
   
    }
    return par;
}

int choose_npar() { //Выбор параметра
    std::cout << "Какой параметр вы хотите указать?\n" <<
        "1. Фамилия.\n" <<
        "2. Имя.\n" <<
        "3. Отчество.\n" <<
        "4. Дата постановки на учёт.\n" <<
        "5. Адрес.\n";
    return int_input(1, 5);
}

bool check(int npar, std::string par, queue& Q) { //Проверка совпадения поля структуры и параметра
    if(npar == 1 && Q.surname == par || 
       npar == 2 && Q.name == par || 
       npar == 3 && Q.midname == par || 
       npar == 4 && Q.date == par || 
       npar == 5 && Q.address == par)
    {
        return true;
    } 
    else return false;
}


//Фукнции работы со структурами
queue* create_list(queue* Q) {
    if(Q != NULL) {
        std::cout << "Ваш список инициализирован, вы можете отредактировать его. Если Вы продложите, старый список будет удалён.\n" <<
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
        "1. Ввод количества покупателей.\n" <<
        "2. Ввод до появления покупателя с заданным признаком.\n" <<
        "3. Диалог о необходимости продолжить ввод.\n";
    int option = int_input(1,3);
    if(option == 1) { //Ввод по количеству
        int N;
        queue* array = NULL;
        while(array == NULL) {
            std::cout << "Введите количество покупателей:\n";
            N = int_input();
            size = N;
            array = (queue*)malloc(N * sizeof(queue));
            if(array == NULL) {
                std::cout << "Ошибка выделения памяти! Попробуйте ввести меньшее количество покупателей.\n" <<
                    "Введите 1, чтобы попробовать заново, или 0, чтобы выйти из режима создания базы покупателей:\n";
                if(!int_input(0, 1)) {
                    std::cout << "Выход из режима создания списка...\n\n";
                    return array;
                }
            }
        }
        for(int i = 0; i < N; i++) {
            getchar();
            queue_input(array[i], i + 1);
        }
        std::cout << std::endl;
        return array;
    }
    else if(option == 2) { //Ввод по заданному параметру
        int npar = choose_npar();
        std::string par = par_choice(npar);
        queue* array = (queue*)malloc(sizeof(queue)), *q = array;
        while (true) {
            if(q == NULL) {
                std::cout << "К сожалению, достигнуто максимальное количество покупателей.\n Выход из режима создания списка...\n\n";
                return array;
            }
            array = q;
            queue_input(array[size], size + 1);
            if(check(npar, par, array[size])) {
                size++;
                std::cout << "Выход из режима создания списка...\n\n";
                free(q);
                return array;
            }
            size++;
            q = (queue*)realloc(array, (size + 1) * sizeof(queue));
        }
    }
    else { //Диалог о необходимости продолжать ввод
        queue* array = (queue*)malloc(sizeof(queue)), *q = array;
        getchar();
        while (true) {
            if(q == NULL) {
                std::cout << "К сожалению, достигнуто максимальное количество покупателей.\n Выход из режима создания списка...\n\n";
                return array;
            }
            array = q;
            queue_input(array[size], size + 1);
            std::cout << "Продолжить ввод? (1 - да, 0 - нет)\n";
            if(!int_input(0,1)) {
                size++;
                return array;
            }
            getchar();
            size++;
            q = (queue*)realloc(array, (size + 1) * sizeof(queue));
        }
    }
}

void show_queues(queue* Q) {
    if(Q == NULL) {
        std::cout << "Список покупателей не инициализирован. Выход из режима просмотра списка..\n\n";
        return;
    }
    for(int i = 0; i < size; i++) {
        queue_output(Q[i]);    
        std::cout << "\n";
    }
}

queue* add_queues(queue* Q) {
    if(Q == NULL) {
        std::cout << "Список покупателей не инициализирован. Выход из режима добавления покупателей...\n\n";
        return Q;
    }
    while (true) {
        queue* q = (queue*)realloc(Q, (size + 1) * sizeof(queue));
        if(q == NULL) {
            std::cout << "К сожалению, достигнуто максимальное количество покупателей.\n Выход из режима добавления покупателей...\n\n";
            return Q;
        }
        int n = size;
        Q = q;
        size++;
        std::cout << std::endl;
        queue_input(Q[size - 1], size);
        std::cout << "Добавить ещё одну квитанцию? (1 - да, 0 - нет):\n";
        if(!int_input(0, 1)) {
            if(efile) { //Проверка записывались ли данные в файл
                std::cout << "Внести изменения в бинарный файл? (1 - да, 0 - нет):\n";
                if(int_input(0, 1)) add_in_file(Q, n);
                else efile = false;
            }
            std::cout << "Выход из режима добавления квитанций...\n\n";
            return q;
        }
    }
}

void del_queue(queue* Q) {
    if(Q == NULL) {
        std::cout << "Список покупателей не инициализирован. Выход из режима удаления из списка...\n\n";
        return;
    }

    std::cout << "Осуществить поиск с возможностью частичного совпаденния при помощи elasticsearch?\n" <<
        "(1 - да, 0 - нет): ";
    int* num;
    bool el = int_input(0, 1);
    if (el) {
        num = elasticsearch_func(Q);
        for(int i = 0; i < del_size; i++) {
            Q[num[i] - 1].surname[0] = '*'; Q[num[i] - 1].surname[1] = '\0';
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
                num[n - 1] = i + 1; //Номера удаляемых покупателей
                Q[i].surname[0] = '*'; Q[i].surname[1] = '\0';
            }
        }
        del_size = n;
    }
    if(del_size) {
        if(del_size == 1) {
            std::cout << "Из списка был удалён покупатель с порядковым номером " << num[0];
        }
        else {
            std::cout << "Из списка были удалены покупатели с порядковыми номерами " << num[0];
            for(int i = 1; i < del_size; i++) std::cout << ", " << num[i];
        }
        std::cout << ".\n";
        for(int i = num[0] - 1; i < size - del_size; i++) { //Удаление покупателей
            if(Q[i].surname[0] == '*') {
                int j = i + 1;
                while(!strcmp(Q[j].surname, "*")) {
                    j++;
                }
                std::swap(Q[i], Q[j]);
                strcpy(Q[i].num.str, std::to_string(i + 1).c_str());
            }
        }
        free(num);
        size -= del_size;
        Q = (queue*)realloc(Q, size * sizeof(queue));
        if(size) {
            std::cout << "Изменённый список покупателей:\n\n";
            show_queues(Q);
        }
        del_size = 0;
    }
    else {
        if(!el) std::cout << "Покупателей с заданным параметром не найдено!\n\n";
    }
}

void change_queue(queue* Q) {
    if(Q == NULL) {
        std::cout << "Список покупателей не инициализирован. Выход из режима изменения списка...\n";
        return;
    }

    bool change = false;
    int npar = choose_npar();
    std::string par = par_choice(npar); //Параметр который надо изменить
    for(int i = 0; i < size; i++) {
        if(check(npar, par, Q[i])) {
            change = true;
        }
    }
    if(!change) { //Нет покупателей с данным параметром
        std::cout << "Покупателей с заданным параметром не найдено!\n\n";
        return;
    }
    std::cout << "Введите, на что заменить данный параметр:\n";
    std::string PAR = par_choice(npar);
    std::cout << "Изменённые покупатели:\n";
    for(int i = 0; i < size; i++) { //Замена параметров, вывод изменённых клиентов
        if(check(npar, par, Q[i])) {
            if(npar == 1) strcpy(Q[i].surname, PAR.c_str());
            else if(npar == 2) strcpy(Q[i].name, PAR.c_str());
            else if(npar == 3) strcpy(Q[i].midname, PAR.c_str());
            else if(npar == 4) strcpy(Q[i].date, PAR.c_str());
            else if(npar == 5) strcpy(Q[i].address, PAR.c_str());
            queue_output(Q[i]);
        }
    }
}

void del_rep_queue(queue* Q) { //Удаление повторяющихся
    int n = 0;
    int* num = (int*)malloc(sizeof(int));
    for(int i = 0; i < size - 1; i++) {
        if(Q[i].name[0] == '*') continue;
        for(int j = i + 1; j < size - 1; j++) {
            if(Q[i].surname == Q[j].surname &&
               Q[i].name == Q[j].name &&
               Q[i].midname == Q[j].midname &&
               Q[i].address == Q[j].address) 
            {
                n++;
                num = (int*)realloc(num, n * sizeof(int));
                num[n - 1] = i;
                Q[i].surname[0] = '*'; Q[i].surname[1] = '\0';
            }
        }
    }
    if(n) {
        if(n == 1) {
            std::cout << "Из списка был удалён покупатель с порядковым номером " << num[0] + 1;
        }
        else {
            std::cout << "Из списка были удалены покупатели с порядковыми номерами " << num[0] + 1;
            for(int i = 1; i < n; i++) std::cout << ", " << num[i] + 1;
        }
        std::cout << ".\n";
        for(int i = num[0]; i < size - n; i++) {
            if(!strcmp(Q[i].surname, "*")) {
                int j = i + 1;
                while(!strcmp(Q[j].surname, "*")) {
                    j++;
                }
                std::swap(Q[i], Q[j]);
                strcpy(Q[i].num.str, std::to_string(i + 1).c_str());
            }
        }
        size -= n;
        Q = (queue*)realloc(Q, size * sizeof(queue));
        std::cout << "Изменённый список покупателей:\n\n";
        show_queues(Q);
    }
    free(num);
} 

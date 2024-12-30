#include "func.h"

bool Check = false, file = false, efile = false;
std::string filename = "struct.txt";
int size = 0;

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


void format_str(std::string& str, bool rus) { //Приведение строки к виду Ааааа
    if(rus) { //если слово на русском
        int c = int(str[1]);
        if(c == -111 && int(str[0]) == -47) { //буква ё
            str[1] = char(-127);
            str[0]--;
        }
        else if(c >= -80 && c <= -65) str[1] = char(c - 32);
        else if(c >= -128 && c <= -113) {
            str[0]--;
            str[1] = char(c + 32);
        }
        for(int i = 3; i < str.length(); i += 2) {
            c = int(str[i]);
            if(c == -127 && int(str[i - 1]) == -48) { //буква ё
                str[i] = char(-111);
                str[i - 1]++;
            }
            else if(c >= -112 && c <= -97) str[i] = char(c + 32);
            else if(c >= -96 && c <= -81) {
                str[i] = char(c - 32);
                str[i - 1]++;
            }
        }
    }
    else { //слово на ангнлийском
        for(auto& c : str) {
            c = tolower(c);
        }
        str[0] = toupper(str[0]);
    }
}

std::string name_input() { //Ввод ФИО и улицы
    std::string name;
    bool rus = false;
    while (true) {
        int i;
        getline(std::cin, name);
        for(i = 0; i < name.length(); i++) {
            if((int) name[i] > 0 && !isalpha(name[i])) {
                std::cout << "Неверный формат ввода.\n";
                break;
            }
            else if((int)name[i] < 0) rus = true;
        }
        if(i == name.length() && name != "") {
            format_str(name, rus);
            return name;
        }
    }
}

std::string date_input() { //Ввод даты
    std::string d;
    int i;
    getline(std::cin, d);
    if(d.size() == 10 && d[2] == '.' && d[5] == '.') {
        for(i = 0; i < 10; i++) {
            if(i != 2 && i != 5 && !isdigit(d[i])) {
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


void queue_input(queue& Q, int n) { //Ввод покупателя
    std::string A;
    Q.num.itgr = n;
    std::cout << "Порядковый номер: " << n << "\n";
    strcpy(Q.num.str, std::to_string(Q.num.itgr).c_str());
    std::cout << "Фамилия (состоит только из букв):\n";
    strcpy(Q.surname, name_input().c_str());
    std::cout << "Имя (состоит только из букв):\n";
    strcpy(Q.name, name_input().c_str());
    std::cout << "Отчество (состоит только из букв):\n";
    strcpy(Q.midname, name_input().c_str());
    std::cout << "Дата постановки на учёт (формат: ДД.ММ.ГГГГ):\n";
    strcpy(Q.date, date_input().c_str());
    std::cout << "Адрес:\nулица: ";
    A = "ул. ";
    A += name_input().c_str();
    A +=  ", д. ";
    std::cout << "дом: ";
    A += std::to_string(int_input());
    strcpy(Q.address, A.c_str());
}

void queue_output(queue& Q) { //Вывод покупателя
    std::cout << "Порядковый номер: " << Q.num.str <<
            ".\nФамилия: " << Q.surname <<
            ".\nИмя: " << Q.name <<
            ".\nОтчество: " << Q.midname << 
            ".\nДата постановки на учёт: " << Q.date <<
            ".\nАдрес: " << Q.address << ".\n";
}



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
    int n = size;
    while (true) {
        queue* q = (queue*)realloc(Q, (size + 1) * sizeof(queue));
        if(q == NULL) {
            std::cout << "К сожалению, достигнуто максимальное количество покупателей.\n Выход из режима добавления покупателей...\n\n";
            return Q;
        }
        Q = q;
        size++;
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

    int* num, npar = choose_npar();
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
    if(n) {
        if(n == 1) {
            std::cout << "Из списка был удалён покупатель с порядковым номером " << num[0];
        }
        else {
            std::cout << "Из списка были удалены покупатели с порядковыми номерами " << num[0];
            for(int i = 1; i < n; i++) std::cout << ", " << num[i];
        }
        std::cout << ".\n";
        for(int i = num[0] - 1; i < size - n; i++) { //Удаление покупателей
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
        size -= n;
        Q = (queue*)realloc(Q, size * sizeof(queue));
        if(size) {
            std::cout << "Изменённый список покупателей:\n\n";
            show_queues(Q);
        }
        n = 0;
    }
    else {
        std::cout << "Покупателей с заданным параметром не найдено!\n\n";
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
        for(int j = i + 1; j < size; j++) {
            if(!strcmp(Q[i].surname, Q[j].surname) &&
               !strcmp(Q[i].name, Q[j].name) &&
               !strcmp(Q[i].midname, Q[j].midname) &&
               !strcmp(Q[i].address, Q[j].address))
            {
                n++;
                num = (int*)realloc(num, n * sizeof(int));
                num[n - 1] = j;
                Q[j].surname[0] = '*'; Q[j].surname[1] = '\0';
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
    else std::cout << "Повторяющихся покупателей не было обнаружено!\n\n";
    free(num);
} 



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
    std::ofstream stream(filename, std::ios::app);
    for(; n < size; n++) {
        stream << queue_to_string(Q[n]) << std::endl << std::endl;
    }
    stream.close();
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

void delete_old_docs() { //Удаление предыдущих структур
    CURL* curl;
    CURLcode res;

    // URL Elasticsearch
    const std::string url = "http://localhost:9200/queue/_delete_by_query";
    
    // Условие удаления всех документов
    const std::string query = "{\"query\": {\"match_all\": {}}}";

    // Инициализация cURL
    curl = curl_easy_init();
    if(curl) {
        // Установка URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

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

std::string struct_to_JSON(queue* Q, int n) {
    std::string Struct;
    Struct = "{\n    \"num\": ";
    Struct += std::to_string(n);
    Struct += ",\n    \"surname\": \"";
    Struct += Q->surname;
    Struct += "\",\n    \"name\": \"";
    Struct += Q->name;
    Struct += "\",\n    \"midname\": \"";
    Struct += Q->midname;
    Struct += "\",\n    \"date\": \"";
    Struct += d_to_jform(Q->date);
    Struct += "\",\n    \"address\": \"";
    Struct += Q->address;
    Struct += "\",\n    \"num_txt\": \"";
    Struct += std::to_string(n);
    Struct += "\",\n    \"date_txt\": \"";
    Struct += Q->date;
    Struct += "\"\n}";
    return Struct;
}

void send_indexes(queue* Q) {
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
            std::string url = "http://localhost:9200/queue/_doc/" + std::to_string(i + 1);

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
        "1. Порядковый номер.\n" <<
        "2. ФИО.\n" <<
        "3. Дата постановки на учёт.\n" <<
        "4. Улица\n" <<
        "5. Поиск по всем параметрам.\n";
    int npar = int_input(1, 5);

    // Инициализация curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init(); 
    if(curl) {
        std::string request, par;
        if(npar == 1) { //Порядковый номер
            int ngr = 1, vgr = 0;
            while(vgr - ngr < 0) {
                std::cout << "Введите нижнюю границу:\n";
                ngr = int_input();
                std::cout << "Введите верхнюю границу:\n";
                vgr = int_input(); 
                if(vgr - ngr >= 0) break;
                std::cout << "Неверно заданный диапазон!\n";
            }
            request = R"({
                "query": {
                    "range": {
                        "num": {
                            "gte": )" + std::to_string(ngr) + R"(,
                            "lte": )" + std::to_string(vgr) + R"(
                        }
                    }
                }
            })";
        }
        else if(npar == 2) { //ФИО
            std::cout << "Введите фамилию/имя/отчество:\n";
            par = name_input();
            request = R"({
                "query": {
                    "multi_match": {
                    "query": ")" + par + R"(",
                    "fuzziness": "1",
                    "fields": ["name", "surname", "midname"]
                    }
                }
            })";
        }
        else if(npar == 3) { //Дата
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
                while(d_int(date2) <= d_int(date1)) {
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
        else if(npar == 4) { //Улица
            std::cout << "Введите улицу:\n";
            par = name_input();
            request  = R"({
                "query": {
                    "match": {
                        "address": ")" + par + R"(",
                        "fuzziness": "1"
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
                    "fields": ["name", "surname", "midname", "date_txt", "num_txt", "address"]
                    }
                }
            })";
        }
        std::cout << std::endl;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Установка URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:9200/queue/_search");

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

void JSON_to_queue() { //Преобразование JSON-вывода в список
    std::string str;
    queue Q;
    std::ifstream stream("res.json");

    getline(stream, str);
    if(stream.eof()) { //Проверка найдены ли покупатели(если нет, указатель будет в конце файла)
        std::cout << "Покупателей с заданным параметром не обнаружено!\n\n";
        return;
    }

    while(!stream.eof()) { 
        //Порядковый номер
        stream.seekg(11, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 1, 1);
        strcpy(Q.num.str, str.c_str());

        //Фамилия
        stream.seekg(16, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        strcpy(Q.surname, str.c_str());

        //Имя
        stream.seekg(13, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        strcpy(Q.name, str.c_str());

        //Отчество
        stream.seekg(16, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        strcpy(Q.midname, str.c_str());

        //Дата
        stream.seekg(13, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        std::string A = str.substr(8, 2) + "." + str.substr(5, 2) + "." + str.substr(0, 4);
        strcpy(Q.date, A.c_str());

        //Адрес
        stream.seekg(16, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        strcpy(Q.address, str.c_str());

        getline(stream, str);
        getline(stream, str);
        getline(stream, str);

        queue_output(Q); //Вывод списка покупателей
        std::cout << std::endl;
    }
}

//Функция с поиском для main'а
void elasticsearch_func(queue* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима поиска квитанций...\n\n";
        return;
    }
    delete_old_docs();
    send_indexes(Q); //Покупатели индексируются и отправляются
    if(Check) {
        search_by_par(); //Поиск покупателей по заданному параметру
        std::cout << "Найденные по заданным параметрам покупатели:\n\n";
        JSON_to_queue(); //Вывод списка в нужном формате
    }
    else {
        Check = true;
        elasticsearch_func(Q);
    }
}

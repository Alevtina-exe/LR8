#include "func.h"
#include <fstream>
#include <curl/curl.h>
extern int size;
bool check = false;
int del_size;

//Вспомогательные функции для параметров
std::string elastic_choose_npar() { //Выбор параметра
    std::cout << "Какой параметр вы хотите указать?\n" <<
        "1. Порядковый номер.\n" <<
        "2. Фамилия.\n" <<
        "3. Имя.\n" <<
        "4. Отчество.\n" <<
        "5. Дата постановки на учёт.\n" <<
        "6. Адрес.\n";
    int npar = int_input(1, 6);
    if(npar == 1) return "num";
    else if(npar == 2) return "surname";
    else if(npar == 3) return "name";
    else if(npar == 4) return "midname";
    else if(npar == 5) return "date";
    else return "address";
}
std::string elastic_par_choice(std::string& npar) { //Ввод параметра по его номеру
    std::string par;
    getchar();
    if(npar == "num") {
        std::cout << "Введите порядковый номер:\n";
        par = std::to_string(int_input());
    }
    else if(npar == "surname") {
        std::cout << "Введите фамилию:\n";
        par = name_input();
    }
    else if(npar == "name") {
        std::cout << "Введите имя:\n";
        par = name_input();            
    }
    else if(npar == "midname") {
        std::cout << "Введите отчество:\n";
        par = name_input();            
    }
    else if(npar == "date") {
        std::cout <<  "Введите дату постановки на учёт (формат: ДД.ММ.ГГГГ):\n";
        par = date_input();            
    }
    else {
        std::cout << "Введите улицу:\n";
        par = name_input();
   
    }
    return par;
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
    Struct = "{\n    \"num\": \"";
    Struct += std::to_string(n);
    Struct += "\",\n    \"surname\": \"";
    Struct += Q->surname;
    Struct += "\",\n    \"name\": \"";
    Struct += Q->name;
    Struct += "\",\n    \"midname\": \"";
    Struct += Q->midname;
    Struct += "\",\n    \"date\": \"";
    Struct += Q->date;
    Struct += "\",\n    \"address\": \"";
    Struct += Q->address;
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

void search_by_par(std::string par, std::string npar) {
    CURL *curl;
    CURLcode res;

    // Инициализация curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init(); 
    if(curl) {
        std::string request  = R"({
            "query": {
                "match": {
                    ")" + npar + R"(": ")" + par + R"("
                }
            }
        })";

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

int* JSON_to_queue() { //Преобразование JSON-вывода в список
    std::string str;
    queue Q;
    int* del_list = (int*)malloc(sizeof(int)), n = 0;
    std::ifstream stream("res.json");

    getline(stream, str);
    if(stream.eof()) { //Проверка найдены ли покупатели(если нет, указатель будет в конце файла)
        std::cout << "Покупателей с заданным параметром не обнаружено!\n\n";
        return NULL;
    }
    while(!stream.eof()) { 
        //Порядковый номер
        stream.seekg(12, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 2, 2);
        strcpy(Q.num.str, str.c_str());
        del_list = (int*)realloc(del_list, (n + 1) * sizeof(int));
        del_list[n] = stoi(str); n++;

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
        strcpy(Q.date, str.c_str());

        //Адрес
        stream.seekg(16, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 1, 1);
        strcpy(Q.address, str.c_str());
        getline(stream, str);

        queue_output(Q); //Вывод списка покупателей
    }
    std::cout << std::endl << std::endl;
    del_size = n;
    return del_list;
}

//Функция с поиском для main'а
int* elasticsearch_func(queue* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима поиска квитанций...\n\n";
        return NULL;
    }
    delete_old_docs();
    send_indexes(Q); //Покупатели индексируются и отправляются
    if(check) {
        std::string npar = elastic_choose_npar();
        std::string par = elastic_par_choice(npar); //Задаётся параметр
        search_by_par(par, npar); //Поиск покупателей по заданному параметру
        return JSON_to_queue(); //Вывод списка в нужном формате
    }
    else {
        check = true;
        return elasticsearch_func(Q);
    }
}

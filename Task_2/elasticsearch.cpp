#include "func.h"
#include <fstream>
#include <curl/curl.h>
extern int size;
bool check = false;

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
    Struct += Q->date;
    Struct += "\",\n    \"address\": \"";
    Struct += Q->address;
    Struct += "\"\n}";
    return Struct;
}

// Пустая функция для подавления вывода 
size_t write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) { 
    return size * nmemb; 
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
            std::cout << json_data << std::endl;
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

std::string npar_to_par(int npar) {
    if(npar == 1) return "num";
    else if(npar == 2) return "surname";
    else if(npar == 3) return "name";
    else if(npar == 4) return "midname";
    else if(npar == 5) return "date";
    else return "address";
}

// Функция для записи данных в файл 
size_t write_to_file(void* ptr, size_t size, size_t nmemb, std::ofstream* stream) { 
    stream->write((char*)ptr, size * nmemb); 
    return size * nmemb; 
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

//Преобразование JSON-вывода в список
void JSON_to_queue() {
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
        strcpy(Q.date, str.c_str());

        //Адрес
        stream.seekg(16, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 1, 1);
        strcpy(Q.address, str.c_str());
        getline(stream, str);

        queue_output(Q); //Вывод списка покупателей
    }
    std::cout << std::endl;
}

//Функция с поиском для main'а
void elasticsearch_func(queue* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима поиска квитанций...\n\n";
        return;
    }

    send_indexes(Q); //Покупатели индексируются и отправляются
    if(check) {
        int npar = choose_npar();
        std::string par = par_choice(npar); //Задаётся параметр
        std::string NPAR = npar_to_par(npar);
        search_by_par(par, NPAR); //Поиск покупателей по заданному параметру
        JSON_to_queue(); //Вывод списка в нужном формате
    }
    else {
        check = true;
        elasticsearch_func(Q);
    }
}







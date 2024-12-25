#include "func.h"
#include <fstream>
#include <curl/curl.h>
extern int size;
bool check;

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
    Struct += Q->date.str;
    Struct += "\",\n    \"status\": ";
    !strcmp(Q->status.str_stat ,"Выполнен") ? Struct += "true" : Struct += "false";
    Struct += "\n}";
    return Struct;
}

// Пустая функция для подавления вывода 
size_t write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) { 
    return size * nmemb; 
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

std::string npar_to_par(int npar) {
    if(npar == 1) return "device";
    else if(npar == 2) return "serial_num";
    else if(npar == 3) return "name";
    else if(npar == 4) return "date";
    else return "status";
}

// Функция для записи данных в файл 
size_t write_to_file(void* ptr, size_t size, size_t nmemb, std::ofstream* stream) { 
    stream->write((char*)ptr, size * nmemb); 
    return size * nmemb; 
}

void search_by_par(std::string& par, std::string& npar) {
    CURL *curl;
    CURLcode res;

    // Инициализация curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(npar == "status") par == "Выполнен" ? par = "true" : par = "false"; 
    else {
        par.insert(0, "\"");
        par.insert(par.size(), "\"");
        std::cout << par << std::endl;
    }
    if(curl) {
        std::string request  = R"({
            "query": {
                "match": {
                    ")" + npar + R"(": )" + par + R"(
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

//Преобразование JSON-вывода в квитанции
void JSON_to_quitt() {
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
        strcpy(Q.date.str, str.c_str());

        //Статус
        stream.seekg(14, std::ios::cur);
        getline(stream, str);
        str.erase(str.size() - 1, 1);
        str == "true" ? strcpy(Q.status.str_stat,"Выполнен\0") : strcpy(Q.status.str_stat,"Не выполнен\0");
        getline(stream, str);

        quitt_output(Q); //Вывод квитанций
    }
    std::cout << std::endl;
}

//Функция с поиском для main'а
void elasticsearch_func(quittance* Q) {
    if(Q == NULL) {
        std::cout << "Список квитанций не инициализирован. Выход из режима поиска квитанций...\n\n";
        return;
    }
    send_indexes(Q); //Квитанции индексируются и отправляются
    if(check) {
        int npar = choose_npar();
        std::string par = par_choice(npar); //Задаётся параметр
        std::string NPAR = npar_to_par(npar);
        search_by_par(par, NPAR); //Поиск квитанций по заданному параметру
        JSON_to_quitt(); //Вывод квитанций в нужном формате
    }
    else {
        check = true;
        elasticsearch_func(Q);
    }
}







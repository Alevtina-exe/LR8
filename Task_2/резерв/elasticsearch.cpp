#include "func.h"

bool check = false;


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
                        "address": ")" + par + R"("
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
    if(check) {
        search_by_par(); //Поиск покупателей по заданному параметру
        std::cout << "Найденные по заданным параметрам покупатели:\n\n";
        JSON_to_queue(); //Вывод списка в нужном формате
    }
    else {
        check = true;
        elasticsearch_func(Q);
    }
}

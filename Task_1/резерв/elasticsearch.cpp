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
        std::string request, par;
        if(npar == 1) { //название прибора/имя заказчика
            std::cout << "Введите название прибора/имя заказчика:\n";
            par = device_name_input(0);
            request = R"({
                "query": {
                    "multi_match": {
                    "query": ")" + par + R"(",
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
        else if(npar == 3) { //Серийный номер
            std::cout << "Введите серийный номер\n";
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
    if(check) {
        search_by_par(); //Поиск квитанций по заданному параметру
        std::cout << "Найденные по заданным параметрам покупатели:\n";
        return JSON_to_quitt(); //Вывод квитанций в нужном формате
    }
    else {
        check = true;
        return elasticsearch_func(Q);
    }
}
#include "func.h"

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
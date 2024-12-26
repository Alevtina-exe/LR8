#include <string.h>
#include "func.h"
#include <algorithm>

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
        if(i == dev.length() && dev != "" && 
           std::count(dev.begin(), dev.end(), ' ') != dev.size()) return dev;
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
        if(i == num.length() && num != "") {
            return num;
        }
    }
}
std::string date_input() { //Ввод даты
    std::string d;
    int i;
    getline(std::cin, d);
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
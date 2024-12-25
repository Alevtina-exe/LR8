#pragma once
#include <iostream>
#include <cstring>
#include "struct.h"

//консольное меню
int console_func();

//Вспомогательные функции
std::string par_choice(int npar);
int choose_npar();

//проверка ввода
int int_input(int min, int max);
int int_input();

std::string device_name_input(bool name);
std::string serial_num_input();
std::string date_input();

void quitt_input(quittance& Q);
void quitt_output(quittance& Q);


//функции со структурами
quittance* create_list(quittance* Q);
void show_quittances(quittance* Q);
quittance* add_quittances(quittance* Q);
void display_par_quitt(quittance* Q);
void del_quitt(quittance* Q);
void change_quitt(quittance* Q);
void sort_quitt(quittance* Q);


//работа с файлами
void file_write(quittance* Q);
quittance* recover_from_file(quittance* Q);
void add_in_file(quittance* Q, int n);
void edit_in_file(quittance* Q, int n, int npar, const char* par);

//Работа с elasticsearch
void elasticsearch_func(quittance* Q);
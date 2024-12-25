#pragma once
#include <iostream>
#include <cstring>
#include "struct.h"

//Консольно меню
int console_func();

//Проверка ввода
int int_input(int min, int max);
int int_input();
std::string name_input();
std::string date_input();

//Работа с полями
int choose_npar();
std::string par_choice(int npar);

//Работа со структурами
void queue_input(queue& Q, int n);
void queue_output(queue& Q);
void show_queues(queue* Q);
queue* add_queues(queue* Q);
void del_queue(queue* Q);
void change_queue(queue* Q);
void del_rep_queue(queue* Q);
queue* create_list(queue* Q);

//Работа с файлами
void add_in_file(queue* Q, int n);
void file_write(queue* Q);

//Работа с elasticsearch
void elasticsearch_func(queue* Q); 
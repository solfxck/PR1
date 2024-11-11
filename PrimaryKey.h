#pragma once
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

class PrimaryKey {
public:
    static int nextPrimaryKey(const string& tableName); // получение следующего первичного ключа для таблицы
private:
    static string pathFile(const string& tableName); // получение полного пути к файлу последовательности первичных ключей
};
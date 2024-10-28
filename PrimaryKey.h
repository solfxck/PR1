#include <string>
#pragma once

using namespace std;

class PrimaryKey {
public:
    static int getNextPrimaryKey(const string& tableName); // получение следующего первичного ключа для таблицы
private:
    static string pkSequenceFilePath(const string& tableName); // получение полного пути к файлу последовательности первичных ключей
};
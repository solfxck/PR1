#include <string>
#pragma once

using namespace std;

class lockTable {
public:
    static bool lockTablee(const string& tableName); // блокировка таблицы
    static bool unlockTable(const string& tableName); // разблокировка таблицы
    static bool isTableLocked(const string& tableName); // проверка, заблокирована ли таблица
private:
    static string lockFilePath(const string& tableName); // получение имени файла блокировки
};
#pragma once
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

class LockTable {
public:
    static bool lockTable(const string& tableName);
    static bool unlockTable(const string& tableName);
    static bool isTableLocked(const string& tableName); // проверка, заблокирована ли таблица
private:
    static string pathLockFile(const string& tableName); // получение имени файла блокировки
};
#include "lockTable.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

string lockTable::lockFilePath(const string& tableName) {
    return "scheme/" + tableName + "/" + tableName + "_lock.txt";
}

// блокировка таблицы
bool lockTable::lockTablee(const string& tableName) {
    string lockFile = lockFilePath(tableName);
    ifstream lockFileStream(lockFile);
    int lockStatus = 0;
    lockFileStream >> lockStatus;
    lockFileStream.close();

    if (lockStatus == 1) {
        return false; // таблица уже заблокирована
    }

    ofstream lockFileOutStream(lockFile);
    if (lockFileOutStream.is_open()) {
        lockFileOutStream << "1";
        lockFileOutStream.close();
        return true;
    }

    return false; // не удалось создать файл блокировки
}

// разблокировка таблицы
bool lockTable::unlockTable(const string& tableName) {
    string lockFile = lockFilePath(tableName);
    ifstream lockFileStream(lockFile);
    int lockStatus = 0;
    lockFileStream >> lockStatus;
    lockFileStream.close();

    if (lockStatus == 0) {
        return false; // таблица уже разблокирована
    }

    ofstream lockFileOutStream(lockFile);
    if (lockFileOutStream.is_open()) {
        lockFileOutStream << "0";
        lockFileOutStream.close();
        return true;
    }

    return false; // не удалось изменить файл блокировки
}

// проверка, заблокирована ли таблица
bool lockTable::isTableLocked(const string& tableName) {
    string lockFile = lockFilePath(tableName);
    ifstream lockFileStream(lockFile);
    int lockStatus = 0;
    lockFileStream >> lockStatus;
    lockFileStream.close();

    return lockStatus == 1;
}
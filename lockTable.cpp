#include "lockTable.h"

namespace fs = std::filesystem;

string LockTable::pathLockFile(const string& tableName) {
    return "scheme/" + tableName + "/" + tableName + "_lock.txt";
}

bool LockTable::lockTable(const string& tableName) {
    string lockFile = pathLockFile(tableName);
    ifstream lockFileStream(lockFile); // открываем файл для чтения
    int lockStatus = 0; // переменная для хранения текущего статуса блокировки
    lockFileStream >> lockStatus; // считываем текущий статус блокировки из файла
    lockFileStream.close(); // закрываем файл после чтения

    if (lockStatus == 1) {
        return false; // таблица уже заблокирована
    }

    ofstream lockFileOutStream(lockFile); // открываем файл для записи
    if (lockFileOutStream.is_open()) {
        lockFileOutStream << "1"; // записываем статус блокировки "1" (заблокировано)
        lockFileOutStream.close(); // закрываем файл после записи
        return true; // успешно заблокировали таблицу
    }

    return false; // не удалось создать файл блокировки
}

bool LockTable::unlockTable(const string& tableName) {
    string lockFile = pathLockFile(tableName);
    ifstream lockFileStream(lockFile);
    int lockStatus = 0;
    lockFileStream >> lockStatus;
    lockFileStream.close();

    if (lockStatus == 0) {
        return false;  // таблица уже разблокирована
    }

    ofstream lockFileOutStream(lockFile);
    if (lockFileOutStream.is_open()) {
        lockFileOutStream << "0";  // записываем статус блокировки "0" (разблокировано)
        lockFileOutStream.close();
        return true;  // успешно разблокировали таблицу
    }

    return false;  // не удалось изменить файл блокировки
}

// заблокирована ли таблица?
bool LockTable::isTableLocked(const string& tableName) {
    string lockFile = pathLockFile(tableName);
    ifstream lockFileStream(lockFile);
    int lockStatus = 0; 
    lockFileStream >> lockStatus;  // считываем текущий статус блокировки из файла
    lockFileStream.close();

    return lockStatus == 1;  // возвращаем true, если таблица заблокирована, иначе false
}
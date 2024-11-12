#include "primaryKey.h"

// путь к файлу с ключем
string PrimaryKey::pathFile(const string& tableName) {
    return "Scheme/" + tableName + "/" + tableName + "_pk_sequence.txt";
}

// возвращает следующий первичный ключ для заданной таблицы
int PrimaryKey::nextPrimaryKey(const string& tableName) {
    string pkFilePath = pathFile(tableName);
    ifstream pkFile(pkFilePath);  // открываем файл для чтения
    int currentPk;  // для текуще. ключ
    pkFile >> currentPk;  // считываем текущий ключ из файла
    pkFile.close();  // закрываем файл

    ofstream pkFileOut(pkFilePath);  // открываем файл для записи
    pkFileOut << (currentPk + 1) << endl;  // записываем след. ключ (текущий + 1)
    pkFileOut.close();

    return currentPk;  // ключ (который был считан из файла)
}
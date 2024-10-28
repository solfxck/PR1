#include "PrimaryKey.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
using namespace std;

string PrimaryKey::pkSequenceFilePath(const string& tableName) {
    return "scheme/" + tableName + "/" + tableName + "_pk_sequence.txt";
}

// получение следующего первичного ключа для таблицы
int PrimaryKey::getNextPrimaryKey(const string& tableName) {
    string pkSequenceFile = pkSequenceFilePath(tableName);
    int currentKey = 0;

    // проверка существования файла последовательности
    if (fs::exists(pkSequenceFile)) {
        ifstream inFile(pkSequenceFile);
        inFile >> currentKey;
        inFile.close();
    }

    // увеличение ключа
    currentKey++;

    // запись нового значения ключа в файл
    ofstream outFile(pkSequenceFile);
    if (outFile.is_open()) {
        outFile << currentKey;
        outFile.close();
    } else {
        // Обработка ошибки открытия файла
        cout << "Не удалось открыть файл последовательности первичных ключей для записи." << endl;
    }

    return currentKey;
}
#include "database.h"

namespace fs = std::filesystem;

string Database::schemaName; // статич. переменные
int Database::tuplesLimit;

void Database::createDirectoryStructure(const json& config) {
    schemaName = config["name"];  // получаем имя схемы из конфигурации
    tuplesLimit = config["tuples_limit"];  // получаем лимит строк из конфигурации

    // создаем директорию схемы, если она не существует
    if (!fs::exists(schemaName)) {
        fs::create_directory(schemaName);
    }

    // создаем директории и файлы для каждой таблицы
    for (const auto& table : config["structure"].items()) {
        string tableName = table.key();  // получаем имя таблицы
        string tableDir = schemaName + "/" + tableName;  // формируем путь к директории таблицы

        // создаем директорию таблицы, если она не существует
        if (!fs::exists(tableDir)) {
            fs::create_directory(tableDir);
        }

        // создаем файлы для таблицы, если они не существуют
        string csvFilePath = tableDir + "/" + tableName + ".csv";  // путь к CSV файлу
        string lockFilePath = tableDir + "/" + tableName + "_lock.txt";  // путь к файлу блокировки
        string pkFilePath = tableDir + "/" + tableName + "_pk_sequence.txt";  // путь к файлу последовательности ключей

        if (!fs::exists(csvFilePath)) {
            ofstream csvFile(csvFilePath);  // открываем CSV файл для записи

            // добавляем колонку первичного ключа
            List columns;
            columns.pushHead(tableName + "_pk");
            for (const auto& column : table.value()) {
                columns.pushTail(column);  // добавляем столбцы из конфигурации
            }

            // записываем заголовок в CSV файл
            Node1* current = columns.head;
            while (current != nullptr) {
                csvFile << current->data;
                if (current->next != nullptr) {
                    csvFile << ",";
                }
                current = current->next;
            }
            csvFile << endl;
            csvFile.close();  // закрываем CSV файл
        }

        if (!fs::exists(lockFilePath)) {
            ofstream lockFile(lockFilePath);  // открываем файл блокировки для записи
            lockFile << "0" << endl;  // записываем начальное состояние блокировки
            lockFile.close();  // закрываем файл блокировки
        }

        if (!fs::exists(pkFilePath)) {
            ofstream pkFile(pkFilePath);
            pkFile << "1" << endl;
            pkFile.close();
        }
    }
}

void Database::insertTable(const string& tableName, const List& values) {
    if (LockTable::isTableLocked(tableName)) {  // проверяем, заблокирована ли таблица
        cout << "Таблица заблокирована!" << endl;
        return;
    }

    string tableDir = schemaName + "/" + tableName;  // формируем путь к директории таблицы

    // проверяем количество значений
    int columnCount = 0;
    Node1* current = values.head;
    while (current != nullptr) {
        columnCount++;
        current = current->next;
    }

    // получаем количество столбцов в таблице
    ifstream configFile("schema.json");
    json config;
    configFile >> config;
    int tableColumnCount = config["structure"][tableName].size();

    if (columnCount != tableColumnCount) {  // проверяем соответствие количества значений и столбцов
        cout << "Количество значений не соответствует количеству столбцов в таблице!" << endl;
        return;
    }

    // находим последний файл CSV
    int fileNumber = 1;
    string csvFilePath = tableDir + "/" + tableName + ".csv";  // путь к основному CSV файлу
    if (!fs::exists(csvFilePath)) {
        ofstream newCsvFile(csvFilePath);  // создаем основной CSV файл, если он не существует
        newCsvFile.close();
    }

    ifstream csvFile(csvFilePath);
    string line;
    int lineCount = 0;

    // считаем количество строк в последнем файле
    while (getline(csvFile, line)) {
        lineCount++;
    }
    csvFile.close();

    // если файл заполнен, создаем новый файл
    if (lineCount >= Database::getTuplesLimit()) {
        fileNumber++;
        csvFilePath = tableDir + "/" + tableName + "_" + to_string(fileNumber) + ".csv";  // путь к новому CSV файлу
        if (!fs::exists(csvFilePath)) {
            ofstream newCsvFile(csvFilePath);  // создаем новый CSV файл, если он не существует
            newCsvFile.close();
        }
    }

    // получаем следующий первичный ключ только в случае успешной вставки
    int nextPk = PrimaryKey::nextPrimaryKey(tableName);

    // добавляем новую строку в файл
    ofstream csvFileOut(csvFilePath, ios::app);  // открываем CSV файл для добавления строки
    csvFileOut << nextPk;  // записываем первичный ключ
    current = values.head;
    while (current != nullptr) {
        csvFileOut << "," << current->data;  // записываем значения столбцов
        current = current->next;
    }
    csvFileOut << endl;
    csvFileOut.close();  // закрываем CSV файл
}

/*
void Database::selectTables() {
}

void Database::deleteTables() {
}
*/
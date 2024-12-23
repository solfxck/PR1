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

// для проверки условий
bool checkConditions(const List& row, const List& availableColumns, const List& conditions) {
    if (conditions.head == nullptr) return true;  // если нет условий, возвращаем true

    bool result = true;  // для AND начинаем с true
    bool orMode = false; // флаг для OR операции
    Node1* condNode = conditions.head; // указатель на текущий условие
    
    while (condNode != nullptr) {
        string condition = condNode->data; // текущее условие
        
        // разбираем условие
        size_t opPos = condition.find('=');
        size_t andPos = condition.find('&');
        size_t orPos = condition.find('|');
        
        // получаем имя колонки и убираем префикс таблицы
        string fullColumnName = condition.substr(0, opPos);
        size_t dotPos = fullColumnName.find('.');
        string columnName = fullColumnName.substr(dotPos + 1);

        // получаем значение
        string value = condition.substr(opPos + 1, 
            min(andPos == string::npos ? condition.length() : andPos,
                orPos == string::npos ? condition.length() : orPos) - (opPos + 1));
                
        // находим индекс колонки
        Node1* colNode = availableColumns.head;
        int colIndex = 0;
        while (colNode != nullptr) {
            if (colNode->data == columnName) break;
            colIndex++;
            colNode = colNode->next;
        }
        
        // получаем значение из строки
        Node1* rowNode = row.head;
        for (int i = 0; i < colIndex && rowNode != nullptr; i++) {
            rowNode = rowNode->next;
        }
        
        bool conditionResult = (rowNode != nullptr && rowNode->data == value);
        
        // применяем условие
        if (orMode) {
            result = result || conditionResult;
            orMode = false;
        } else {
            result = result && conditionResult;
        }
        
        // проверяем следующий оператор
        if (orPos != string::npos) {
            orMode = true;
        }
        
        condNode = condNode->next; // переходим к следующему условию
    }
    
    return result;
}

void Database::selectTable(const SelectQuery& query) {
    // проверяем, что есть таблицы для выборки
    if (query.tables.head == nullptr) {
        cerr << "Ошибка: Не указаны таблицы для выборки!" << endl;
        return;
    }

    // проверяем существование таблиц в схеме
    ifstream schemaFile("schema.json");
    if (!schemaFile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл схемы 'schema.json'!" << endl;
        return;
    }

    json schemaData;
    schemaFile >> schemaData;
    schemaFile.close();

    // список для хранения данных каждой таблицы
    HashTableList tableDataList; // список для хранения данных каждой таблицы
    HashTableList tableColumnNames; // список для хранения названий колонок каждой таблицы
    HashTableList tableSelectedColumnIndexes; // список для хранения индексов выбранных колонок
    List tableNamesList; // список для хранения названий таблиц

    // проходим по всем таблицам в запросе
    Node1* tableNode = query.tables.head;
    while (tableNode != nullptr) {
        string tableName = tableNode->data;
        string tableDir = schemaName + "/" + tableName;

        // проверка существования таблицы в схеме
        if (schemaData["structure"].find(tableName) == schemaData["structure"].end()) {
            cerr << "Ошибка: Таблица '" << tableName << "' не существует в схеме!" << endl;
            return;
        }

        // список результатов для текущей таблицы
        List results; // список для хранения результатов
        List availableColumns; // список доступных колонок
        List selectedColumnIndexes; // список индексов выбранных колонок
        List selectedColumnNames; // список названий выбранных колонок

        // Находим CSV файл таблицы
        bool csvFound = false;
        for (const auto& entry : fs::directory_iterator(tableDir)) {
            if (entry.path().extension() == ".csv") {
                csvFound = true;
                ifstream csvFile(entry.path());
                if (!csvFile.is_open()) {
                    cerr << "Ошибка: Не удалось открыть CSV файл!" << endl;
                    return;
                }

                string header;
                getline(csvFile, header);  // Читаем заголовок

                // Проверяем, что нужные колонки есть в таблице
                stringstream headerStream(header); // поток для чтения заголовка
                string columnName;
                int columnIndex = 0;
                while (getline(headerStream, columnName, ',')) {
                    availableColumns.pushTail(columnName);
                    columnIndex++;
                }

                // проверяем корректность выбранных колонок
                Node1* queryColumnNode = query.columns.head;
                while (queryColumnNode != nullptr) {
                    // разбираем имя колонки
                    size_t dotPos = queryColumnNode->data.find('.');
                    string queryTableName = queryColumnNode->data.substr(0, dotPos);
                    string queryColumnName = queryColumnNode->data.substr(dotPos + 1);

                    // проверка соответствия таблицы и колонки
                    if (queryTableName != tableName) {
                        // пропускаем колонки из других таблиц
                        queryColumnNode = queryColumnNode->next;
                        continue;
                    }

                    // проверка существования колонки в схеме
                    bool columnExists = false;
                    for (const auto& schemaColumn : schemaData["structure"][tableName]) {
                        if (schemaColumn == queryColumnName) {
                            columnExists = true;
                            break;
                        }
                    }

                    if (!columnExists) {
                        cerr << "Ошибка: Колонка '" << queryColumnName 
                             << "' не существует в таблице '" << tableName << "'!" << endl;
                        return;
                    }

                    // находим индекс колонки
                    Node1* availColumnNode = availableColumns.head;
                    int searchIndex = 0;
                    bool columnFound = false;
                    while (availColumnNode != nullptr) {
                        if (availColumnNode->data == queryColumnName) {
                            selectedColumnIndexes.pushTail(to_string(searchIndex));
                            selectedColumnNames.pushTail(queryColumnNode->data);
                            columnFound = true;
                            break;
                        }
                        availColumnNode = availColumnNode->next;
                        searchIndex++;
                    }

                    if (!columnFound) {
                        cerr << "Ошибка: Колонка '" << queryColumnName 
                             << "' не найдена в CSV файле!" << endl;
                        return;
                    }

                    queryColumnNode = queryColumnNode->next;
                }

                // читаем и сохраняем строки
                string line;
                while (getline(csvFile, line)) {
                    List row;
                    stringstream lineStream(line);
                    string value;
                    while (getline(lineStream, value, ',')) {
                        row.pushTail(value);
                    }
                    results.pushTail(row.toString());
                }
                csvFile.close();
                break; // обрабатываем только первый CSV файл
            }
        }

        if (!csvFound) {
            cerr << "Ошибка: Не найден CSV файл для таблицы '" << tableName << "'!" << endl;
            return;
        }

        // сохраняем данные таблицы
        tableDataList.insert(tableName, results);
        tableColumnNames.insert(tableName, selectedColumnNames);
        tableSelectedColumnIndexes.insert(tableName, selectedColumnIndexes);
        tableNamesList.pushTail(tableName);

        tableNode = tableNode->next;
    }

    // выводим заголовки выбранных колонок
    Node1* tableNameNode = tableNamesList.head;
    while (tableNameNode != nullptr) {
        List selectedColumnNames = tableColumnNames.get(tableNameNode->data);
        Node1* headerColumnNode = selectedColumnNames.head;
        while (headerColumnNode != nullptr) {
            cout << headerColumnNode->data << "\t";
            headerColumnNode = headerColumnNode->next;
        }
        tableNameNode = tableNameNode->next;
    }
    cout << endl;

    // обработка запросов с одной таблицей
    if (tableNamesList.length() == 1) {
        string tableName = tableNamesList.head->data;

        const List& tableData = tableDataList.get(tableName);
        const List& tableColumnIndexes = tableSelectedColumnIndexes.get(tableName);

        // получаем список всех доступных колонок для таблицы
        string tableDir = schemaName + "/" + tableName;
        List availableColumns;
        
        // находим CSV файл таблицы
        for (const auto& entry : fs::directory_iterator(tableDir)) {
            if (entry.path().extension() == ".csv") {
                ifstream csvFile(entry.path());
                string header;
                getline(csvFile, header);
                stringstream headerStream(header);
                string columnName;
                while (getline(headerStream, columnName, ',')) {
                    availableColumns.pushTail(columnName);
                }
                csvFile.close();
                break;
            }
        }

        // проверяем условия WHERE
        Node1* rowNode = tableData.head; // проходим по всем строкам
        while (rowNode != nullptr) { // пока не закончились строки
            List row = List::fromString(rowNode->data); // преобразуем строку в список

            // проверяем условия WHERE
            if (checkConditions(row, availableColumns, query.conditions)) {
                // выводим выбранные колонки
                Node1* colIndexNode = tableColumnIndexes.head;
                while (colIndexNode != nullptr) {
                    int targetColumnIndex = stoi(colIndexNode->data);

                    Node1* rowSearchNode = row.head;
                    for (int i = 0; i < targetColumnIndex; ++i) {
                        rowSearchNode = rowSearchNode->next;
                    }

                    if (rowSearchNode != nullptr) {
                        cout << rowSearchNode->data << "\t";
                    }

                    colIndexNode = colIndexNode->next;
                }

                cout << endl;
            }
            rowNode = rowNode->next;
        }
    }
    // обработка запросов с двумя таблицами (декартово произведение)
    else if (tableNamesList.length() == 2) {
        string table1Name = tableNamesList.head->data;
        string table2Name = tableNamesList.head->next->data;

        const List& table1Data = tableDataList.get(table1Name);
        const List& table2Data = tableDataList.get(table2Name);
        const List& table1ColumnIndexes = tableSelectedColumnIndexes.get(table1Name);
        const List& table2ColumnIndexes = tableSelectedColumnIndexes.get(table2Name);

        Node1* row1Node = table1Data.head;
        while (row1Node != nullptr) {
            List row1 = List::fromString(row1Node->data);

            Node1* row2Node = table2Data.head;
            while (row2Node != nullptr) {
                List row2 = List::fromString(row2Node->data);

                // Выводим колонки из первой таблицы
                Node1* col1IndexNode = table1ColumnIndexes.head;
                while (col1IndexNode != nullptr) {
                    int targetColumnIndex = stoi(col1IndexNode->data);

                    Node1* rowSearchNode = row1.head;
                    for (int i = 0; i < targetColumnIndex; ++i) {
                        rowSearchNode = rowSearchNode->next;
                    }

                    if (rowSearchNode != nullptr) {
                        cout << rowSearchNode->data << "\t";
                    }

                    col1IndexNode = col1IndexNode->next;
                }

                // выводим колонки из второй таблицы
                Node1* col2IndexNode = table2ColumnIndexes.head;
                while (col2IndexNode != nullptr) {
                    int targetColumnIndex = stoi(col2IndexNode->data);

                    Node1* rowSearchNode = row2.head;
                    for (int i = 0; i < targetColumnIndex; ++i) {
                        rowSearchNode = rowSearchNode->next;
                    }

                    if (rowSearchNode != nullptr) {
                        cout << rowSearchNode->data << "\t";
                    }

                    col2IndexNode = col2IndexNode->next;
                }

                cout << endl;
                row2Node = row2Node->next;
            }
            row1Node = row1Node->next;
        }
    }
    else {
        cerr << "Выборка более чем из двух таблиц пока не поддерживается" << endl;
    }
}
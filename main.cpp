#include <iostream>
#include <fstream>
#include <string>
#include "support/json.hpp"
#include "database.h"
#include "support/header.h"
#include "parser.h"

using namespace std;
using json = nlohmann::json;

int main() {
    // чтение конфигурации из файла schema.json
    ifstream configFile("schema.json");
    json config;
    configFile >> config;

    Database::createDirectoryStructure(config);

    while (true) {
        cout << "Введите команду (или 'EXIT' для выхода): ";
        string command;
        getline(cin, command);

        if (command == "EXIT") {
            break;
        }

        else if (command.substr(0, 11) == "INSERT INTO") {
            List values = parseInsert(command);
            string tableName = values.head->data;
            values.popHead(); // удаляем имя таблицы из списка значений
            Database::insertTable(tableName, values);
        }
        else if (command.substr(0, 6) == "SELECT") {
            SelectQuery selectQuery = parseSelect(command);
            Database::selectTable(selectQuery);
        } 
        else {
            cout << "Неизвестная команда!" << endl;
        }
    }

    return 0;
}
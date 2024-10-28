#include <iostream>
#include "json.hpp"
#include "lockTable.h"
#include "PrimaryKey.h"
#include "parser.h"

using json = nlohmann::json;
using namespace std;

int main() {
/*
    string tableName = "table1";

    // блокировка таблицы
    if (lockTable::lockTablee(tableName)) {
        cout << "bloc" << endl;
    } else {
        cout << "ne bloc" << endl;
    }

    // проверка, заблокирована ли таблица
    if (lockTable::isTableLocked(tableName)) {
        cout << "bloc" << endl;
    } else {
        cout << "ne bloc" << endl;
    }

    // попытка заблокировать таблицу снова (должно не получиться)
    if (lockTable::lockTablee(tableName)) {
        cout << "bloc" << endl;
    } else {
        cout << "ne bloc" << endl;
    }

    // разблокировка таблицы
    if (lockTable::unlockTable(tableName)) {
        cout << "ne bloc" << endl;
    } else {
        cout << "bloc" << endl;
    }

    // проверка, заблокирована ли таблица
    if (lockTable::isTableLocked(tableName)) {
        cout << "bloc" << endl;
    } else {
        cout << "ne bloc" << endl;
    }

    int pk1 = PrimaryKey::getNextPrimaryKey(tableName);
    cout << tableName << ": " << pk1 << endl;
    int pk2 = PrimaryKey::getNextPrimaryKey(tableName);
    cout << tableName << ": " << pk2 << endl;
    int pk3 = PrimaryKey::getNextPrimaryKey(tableName);
    cout << tableName << ": " << pk3 << endl;
*/

    string selectQuery = "SELECT column1, column2 FROM table1 WHERE condition";
    Query selectParsed = Parser::parseQuery(selectQuery);
    cout << "SELECT Parsed: " << selectParsed.type << " " << selectParsed.table << " " << selectParsed.condition << endl;

    return 0;
}
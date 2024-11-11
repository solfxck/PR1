#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>
#include "support/header.h"
#include "support/json.hpp"
#include "PrimaryKey.h"
#include "lockTable.h"
#include "parser.h"

using namespace std;
using json = nlohmann::json;

class Database {
public:
    static void createDirectoryStructure(const json& config); // создание структуры директорий и файлов
    static void insertTable(const string& tableName, const List& values); // INSERT INTO
    static int getTuplesLimit() { return tuplesLimit; } // метод для доступа к tuplesLimit
    // static void selectFromTables(); // SELECT <> FROM <> (+WHERE и операторы OR , AND ???)
    // static void deleteTables() // DELETE FROM
private:
    static string schemaName; // имя схемы
    static int tuplesLimit; // лимит строк в файле
};
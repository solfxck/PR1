#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "support/header.h"

using namespace std;

enum class LogicalOperator {
    NONE,
    AND,
    OR
};

struct Condition {
    string column;
    string value;
    LogicalOperator nextOp = LogicalOperator::NONE;
};

struct SelectQuery {
    List columns;  // список колонок для выборки
    List tables;   // список таблиц
    List conditions; // список условий WHERE
};

SelectQuery parseSelect(const string& query);
List parseInsert(const string& query);

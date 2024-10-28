#include "parser.h"
#include <sstream>

// вспомогательная функция для разделения строки на токены
Array Parser::tokenize(const string& query) {
    Array tokens;
    string token;
    istringstream tokenStream(query);
    while (tokenStream >> token) {
        tokens.pushEnd(token);
    }
    return tokens;
}

// парсинг SQL-запроса
Query Parser::parseQuery(const string& query) {
    Array tokens = tokenize(query);
    if (tokens.length() == 0) {
        cout << "Пустой запрос" << endl;
    }

    string command = tokens.get(0);
    if (command == "SELECT") {
        return parseSelect(tokens);
    } 
    
    /*
    else if (command == "INSERT") {
        return parseInsert(tokens);
    } 
    
    else if (command == "DELETE") {
        return parseDelete(tokens);
    } 
    */

    else {
        cout << "Ошибка" << endl;
    }
}

Query Parser::parseSelect(const Array& tokens) {
    Query query;
    query.type = "SELECT";

    // Парсинг колонок
    int fromIndex = -1;
    for (int i = 1; i < tokens.length(); ++i) {
        if (tokens.get(i) == "FROM") {
            fromIndex = i;
            break;
        }
        query.columns.pushTail(tokens.get(i));
    }

    if (fromIndex == -1) {
        cout << "Ошибка синтаксиса: не найдено ключевое слово FROM" << endl;
    }

    // парсинг таблиц
    int whereIndex = -1;
    for (int i = fromIndex + 1; i < tokens.length(); ++i) {
        if (tokens.get(i) == "WHERE") {
            whereIndex = i;
            break;
        }
        query.table = tokens.get(i);
    }

    // парсинг условия (если есть)
    if (whereIndex != -1) {
        query.condition = tokens.get(whereIndex + 1);
    }

    return query;
}

/*
Query Parser::parseInsert(const Array& tokens) {
}

Query Parser::parseDelete(const Array& tokens) {
}
*/
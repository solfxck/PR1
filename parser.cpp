#include "parser.h"

using namespace std;

List parseInsert(const string& query) {
    List result;  // для хранения результата
    stringstream ss(query);  // поток для обработки строки запроса
    string token;  // переменная для хранения текущего токена

    // пропускаем
    ss >> token; // INSERT
    ss >> token; // INTO

    // получаем имя таблицы
    ss >> token;
    result.pushHead(token);  // добавляем имя таблицы в начало списка

    // пропускаем
    ss >> token; // VALUES

    // получаем значения
    getline(ss, token);  // считываем оставшуюся часть строки
    stringstream valuesStream(token);  // создаем поток для обработки значений
    while (getline(valuesStream, token, ',')) {  // разбиваем строку по запятым
        // убираем лишние пробелы и кавычки
        token.erase(remove_if(token.begin(), token.end(), [](char c) { return c == ' ' || c == '(' || c == ')' || c == '\''; }), token.end());
        result.pushTail(token);
    }

    return result;
}

SelectQuery parseSelect(const string& query) {
    SelectQuery result;
    stringstream ss(query);
    string token;

    // пропускаем SELECT
    ss >> token;

    // парсим колонки
    getline(ss, token, 'F');  // считываем до F в FROM
    stringstream columnsStream(token);
    while (getline(columnsStream, token, ',')) {
        // убираем лишние пробелы
        token.erase(0, token.find_first_not_of(" "));
        token.erase(token.find_last_not_of(" ") + 1);
        result.columns.pushTail(token);
    }

    // пропускаем FROM
    ss >> token;

    // парсим таблицы до WHERE или до конца строки
    string remainingQuery;
    getline(ss, remainingQuery);
    size_t wherePos = remainingQuery.find("WHERE");
    string tablesStr;
    
    if (wherePos != string::npos) {
        tablesStr = remainingQuery.substr(0, wherePos);
    } else {
        tablesStr = remainingQuery;
    }

    stringstream tablesStream(tablesStr);
    while (getline(tablesStream, token, ',')) {
        // убираем лишние пробелы
        token.erase(0, token.find_first_not_of(" "));
        token.erase(token.find_last_not_of(" ") + 1);
        result.tables.pushTail(token);
    }

    // если есть WHERE условия
    if (wherePos != string::npos) {
        string conditions = remainingQuery.substr(wherePos + 5); // +5 чтобы пропустить "WHERE"
        stringstream condStream(conditions);
        string condition;
        
        while (condStream >> token) {
            if (token != "AND" && token != "OR") {
                string column = token;
                string op, value;
                condStream >> op >> value;
                
                // убираем кавычки из значения
                value.erase(remove(value.begin(), value.end(), '\''), value.end());
                
                // создаем строку условия
                string condStr = column + "=" + value;
                
                // проверяем следующий оператор
                string nextOp;
                if (condStream >> nextOp) {
                    if (nextOp == "AND") {
                        condStr += "&";
                    } else if (nextOp == "OR") {
                        condStr += "|";
                    }
                }
                
                result.conditions.pushTail(condStr);
            }
        }
    }
    
    return result;
}
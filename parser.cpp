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

/*
parseInsert()
parseDelete()
*/
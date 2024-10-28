#include <string>
#include "header.h"

using namespace std;

struct Query {
    string type; // тип запроса (SELECT, INSERT, DELETE)
    string table; // имя таблицы
    List columns; // список колонок (для SELECT)
    string value; // значение (для INSERT)
    string condition; // условие (для DELETE)
};

class Parser {
public:
    // парсинг SQL-запроса
    static Query parseQuery(const string& query);
private:
    static Array tokenize(const string& query); // разделения строки на токены
    static Query parseSelect(const Array& tokens); // парсинг SELECT
    /*
    static Query parseInsert(const Array& tokens); // парсинг INSERT
    static Query parseDelete(const Array& tokens); // парсинг DELETE
    */
};
#include <iostream>
#pragma once

using namespace std;

// определение структуры Node1 (спиок)
struct Node1 {
    string data; // данные узла
    Node1* next; // указатель на следующий узел

    // конструктор для узла
    Node1(const string& value) : data(value), next(nullptr) {}
};

// объявление структуры Array
struct Array {
    string* data;
    int size;
    int capacity;

    Array();
    void pushEnd(string value);
    void push(int index, string value);
    string get(int index) const; // Добавляем const
    void pop(int index);
    void replace(int index, string value);
    int length() const; // Добавляем const
    void display();
};

// объявление структуры List
struct List {
    Node1* head;
    Node1* tail;
    int size;

    List();
    void pushHead(string value);
    void pushTail(string value);
    void popHead();
    void popTail();
    bool popValue(string value);
    bool search(string value);
    string toString(); // Сериализация списка в строку
    static List fromString(const string& str); // Десериализация строки в список
    int length(); // Получение длины списка
    void display();
};

// Структура узла для HashTable, хранящего List
struct NodeHT_List {
    std::string key;
    List value;
    NodeHT_List* next;

    // Конструктор с инициализацией
    NodeHT_List(const std::string& k, const List& val) : 
        key(k), value(val), next(nullptr) {}
};

// Структура HashTableList для работы с List
struct HashTableList {
    NodeHT_List** table;
    int size;
    int count;

    HashTableList(int initialSize = 100);
    ~HashTableList();
    int hash_function(const string& str);
    void insert(const string& key, const List& value);
    List get(const string& key);
    bool contains(const string& key);
    void remove(const string& key);
    void display();
};
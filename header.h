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

// структура NodeHT представляет узел в хеш-таблице
struct NodeHT {
    string key; // ключ, по которому хранится значение
    string value; // значение, связанное с ключом
    NodeHT* next; // указатель на следующий узел в связанном списке (для решения коллизий)

    // конструктор для создания нового узла хеш-таблицы
    // принимает ключ и значение
    NodeHT(const string& k, const string& v) : key(k), value(v), next(nullptr) {}
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
    void display();
};

// объявление структуры HashTable
struct HashTable {
    NodeHT** table; // массив указателей на узлы хеш-таблицы
    int size; // размер хеш-таблицы
    int count; // количество элементов в хеш-таблице

    HashTable(int initialSize = 10);
    int hash_function(const string& str);
    void insert(const string& key, const string& value);
    string get(const string& key);
    void remove(const string& key);
    void display();
};
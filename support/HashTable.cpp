#include "header.h"

// Реализация хеш-функции
int HashTableList::hash_function(const std::string& str) {
    int hash = 0;
    for (char c : str) {
        hash += c;
    }
    return hash % size;
}

// Конструктор
HashTableList::HashTableList(int initialSize) {
    size = initialSize;
    count = 0;
    table = new NodeHT_List*[size];
    for (int i = 0; i < size; i++) {
        table[i] = nullptr;
    }
}

// Деструктор
HashTableList::~HashTableList() {
    for (int i = 0; i < size; i++) {
        NodeHT_List* current = table[i];
        while (current != nullptr) {
            NodeHT_List* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}

// Вставка элемента
void HashTableList::insert(const std::string& key, const List& value) {
    int index = hash_function(key);
    NodeHT_List* current = table[index];

    // Обновление существующего элемента
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    // Создание нового элемента
    NodeHT_List* newNode = new NodeHT_List(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    count++;
}

// Получение значения по ключу
List HashTableList::get(const std::string& key) {
    int index = hash_function(key);
    NodeHT_List* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    return List(); // Возвращаем пустой List, если ключ не найден
}

// Проверка наличия ключа
bool HashTableList::contains(const std::string& key) {
    int index = hash_function(key);
    NodeHT_List* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }

    return false;
}

// Удаление элемента
void HashTableList::remove(const std::string& key) {
    int index = hash_function(key);
    NodeHT_List* current = table[index];
    NodeHT_List* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Вывод содержимого таблицы
void HashTableList::display() {
    for (int i = 0; i < size; i++) {
        NodeHT_List* current = table[i];
        while (current != nullptr) {
            std::cout << "Key: " << current->key << ", Value: ";
            current->value.display(); // Используем метод display() из List
            current = current->next;
        }
    }
}
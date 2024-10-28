#include "header.h"
#include <iostream>

using namespace std;

// инициализируем пустой список
List::List() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Сложность: O(1)
void List::pushHead(string value) {
    Node1* newNode = new Node1(value);  // создаем новый узел с переданным значением

    // если список пуст, новый элемент становится первым(head) и последним(tail)
    if (head == nullptr) {
        head = tail = newNode;
    }
    else {
        // если список не пуст, связываем новый узел с текущим первым элементом и обновляем указатель head
        newNode->next = head;
        head = newNode;
    }
    size++; // увеличиваем размер списка
}

// Сложность: O(1)
void List::pushTail(string value) {
    Node1* newNode = new Node1(value);  // создаем новый узел с переданным значением

    // если список пуст, новый элемент становится первым(head) и последним(tail)
    if (tail == nullptr) {
        head = tail = newNode;
    }
    else {
        // если список не пуст, связываем текущий последний элемент с новым узлом и обновляем tail
        tail->next = newNode;
        tail = newNode;
    }
    size++; // увеличиваем размер списка
}

// Сложность: O(1)
void List::popHead() {
    // если список пуст, выводим сообщение и выходим из функции
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }

    Node1* toDelete = head; // сохраняем указатель на элемент, который будет удален
    head = head->next; // перемещаем head на следующий элемент

    // если список опустел, tail нужно также установить в nullptr
    if (head == nullptr) tail = nullptr;
    delete toDelete; // освобождаем память удаленного элемента
    size--; // уменьшаем размер списка
}

// Сложность: O(n)
void List::popTail() {
    // если список пуст, выводим сообщение и выходим из функции
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }

    // если в списке один элемент, удаляем его и обновляем head и tail
    if (head == tail) {
        delete head;
        head = tail = nullptr;
    }
    else {
        // если в списке несколько элементов, находим предпоследний элемент
        Node1* current = head;
        while (current->next != tail) {
            current = current->next;
        }

        // удаляем последний элемент и обновляем tail
        delete tail;
        tail = current;
        tail->next = nullptr;
    }

    size--;  // уменьшаем размер списка
}

// Сложность: O(n)
bool List::popValue(string value) {
    // если список пуст, выводим сообщение и возвращаем false
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return false;
    }

    // если первый элемент имеет нужное значение, используем popHead()
    if (head->data == value) {
        popHead();
        return true;
    }

    Node1* current = head;
    // проходим по списку, пока не найдем элемент с требуемым значением
    while (current->next != nullptr && current->next->data != value) {
        current = current->next;
    }

    // если конец списка достигнут и элемент не найден, выводим сообщение и возвращаем false
    if (current->next == nullptr) {
        cout << "Элемент не найден!" << endl;
        return false;
    }

    Node1* toDelete = current->next;  // сохраняем указатель на элемент для удаления
    current->next = toDelete->next;  // изменяем указатель текущего элемента 

    // если удаляем последний элемент, обновляем tail
    if (toDelete == tail) tail = current;
    delete toDelete;  // освобождаем память удаленного элемента
    size--;  // уменьшаем размер списка
    return true;
}

// функция ищет элемент с заданным значением в списке
// Сложность: O(n)
bool List::search(string value) {
    Node1* current = head;
    // проходим по списку, пока не найдем искомый элемент
    while (current != nullptr) {
        if (current->data == value) return true;  // найден нужный элемент
        current = current->next;
    }
    return false;  // элемент не найден
}

// Сложность: O(n)
void List::display() {
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }

    Node1* current = head;
    // проходим по списку, выводя каждый элемент
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}
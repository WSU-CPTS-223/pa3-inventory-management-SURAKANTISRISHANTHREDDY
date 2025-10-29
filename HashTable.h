/************************************************************************
* Programmer:              SURAKANTI SRISHANTH REDDY                    *
* Class:                   CPTS 223                                     *
* Programming Assignment:  PA 3                                         *
* Date:                    OCTOBER 24, 2025                             *
*                                                                       *
* Description: 			   THIS PROJECT IS A C++ INVENTORY MANAGER THAT *
*                          LETS ONE SEARCH AND LIST AMAZON PRODUCTS     *
*                          USING CUSTOM BUILT DATA STRUCTURES INSTEAD   *
*                          OF STL. IT LOADS THOUSANDS OF ITEMS FROM A   *
*                          CSV FILE AND GIVES FAST, EASY COMMANDS LIKE  *
*                          `find` AND `listInventory` TO EXPLORE THE    *
*                          DATA PROVIDED.                               *
*                                                                       *
************************************************************************/
#pragma once
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <functional>
#include <vector>
#include <stdexcept>

template<typename K, typename V>
class HashTable {
private:
    struct Node {
        K key;
        V value;
        Node* next;

        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<Node*> table;
    size_t tableSize;
    size_t numElements;
    std::hash<K> hashFunc;

    size_t hash(const K& key) const {
        return hashFunc(key) % tableSize;
    }

    void rehash() {
        std::vector<Node*> oldTable = table;
        tableSize *= 2;
        table.clear();
        table.resize(tableSize, nullptr);
        numElements = 0;

        for (Node* head : oldTable) {
            Node* curr = head;
            while (curr) {
                insert(curr->key, curr->value);
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

public:
    HashTable(size_t size = 101) : tableSize(size), numElements(0) {
        table.resize(tableSize, nullptr);
    }

    ~HashTable() {
        clear();
    }

    void insert(const K& key, const V& value) {
        if (static_cast<double>(numElements) / tableSize > 0.75) {
            rehash();
        }

        size_t idx = hash(key);
        Node* curr = table[idx];

        // UPDATES IF THE KEY EXISTS
        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }

        // INSERTS NEW NODE IN THE BEGINING
        Node* newNode = new Node(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;
        numElements++;
    }

    bool find(const K& key, V& value) const {
        size_t idx = hash(key);
        Node* curr = table[idx];

        while (curr) {
            if (curr->key == key) {
                value = curr->value;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    bool contains(const K& key) const {
        size_t idx = hash(key);
        Node* curr = table[idx];

        while (curr) {
            if (curr->key == key) {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    bool remove(const K& key) {
        size_t idx = hash(key);
        Node* curr = table[idx];
        Node* prev = nullptr;

        while (curr) {
            if (curr->key == key) {
                if (prev) {
                    prev->next = curr->next;
                }
                else {
                    table[idx] = curr->next;
                }
                delete curr;
                numElements--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    void clear() {
        for (Node* head : table) {
            Node* curr = head;
            while (curr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        table.clear();
        table.resize(tableSize, nullptr);
        numElements = 0;
    }

    size_t size() const {
        return numElements;
    }

    bool empty() const {
        return numElements == 0;
    }

    // GETS ALL KEYS
    std::vector<K> getAllKeys() const {
        std::vector<K> keys;
        for (Node* head : table) {
            Node* curr = head;
            while (curr) {
                keys.push_back(curr->key);
                curr = curr->next;
            }
        }
        return keys;
    }
};

#endif // HASHTABLE_H
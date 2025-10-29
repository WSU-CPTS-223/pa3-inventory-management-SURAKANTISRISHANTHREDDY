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

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include "HashTable.h"
#include "Inventory.h"

// TEST FUNCTIONS 
void testHashTableBasic() {
    std::cout << "RUNNING HashTable BASIC TESTS..." << std::endl;

    HashTable<int, std::string> ht;

    // TESTING (INSERT, FIND, UPDATING EXISTING KEY, REMOVE, SIZE, CONTAINS)

    ht.insert(1, "apple");
    ht.insert(2, "banana");
    ht.insert(3, "cherry");

    std::string value;
    assert(ht.find(1, value) == true);
    assert(value == "apple");
    assert(ht.find(2, value) == true);
    assert(value == "banana");

    assert(ht.find(999, value) == false);

    ht.insert(1, "avocado");
    assert(ht.find(1, value) == true);
    assert(value == "avocado");

    assert(ht.remove(2) == true);
    assert(ht.find(2, value) == false);
    assert(ht.remove(999) == false);                     

    assert(ht.size() == 2);

    assert(ht.contains(1) == true);
    assert(ht.contains(999) == false);

    std::cout << "ALL HashTable BASICS PASSED !\n" << std::endl;
}
// TESTING (STRINGS, STRING KEYS)

void testHashTableString() {
    std::cout << "RUNNING HashTable STRING TESTS..." << std::endl;

    HashTable<std::string, int> ht;

    ht.insert("ONE", 1);
    ht.insert("TWO", 2);
    ht.insert("THREE", 3);

    int value;
    assert(ht.find("ONE", value) == true);
    assert(value == 1);

    ht.insert("", 0);
    assert(ht.find("", value) == true);
    assert(value == 0);

    ht.clear();
    assert(ht.empty() == true);
    assert(ht.size() == 0);

    std::cout << "ALL HashTable STRING TESTS PASSED !\n" << std::endl;
}

void testHashTableRehash() {
    std::cout << "RUNNING HashTable REHASH TESTS..." << std::endl;

    HashTable<int, int> ht(5); //SMALL INITIAL SIZE TO TRIGGER REHASH

    // INSERTING MANY ELEMENTS TO TRIGGER REHASH
    for (int i = 0; i < 20; i++) {
        ht.insert(i, i * 10);
    }

    // VERIFYING IF ALL ELEMENTS ARE PRESENT AFTER REHASHING
    int value;
    for (int i = 0; i < 20; i++) {
        assert(ht.find(i, value) == true);
        assert(value == i * 10);
    }

    assert(ht.size() == 20);

    std::cout << "ALL HashTable REHASHING TESTS PASSED !\n" << std::endl;
}

void testProductClass() {
    std::cout << "RUNNING PRODUCT CLASS TESTS..." << std::endl;

    // TESTING (MULTIPLE CATEGORIES, EMPTY CATEGORY)
    Product p1("12345", "Test Product", "Eaphones", "$19.99",
        "100", "10", "4.5", "Electronics|Computers|Accessories");

    assert(p1.getUniqId() == "12345");
    assert(p1.getProductName() == "Test Product");
    assert(p1.getCategories().size() == 3);
    assert(p1.getCategories()[0] == "Electronics");

    Product p2("67890", "Mouse", "Input device", "$29.99",
        "50", "5", "4.0", "");

    assert(p2.getCategories().size() == 1);
    assert(p2.getCategories()[0] == "NA");

    std::cout << "ALL PRODUCT CLASS TESTS PASSED !" << std::endl;
}

void runAllTests() {
    std::cout << "\n----*** RUNNING ALL THE TESTS ***----\n" << std::endl;
    testHashTableBasic();
    testHashTableString();
    testHashTableRehash();
    testProductClass();
    std::cout << "\n----*** ALL THE TESTS PASSED ***----\n" << std::endl;
}

void displayHelp() {
    std::cout << "\nALL AVAILABLE COMMANDS:" << std::endl;
    std::cout << "  find <UNIQUE I.D.>        - FINDS THE PRODUCT GIVEN THE UNIQUE I.D." << std::endl;
    std::cout << "  listInventory <CATEGORY>  - LISTS ALL THE PRODUCTS IN THE CATEGORY" << std::endl;
    std::cout << "  help                      - TAKES TO THE HELP PAGE" << std::endl;           //DISPLAYS THE SAME PAGE FOR NOW
    std::cout << "  exit                      - TO EXIT THE APPLICATION" << std::endl;
    std::cout << std::endl;
}
//SURAKANTI SRISHANTH REDDY

void processCommand(InventoryManager& manager, const std::string& command) {
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;

    if (cmd == "find") {
        std::string inventoryId;
        ss >> inventoryId;

        if (inventoryId.empty()) {
            std::cout << "USAGE: find <UNIQUE I.D.>" << std::endl;
            return;
        }

        Product* product = manager.findProduct(inventoryId);
        if (product) {
            std::cout << "\nPRODUCT FOUND :" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            product->print();
            std::cout << "----------------------------------------" << std::endl;
        }
        else {
            std::cout << "PRODUCT NOT FOUND" << std::endl;
        }
    }
    else if (cmd == "listInventory") {
        std::string category;
        std::getline(ss, category);

        // TRIM WHITE SPACES 
        size_t start = category.find_first_not_of(" \t");
        if (start != std::string::npos) {
            category = category.substr(start);
        }
        else {
            category = "";
        }

        if (category.empty()) {
            std::cout << "USAGE: listInventory <CATEGORY>" << std::endl;
            return;
        }

        if (!manager.categoryExists(category)) {
            std::cout << "INVALID " << std::endl;
            return;
        }

        std::vector<Product*> products = manager.listInventoryByCategory(category);

        std::cout << "\nPRODUCTS IN CATEGORY '" << category << "':" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        for (Product* p : products) {
            std::cout << "UNIQUE I.D.: " << p->getUniqId()
                << " | PRODUCT NAME: " << p->getProductName() << std::endl;
        }
        std::cout << "TOTAL: " << products.size() << " PRODUCTS" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
    else if (cmd == "help") {
        displayHelp();
    }
    else if (cmd == "exit") {
        std::cout << "EXITING..." << std::endl;
    }
    else if (cmd.empty()) {
      
    }
    else {
        std::cout << "UNKNOWN COMMAND: " << cmd << std::endl;
        std::cout << "TYPE 'help' FOR AVAILABLE COMMANDS" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "**********************************" << std::endl;
    std::cout << "AMAZON INVENTORY MANAGEMENT SYSTEM" << std::endl;
    std::cout << "**********************************" << std::endl;

    // RUNNING TESTS
    runAllTests();

    // LOAD INVENTORY
    InventoryManager manager;

    std::string filename = "Amazon Marketing Sample Jan 2020.csv";
    if (argc > 1) {
        filename = argv[1];
    }

    std::cout << "LOADING THE INVENTORY FROM: " << filename << std::endl;
    if (!manager.loadFromCSV(filename)) {
        std::cerr << "FAILED TO LOAD. EXITING." << std::endl;
        return 1;
    }

    std::cout << "\nINVENTORY LOADED SUCCESSFULLY!" << std::endl;
    displayHelp();

    // REPL LOOP
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        processCommand(manager, command);
    }

    std::cout << "THANK YOU FOR USING OUR SERVICES ! (IF IT DOESNT WORK THAT MEANS THERE WAS AN AWS OUTAGE !! )" << std::endl;
    return 0;
}


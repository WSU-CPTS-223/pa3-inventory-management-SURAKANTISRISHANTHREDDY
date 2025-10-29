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

#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "HashTable.h"

class Product {
private:
    std::string uniqId;
    std::string productName;
    std::string manufacturer;
    std::string price;
    std::string numberOfReviews;
    std::string numberOfAnsweredQuestions;
    std::string averageReviewRating;
    std::string amazonCategoryAndSubCategory;
    std::vector<std::string> categories;

public:
    Product() {}

    Product(const std::string& id, const std::string& name,
        const std::string& mfr, const std::string& pr,
        const std::string& reviews, const std::string& questions,
        const std::string& rating, const std::string& category)
        : uniqId(id), productName(name), manufacturer(mfr),
        price(pr), numberOfReviews(reviews),
        numberOfAnsweredQuestions(questions),
        averageReviewRating(rating),
        amazonCategoryAndSubCategory(category) {
        parseCategories(category);
    }

    void parseCategories(const std::string& categoryStr) {
        if (categoryStr.empty()) {
            categories.push_back("NA");
            return;
        }

        std::stringstream ss(categoryStr);
        std::string category;

        while (std::getline(ss, category, '|')) {
            // TRIM WHITE SPACES
            size_t start = category.find_first_not_of(" \t\r\n");
            size_t end = category.find_last_not_of(" \t\r\n");

            if (start != std::string::npos && end != std::string::npos) {
                categories.push_back(category.substr(start, end - start + 1));
            }
        }

        if (categories.empty()) {
            categories.push_back("NA");
        }
    }

    std::string getUniqId() const { return uniqId; }
    std::string getProductName() const { return productName; }
    std::string getManufacturer() const { return manufacturer; }
    std::string getPrice() const { return price; }
    std::string getNumberOfReviews() const { return numberOfReviews; }
    std::string getNumberOfAnsweredQuestions() const { return numberOfAnsweredQuestions; }
    std::string getAverageReviewRating() const { return averageReviewRating; }
    std::string getCategoryString() const { return amazonCategoryAndSubCategory; }
    const std::vector<std::string>& getCategories() const { return categories; }

    void print() const {
        std::cout << "UNIQUE I.D.: " << uniqId << std::endl;
        std::cout << "PRODUCT NAME: " << productName << std::endl;
        std::cout << "MANUFACTURER: " << (manufacturer.empty() ? "N/A" : manufacturer) << std::endl;
        std::cout << "PRICE: " << (price.empty() ? "N/A" : price) << std::endl;
        std::cout << "NUMBER OF REVIEWS: " << (numberOfReviews.empty() ? "N/A" : numberOfReviews) << std::endl;
        std::cout << "NUMBER OF ANSWERED QUESTIONS: " << (numberOfAnsweredQuestions.empty() ? "N/A" : numberOfAnsweredQuestions) << std::endl;
        std::cout << "AVERAGE REVIEW RATING: " << (averageReviewRating.empty() ? "N/A" : averageReviewRating) << std::endl;
        std::cout << "CATEGORIES: " << (amazonCategoryAndSubCategory.empty() ? "NA" : amazonCategoryAndSubCategory) << std::endl;
    }
};

class InventoryManager {
private:
    HashTable<std::string, Product*> productById;
    HashTable<std::string, std::vector<Product*>> productsByCategory;
    std::vector<Product*> allProducts;

    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\r\n\"");
        size_t end = str.find_last_not_of(" \t\r\n\"");

        if (start == std::string::npos || end == std::string::npos) {
            return "";
        }
        return str.substr(start, end - start + 1);
    }

    std::vector<std::string> parseCSVLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string field;
        bool inQuotes = false;

        for (size_t i = 0; i < line.length(); i++) {
            char c = line[i];

            if (c == '"') {
                inQuotes = !inQuotes;
            }
            else if (c == ',' && !inQuotes) {
                fields.push_back(trim(field));
                field.clear();
            }
            else {
                field += c;
            }
        }
        fields.push_back(trim(field));

        return fields;
    }

public:
    InventoryManager() {}

    ~InventoryManager() {
        for (Product* p : allProducts) {
            delete p;
        }
    }

    bool loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "ERROR CANNOT OPEN THE FILE " << filename << std::endl;
            return false;
        }

        std::string line;
        // SKIPS HEADER ROW
        std::getline(file, line);

        int lineNum = 1;
        while (std::getline(file, line)) {
            lineNum++;
            if (line.empty()) continue;

            std::vector<std::string> fields = parseCSVLine(line);

            // 8 FIELDS ATLEAST
            if (fields.size() < 8) {
                std::cerr << "WARNING: LINE " << lineNum << " HAS INSUFFIECIENT FIELDS ("
                    << fields.size() << "), SKIPPING" << std::endl;
                continue;
            }

            Product* product = new Product(
                fields[0],  // UNIQUE I.D.
                fields[1],  // PRODUCT NAME
                fields[2],  // MANUFACTURER
                fields[7],  // PRICE
                "",         // NUMBER OF REVIEWS
                "",         // NUMBER OF ANSWERED QUESTIONS
                "",         // AVERAGE REVIEW RATING
                fields[4]   // CATEGORIES
            );

            allProducts.push_back(product);

            // InNSERT INTO HASH TABLES
            productById.insert(product->getUniqId(), product);

            // INSERT INTO HASH TABLE FOR EACH CATEGOY
            for (const std::string& category : product->getCategories()) {
                std::vector<Product*> products;
                if (productsByCategory.find(category, products)) {
                    products.push_back(product);
                    productsByCategory.insert(category, products);
                }
                else {
                    std::vector<Product*> newList;
                    newList.push_back(product);
                    productsByCategory.insert(category, newList);
                }
            }
        }

        file.close();
        std::cout << "LOADED " << allProducts.size() << " PRODUCTS." << std::endl;
        return true;
    }

    Product* findProduct(const std::string& uniqId) {
        Product* product = nullptr;
        if (productById.find(uniqId, product)) {
            return product;
        }
        return nullptr;
    }

    std::vector<Product*> listInventoryByCategory(const std::string& category) {
        std::vector<Product*> products;
        if (productsByCategory.find(category, products)) {
            return products;
        }
        return std::vector<Product*>();
    }

    bool categoryExists(const std::string& category) {
        return productsByCategory.contains(category);
    }
};

#endif 












//4c69b61db1fc16e7013b43fc926e502d,"DB Longboards CoreFlex Crossbow 41"" Bamboo Fiberglass Longboard Complete",,,"Sports & Outdoors | Outdoor Recreation | Skates, Skateboards & Scooters | Skateboarding | Standard Skateboards & Longboards | Longboards","",,$237.68,,"","Make sure this fits by entering your model number. | RESPONSIVE FLEX: The Crossbow features a bamboo core encased in triaxial fiberglass and HD plastic for a responsive flex pattern that’s second to none. Pumping & carving have never been so satisfying! Flex 2 is recommended for people 120 to 170 pounds. | COREFLEX TECH: CoreFlex construction is water resistant, impact resistant, scratch resistant and has a flex like you won’t believe. These boards combine fiberglass, epoxy, HD plastic and bamboo to create a perfect blend of performance and strength. | INSPIRED BY THE NORTHWEST: Our founding ideal is chasing adventure & riding the best boards possible, inspired by the hills, waves, beaches & mountains all around our headquarters in the Northwest | BEST IN THE WORLD: DB was founded out of sheer love of longboarding with a mission to create the best custom longboards in the world, to do it sustainably, & to treat customers & employees like family | BEYOND COMPARE: Try our skateboards & accessories if you've tried similar products by Sector 9, Landyachtz, Arbor, Loaded, Globe, Orangatang, Hawgs, Powell-Peralta, Blood Orange, Caliber or Gullwing",Shipping Weight: 10.7 pounds (View shipping rates and policies)|ASIN: B07KMVJJK7|    #474    in Longboards Skateboard,"",10.7 pounds,"",https://images-na.ssl-images-amazon.com/images/I/51j3fPQTQkL.jpg|https://images-na.ssl-images-amazon.com/images/I/31hKM3cSoSL.jpg|https://images-na.ssl-images-amazon.com/images/I/51WlHdwghfL.jpg|https://images-na.ssl-images-amazon.com/images/I/51FsyLRBzwL.jpg|https://images-na.ssl-images-amazon.com/images/G/01/x-locale/common/transparent-pixel.jpg,https://www.amazon.com/DB-Longboards-CoreFlex-Fiberglass-Longboard/dp/B07KMVJJK7|https://www.amazon.com/DB-Longboards-CoreFlex-Fiberglass-Longboard/dp/B07KMN5KS7|https://www.amazon.com/DB-Longboards-CoreFlex-Fiberglass-Longboard/dp/B07KMXK857|https://www.amazon.com/DB-Longboards-CoreFlex-Fiberglass-Longboard/dp/B07KMW2VFR,,https://www.amazon.com/DB-Longboards-CoreFlex-Fiberglass-Longboard/dp/B07KMVJJK7,,,,,,,Y,,

//66d49bbed043f5be260fa9f7fbff5957, "Electronic Snap Circuits Mini Kits Classpack, FM Radio, Motion Detector, Music Box (Set of 5)", , , Toys& Games | Learning & Education | Science Kits & Toys, , , $99.95, , 55324, "Make sure this fits by entering your model number. | Snap circuits mini kits classpack provides basic electronic circuitry activities for students in grades 2-6 | Includes 5 separate mini building kits- an FM radio, a motion detector, music box, space battle sound effects, and a flying saucer | Each kit includes separate components and instructions to build | Each component represents one function in a circuit; components snap together to create working models of everyday electronic devices | Activity guide provides additional projects to teach students how circuitry works", Product Dimensions : 14.7 x 11.1 x 10.2 inches; 4.06 pounds | Shipping Weight : 4 pounds(View shipping rates and policies) | Domestic Shipping : Item can be shipped within U.S. | International Shipping : This item can be shipped to select countries outside of the U.S.Learn More | ASIN : B008AK6DAS | Item model number : 55324 | #3032    in Science Kits & Toys, "The snap circuits mini kits classpack provides basic electric circuitry information for students in grades 2-6. This classpack includes 5 snap-together building kits. Components snap together to create working models of everyday electronic devices. Kits included are an FM radio, a motion detector, a music box, space battle sound effects, and a flying saucer. Each mini kit comes with individual components, and an activity guide which includes instructions and additional project ideas. Each primary-colored component represents one function in a circuit. Activity kits are used by teachers and students in classroom and homeschool settings for educational and research applications in science, math, and for a variety of additional disciplines. Science education products and manipulatives incorporate applied math and science principles into classroom or homeschool projects. Teachers in pre-K, elementary, and secondary classrooms use science education kits, manipualtives, and products alongside science, technology, engineering, and math (STEM) curriculum to demonstrate STEM concepts and real-world applications through hands-on activities. Science education projects include a broad range of activities, such as practical experiments in engineering, aeronautics, robotics, chemistry, physics, biology, and geology.", 4 pounds, 14.7 x 11.1 x 10.2 inches  4.06 pounds, https://images-na.ssl-images-amazon.com/images/I/51M0KnJxjKL.jpg|https://images-na.ssl-images-amazon.com/images/I/5166GD8OkXL.jpg|https://images-na.ssl-images-amazon.com/images/I/61o5S1VnaNL.jpg|https://images-na.ssl-images-amazon.com/images/I/61t4Q0rPYjL.jpg|https://images-na.ssl-images-amazon.com/images/I/61NASUAyqcL.jpg|https://images-na.ssl-images-amazon.com/images/I/51OMrADdyJL.jpg|https://images-na.ssl-images-amazon.com/images/G/01/x-locale/common/transparent-pixel.jpg,"",,https://www.amazon.com/Electronic-Circuits-Classpack-Motion-Detector/dp/B008AK6DAS,,,,,,,Y,,

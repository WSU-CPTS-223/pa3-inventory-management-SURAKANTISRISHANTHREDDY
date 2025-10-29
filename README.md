# Amazon Inventory Management
A C++ inventory system for searching Amazon products using custom data structures.

## Features
-find <UNIQUE I.D.>        - FINDS THE PRODUCT GIVEN THE UNIQUE I.D. 
-listInventory <CATEGORY>  - LISTS ALL THE PRODUCTS IN THE CATEGORY 
-help                      - TAKES TO THE HELP PAGE 
-exit                      - TO EXIT THE APPLICATION 

## Commands
-find 
-listInventory 
-help
-exit

## Data Structures
- **HashTable** - Template based with separate chaining and automatic rehashing
- **Product** - Handles multiple categories and missing data
- **InventoryManager** - Manages product indexing and searches

## Testing
Comprehensive unit tests cover:
- HashTable operations (insert, find, remove, rehash)
- String handling and edge cases
- Product category parsing
- CSV data loading

## Implementation
- O(1) average case for both find and listInventory commands
- Robust CSV parsing with quote handling
- Memory efficient storage with proper cleanup

**CPTS 223**  
Surakanti Srishanth Reddy

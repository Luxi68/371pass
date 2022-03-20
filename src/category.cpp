// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 853761
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <algorithm>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include "item.h"
#include "category.h"

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
//
// Example:
//  Category c{"categoryIdent"};
Category::Category(const std::string ident) {
    this -> ident = ident;
    std::vector<Item> items;
    // std::cout << "Constructed Category... " << ident << std::endl;
}

Category::Category(const std::string ident, const std::size_t size) {
    this -> ident = ident;
    std::vector<Item> items;
    items.reserve(size);
    // std::cout << "Constructed Category with size... " << ident << std::endl;
}

Category::~Category() {
    // std::cout << "Destructed Category... " << ident << std::endl;
}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of Items in the Category contains.
//
// Example:
//  Category c{"categoryIdent"};
//  auto size = c.size();
unsigned int Category::size() {
    return this -> items.size();
}

// TODO Write a function, empty, that takes no parameters and returns true
//  if the number of Items in the Category is zero, false otherwise.
//
// Example:
//  Category c{"categoryIdent"};
//  auto empty = c.empty();
bool Category::empty() {
    if (this -> items.size() == 0) {
        return true;
    }
    return false;
}

// TODO Write a function, getIdent, that returns the identifier for the
//  Category.
//
// Example:
//  Category cObj{"categoryIdent"};
//  auto ident = cObj.getIdent();
std::string Category::getIdent() const {
    return this -> ident;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Category identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.setIdent("categoryIdent2");

// TODO Write a function, newItem, that takes one parameter, an Item identifier,
//  (a string) and returns the Item object as a reference. If an object with the
//  same identifier already exists, then the existing object should be returned.
//  Throw a std::runtime_error if the Item object cannot be inserted into the
//  container for whatever reason.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");

// Return the vector of items.
std::vector<Item> Category::getAllItems() const {
    return this -> items;
}

// TODO Write a function, addItem, that takes one parameter, an Item object,
//  and returns true if the object was successfully inserted. If an object with
//  the same identifier already exists, then the contents should be merged and
//  return false.
//
// Example:
//  Category cObj{"categoryIdent"};
//  Item iObj{"itemIdent"};
//  cObj.addItem(iObj);
bool Category::addItem(Item newItem) {
    for (auto itr = items.begin(); itr != items.end(); ++itr) {
        if(itr -> getIdent() == newItem.getIdent()) {
            if (*itr == newItem) {
                return false;
            }

            itr -> mergeItem(newItem);
            // std::cout << newItem.getIdent() << " new item merged with old" << std::endl;
            return false;
        }
    }

    items.push_back(newItem);
    // std::cout << newItem.getIdent() << " new item has been added to " << this -> getIdent() << std::endl;
    return true;
}

// TODO Write a function, getItem, that takes one parameter, an Item
//  identifier (a string) and returns the Item as a reference. If no Item
//  exists, throw an appropriate exception.
//
// Hint:
//  See the test scripts for the exception expected.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
//  auto iObj = cObj.getItem("itemIdent");
Item Category::getItem(std::string itemIdent) const {
    for (auto const& item : items) {
        if(item.getIdent() == itemIdent) {
            return item;
            // std::cout << itemIdent << " has been gotten from " << this -> getIdent() << std::endl;
        }
    }

    throw std::out_of_range("getItem failed, no item found for itemIdent: " + itemIdent);
}

// TODO Write a function, deleteItem, that takes one parameter, an Item
//  identifier (a string), deletes it from the container, and returns true if
//  the Item was deleted. If no Item exists, throw an appropriate exception.
//
// Example:
//  Category cObj{"categoryIdent"};
//  cObj.newItem("itemIdent");
//  bool result = cObj.deleteItem("itemIdent");
bool Category::deleteItem(std::string itemIdent) {
    for (auto itr = items.begin(); itr != items.end(); ++itr) {
        if(itr -> getIdent() == itemIdent) {
            itr = items.erase(itr);
            // std::cout << itemIdent << " has been erased from " << this -> ident << std::endl;
            return true;
        }
    }

    throw std::out_of_range("deleteItem failed, no item found for itemIdent: " + itemIdent);
}



// A function which takes a new category with the same ident value and
//  merges it with this (old) category. For any entries with the same key,
//  the new category takes priority. Returns the merged category object
//
// Example:
//  Item iObj1{"identIdent"};
//  iObj1.addEntry("key", "value");
//  Item iObj2{"identIdent"};
//  Item iObj3 = iObj2.merge(iObj1);
void Category::mergeCategory(const Category newCategory) {
    std::vector<Item> newCategoryItems = newCategory.getAllItems();
    for (auto const& item : newCategoryItems) {
        this -> addItem(item);
    }
}

// TODO Write an == operator overload for the Category class, such that two
//  Category objects are equal only if they have the same identifier and same
//  Items.
//
// Example:
//  Category cObj1{"categoryIdent1"};
//  cObj1.newItem("itemIdent");
//  Category cObj2{"categoryIdent2"};
//  if(cObj1 == cObj2) {
//    ...
//  }
bool operator==(const Category category1, const Category category2) {
    return (category1.ident == category2.ident) && (category1.items == category2.items);
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Category.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Category cObj{"categoryIdent"};
//  std::string s = cObj.str();
std::string Category::str() const {
    std::stringstream sstr;
    long unsigned int counter = 1;

    sstr << "{" ;
    for (auto itr = items.begin(); itr != items.end(); ++itr){
        sstr << "\"" << itr -> getIdent() << "\":" << itr -> str();
        
        if(counter != items.size()) {
            sstr << ",";
            counter++;
        }
    }
    sstr << "}";

    return sstr.str();
}

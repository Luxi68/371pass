// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 853761
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <string>
#include <map>
#include <sstream>
// #include <iostream>

#include "item.h"

// TODO Write a constructor that takes one parameter, a string identifier
//  and initialises the object and member data.
//
// Example:
//  Item iObj{"identIdent"};
Item::Item(const std::string ident) {
    this -> ident = ident;
    std::map<std::string, std::string> entries;
}

Item::~Item() {}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of entries in the Item contains.
//
// Example:
//  Item iObj{"identIdent"};
//  auto size = iObj.size();
unsigned int Item::size() {
    return this -> entries.size();
}

// TODO Write a function, empty, that takes no parameters and returns true
//  if the number of entries in the Item is zero, false otherwise.
//
// Example:
//  Item iObj{"identIdent"};
//  auto empty = iObj.empty();
bool Item::empty() {
    if (this -> entries.size() == 0) {
        return true;
    }
    return false;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Item identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.setIdent("identIdent2");

// TODO Write a function, getIdent, that returns the identifier for the Item.
//
// Example:
//  Item iObj{"identIdent"};
//  auto ident = iObj.getIdent();
std::string Item::getIdent() const {
    return this -> ident;
}

// TODO Write a function, setIdent, that takes one parameter, a string for a new
//  Item identifier, and updates the member variable. It returns nothing.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.setIdent("identIdent2");

// TODO Write a function, addEntry, that takes two parameters, an entry
//  key and value and returns true if the entry was inserted into the
//  container or false if the entry already existed and was replaced.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
bool Item::addEntry(std::string key, std::string value) {
    bool isFound = false;
    auto itr = this -> entries.find(key);

    if (itr != this -> entries.end()) {
        entries.erase(key);
        isFound = true;
    }

    this -> entries.insert (std::pair<std::string, std::string>(key, value));
    // std::cout << key << " has been added to " << this -> getIdent() << std::endl;
    return !isFound;
}

// TODO Write a function, getEntry, that takes one parameter, an entry
//  key and returns it's value. If no entry exists, throw an appropriate
//  exception.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
//  auto value = iObj.getEntry("key");
std::string Item::getEntry(std::string key) const {
    auto itr = this -> entries.find(key);

    if (itr != this -> entries.end()) {
        return itr -> second;
    } else {
        throw std::out_of_range("getEntry failed, no entry found for key: " + key);
    }
}

// TODO Write a function, deleteEntry, that takes one parameter, an entry
//  key, deletes it from the container, and returns true if the Item was
//  deleted. If no entry exists, throw an appropriate exception.
//
// Example:
//  Item iObj{"identIdent"};
//  iObj.addEntry("key", "value");
//  iObj.deleteEntry("key");
bool Item::deleteEntry(std::string key) {
    auto itr = this -> entries.find(key);

    if (itr != this -> entries.end()) {
        this -> entries.erase(itr);
        return true;
    } else {
        throw std::out_of_range("deleteEntry failed, no entry found for key: " + key);
    }
}

// A function which takes a item with the same ident value and
//  merges it with this (new) item. For any entries with the same key,
//  the new item takes priority. Returns the merged item object
//
// Example:
//  Item iObj1{"identIdent"};
//  iObj1.addEntry("key", "value");
//  Item iObj2{"identIdent"};
//  Item iObj3 = iObj2.merge(iObj1);
Item Item::mergeItem(Item oldItem) const {
    auto itr = this -> entries.begin();
    while (itr != this -> entries.end()) {
        oldItem.addEntry(itr -> first, itr -> second);
    }
    return oldItem;
}

// TODO Write an == operator overload for the Item class, such that two
//  Item objects are equal only if they have the same identifier and same
//  entries.
//
// Example:
//  Item iObj1{"identIdent"};
//  iObj1.addEntry("key", "value");
//  Item iObj2{"identIdent2"};
//  if(iObj1 == iObj2) {
//    ...
//  }
bool operator==(const Item item1, const Item item2) {
    return (item1.getIdent() == item2.getIdent()) && (item1.entries == item2.entries);
}

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Item.
//
// See the coursework specification for how this JSON should look.
//
// Example:
//  Item iObj{"itemIdent"};
//  std::string s = iObj.str();
std::string Item::str() const {
    std::stringstream sstr;
    long unsigned int counter = 1;
    
    sstr << "{" ;
    for (auto itr = entries.begin(); itr != entries.end(); ++itr){
        sstr << "\"" << itr -> first << "\""
             << ":\"" << itr -> second << "\"";
        
        if(counter != entries.size()) {
            sstr << ",";
            counter++;
        }
    }
    sstr << "}";

    return sstr.str();
}
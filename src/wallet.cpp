// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 853761
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>

#include "lib_json.hpp"
#include "wallet.h"
#include "category.h"
#include "item.h"

// TODO Write a Wallet constructor that takes no parameters and constructs an
//  empty wallet.
//
// Example:
//  Wallet wObj{};
Wallet::Wallet() {
    std::list<Category> categories;
    // std::cout << "Constructed Wallet..." << std::endl;
}

Wallet::~Wallet() {
    // std::cout << "Destructed Wallet..." << std::endl;
}

// TODO Write a function, size, that takes no parameters and returns an unsigned
//  int of the number of categories in the Wallet contains.
//
// Example:
//  Wallet wObj{};
//  auto size = wObj.size();
unsigned int Wallet::size() {
    return this -> categories.size();
}

// TODO Write a function, empty, that takes no parameters and returns true
//  if the number of categories in the Wallet is zero, false otherwise.
//
// Example:
//  Wallet wwObj{};
//  auto isEmpty = wObj.empty();
bool Wallet::empty() {
    if (this -> categories.size() == 0) {
        return true;
    }
    return false;
}

// TODO Write a function, newCategory, that takes one parameter, a category
//  identifier, and returns the Category object as a reference. If an object
//  with the same identifier already exists, then the existing object should be
//  returned. Throw a std::runtime_error if the Category object cannot be
//  inserted into the container.
//
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");
// Category &Wallet::newCategory(const std::string categoryIdent) {} 

// TODO Write a function, addCategory, that takes one parameter, a Category
//  object, and returns true if the object was successfully inserted. If an
//  object with the same identifier already exists, then the contents should be
//  merged and then return false. Throw a std::runtime_error if the Category
//  object cannot be inserted into the container for whatever reason.
//
// Example:
//  Wallet wObj{};
//  Category cObj{"categoryIdent"};
//  wObj.addCategory(cObj);
bool Wallet::addCategory(Category newCategory) {
    for(auto itr = categories.begin(); itr != categories.end(); ++itr) {
        if(itr -> getIdent() == newCategory.getIdent()) {
            if (*itr == newCategory) {
                return false;
            }

            itr -> mergeCategory(newCategory);
            // std::cout << newCategory.getIdent() << " new category merged with old" << std::endl;
            return false;
        }
    }

    try {
        categories.push_back(newCategory);
        // std::cout << newCategory.getIdent() << " new category has been added to wallet" << std::endl;
        return true;

    } catch (std::length_error const&) { 
        throw std::runtime_error("Could not add category: " + newCategory.getIdent());
    }
}

// TODO Write a function, getCategory, that takes one parameter, a Category
//  identifier and returns the Category. If no Category exists, throw an
//  appropriate exception.
//
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");
//  auto cObj = wObj.getCategory("categoryIdent");
Category Wallet::getCategory(std::string categoryIdent) const {
    for (auto const& category : categories) {
        if(category.getIdent() == categoryIdent) {
            return category;
            // std::cout << categoryIdent << " has been gotten" << std::endl;
        }
    }

    throw std::out_of_range(
        "getCategory failed, no category found for categoryIdent: " 
        + categoryIdent);
}

// TODO Write a function, deleteCategory, that takes one parameter, a Category
//  identifier, and deletes it from the container, and returns true if the
//  Category was deleted. If no Category exists, throw an appropriate exception.
//
// Example:
//  Wallet wObj{};
//  wObj.newCategory("categoryIdent");
//  wObj.deleteCategory("categoryIdent");
bool Wallet::deleteCategory(std::string categoryIdent) {
    for (auto const& category : categories) {
        if(category.getIdent() == categoryIdent) {
            categories.remove(category);
            // std::cout << categoryIdent << " has been erased from wallet" << std::endl;
            return true;
        }
    }

    throw std::out_of_range(
        "deleteCategory failed, no category found for categoryIdent: " 
        + categoryIdent);
}

// TODO Write a function, load, that takes one parameter, a std::string,
//  containing the filename for the database. Open the file, read the contents,
//  and populates the container for this Wallet. If the file does open throw an
//  appropriate exception (either std::runtime_error or a derived class).
//
// A note on clashes:
//  If you encounter two categories with the same key, the categories should be
//  merged (not replaced!). If you encounter two items with the same key in the
//  same category, the items should be merged (not replaced!). If you encounter
//  two entries with the same key in the same item, the entries should be merged
//  (undefined as to which value is picked). Two items in different categories
//  can have the same key, as can two entries in different items.
//
// JSON formatting:
//  The JSON file has the following format (see the sample database.json file
//  also provided with the coursework framework):
//    {
//      "Category 1" : {
//        "Item 1":  {
//          "detail 1 key": "detail 1 value",
//          "detail 2 key": "detail 2 value",
//          ...
//        },
//        "Item 2":  {
//          "detail 1 key": "detail 1 value",
//          "detail 2 key": "detail 2 value"
//        },
//        ...
//      },
//      "Category 2": {
//        "Item 1": {
//          "detail 1 key": "detail 1 value"
//        }
//        ...
//      }
//    }
//
// More help:
//  To help you with this function, I've selected the nlohmann::json
//  library that you must use for your coursework. Read up on how to use it
//  here: https://github.com/nlohmann/json. You may not use any other external
//  library other than the one I have provided. You may choose to process the
//  JSON yourself without the help of the library but I guarantee this will be
//  more work.
//
//  Understanding how to use external libraries is part of this coursework! You
//  will need to use this file to deserialize the JSON from string
//  to the JSON object provided by this library. Don't just look at the code
//  provided below, or in the README on the GitHub and despair. Google search,
//  look around, try code out in a separate file to all figure out how to use
//  this library.
//
//  Once you have deserialized the JSON string into an object, you will need to
//  loop through this object, constructing Category and Item objects according
//  to the JSON data in the file.
//
// Example:
//  Wallet wObj{};
//  wObj.load("database.json");
void Wallet::load(const std::string filename){
    std::ifstream ifs(filename);
    nlohmann::json jsonFile = nlohmann::json::parse(ifs);

    for(auto i = jsonFile.begin(); i != jsonFile.end(); ++i) {
        std::size_t itemSize = i.value().size();
        Category cObj{i.key(), itemSize};

        for(auto j = i.value().begin(); j != i.value().end(); ++j) {
            Item iObj{j.key()};

            for(auto k = j.value().begin(); k != j.value().end(); ++k) {
                iObj.addEntry(k.key(), k.value());
            }

            cObj.addItem(iObj);
        }
        
        this -> addCategory(cObj);
    }
}

// TODO Write a function ,save, that takes one parameter, the path of the file
//  to write the database to. The function should serialise the Wallet object
//  as JSON.
//
// Example:
//  Wallet wObj{};
//  wObj.load("database.json");
//  ...
//  wObj.save("database.json");
void Wallet::save(std::string filepath) const {
    nlohmann::json jsonFile;

    jsonFile = nlohmann::json::parse(this -> str());

    std::ofstream file(filepath);
    file << jsonFile;
}

// TODO Write an == operator overload for the Wallet class, such that two
//  Wallet objects are equal only if they have the exact same data.
//
// Example:
//  Wallet wObj1{};
//  Wallet wObj2{};
//  if(wObj1 == wObj2) {
//    ...
//  }

// TODO Write a function, str, that takes no parameters and returns a
//  std::string of the JSON representation of the data in the Wallet.
//
// Hint:
//  See the coursework specification for how this JSON should look.
//
// Example:
//  Wallet wObj{};
//  std::string s = wObj.str();
std::string Wallet::str() const {
    std::stringstream sstr;
    long unsigned int counter = 1;
    
    sstr << "{" ;
    for (auto itr = categories.begin(); itr != categories.end(); ++itr){
        sstr << "\"" << itr -> getIdent() << "\":" << itr -> str();
        
        if(counter != categories.size()) {
            sstr << ",";
            counter++;
        }
    }
    sstr << "}";

    return sstr.str();
}

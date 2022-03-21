// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 853761
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// The root object that holds all data for 371pass. This
// class contains Categories, which in turn hold Items,
// which hold a mapping of entries.
// -----------------------------------------------------

#ifndef WALLET_H
#define WALLET_H

#include <vector>
#include "category.h"

class Wallet {
    private:
        std::vector<Category> categories;

    public:
        Wallet();
        ~Wallet();

        unsigned int size() const;
        bool empty() const;

        Category &newCategory(const std::string categoryIdent);
        bool addCategory(const Category category);
        Category getCategory(std::string categoryIdent) const;
        bool deleteCategory(const std::string categoryIdent);

        void load(const std::string filename);
        std::string str() const;
        void save(const std::string filepath) const;
};

#endif // WALLET_H

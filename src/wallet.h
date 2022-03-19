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

#include <list>
#include "category.h"

class Wallet {
    private:
        std::list<Category> categories;

    public:
        Wallet();
        ~Wallet();

        unsigned int size();
        bool empty();

        bool addCategory(Category category);
        Category getCategory(std::string categoryIdent) const;
        bool deleteCategory(std::string categoryIdent);

        void load(const std::string filename);
        std::string str() const;
};

#endif // WALLET_H

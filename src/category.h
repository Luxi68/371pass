// -----------------------------------------------------
// CSC371 Advanced Object Oriented Programming (2021/22)
// Department of Computer Science, Swansea University
//
// Author: 853761
//
// Canvas: https://canvas.swansea.ac.uk/courses/24793
// -----------------------------------------------------
// A category contains one or more Items, each with
// their own identifier ('ident').
// -----------------------------------------------------

#ifndef CATEGORY_H
#define CATEGORY_H

#include <list>
#include "item.h"

class Category {
    private:
        std::string ident;
        std::list<Item> items;

    public:
        Category(const std::string ident);
        ~Category();

        unsigned int size();
        bool empty();

        std::string getIdent() const;

        bool addItem(Item item);
        Item getItem(std::string itemIdent) const;
        bool deleteItem(std::string itemIdent);

        Category mergeCategory(Category oldCategory) const;
        friend bool operator==(const Category category1, const Category category2);
        std::string str() const;
};

#endif // CATEGORY_H

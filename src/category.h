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

#include <vector>
#include "item.h"

class Category {
    private:
        std::string ident;
        std::vector<Item> items;

    public:
        Category(const std::string ident);
        Category(const std::string ident, const std::size_t size);
        ~Category();

        unsigned int size() const;
        bool empty() const;

        std::string getIdent() const;
        void setIdent(const std::string newIdent);
        std::vector<Item> getAllItems() const;

        bool addItem(const Item item);
        Item getItem(std::string itemIdent) const;
        bool deleteItem(const std::string itemIdent);

        void mergeCategory(const Category Category);
        friend bool operator==(const Category category1, const Category category2);
        std::string str() const;
};

#endif // CATEGORY_H

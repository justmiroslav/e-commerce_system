#pragma once
#include <map>
#include "Product.h"
#include "RandomId.h"

class ProductCatalog {
public:
    void addProduct(Product* product) {
        products_.push_back(product);
    }

    Product* getProductById(int productId) {
        for (const auto& product : products_) {
            if (product->getId() == productId) {
                return product;
            }
        }
        return nullptr;
    }

    void viewNameQuantityId() {
        cout << "Products:" << endl;
        for (const auto& product : products_) {
            cout << "Name: " << product->getName() << ", Quantity: " << product->getQuantity() << ". ID: " << product->getId() << "." << endl;
        }
    }

    void viewCategoryIdName() {
        for (const auto& entry : categoryIdName()) {
            const string& category = entry.first;
            const auto& idName = entry.second;
            cout << category << ": ProductIds: (";
            for (size_t i = 0; i < idName.size(); ++i) {
                cout << idName[i].first << "->" << idName[i].second;
                if (i < idName.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "), Fields: (";
            const auto& firstProduct = idName.front();
            if (dynamic_cast<Electronics*>(getProductById(firstProduct.first))) {
                cout << "brand/model/power";
            } else if (dynamic_cast<Books*>(getProductById(firstProduct.first))) {
                cout << "author/genre/ISBN";
            } else {
                cout << "size/color/material";
            }
            cout << ")" << endl;
        }
    }

    void viewBooks() {
        cout << "Books:" << endl;
        for (auto& product : products_) {
            if (auto* book = dynamic_cast<Books*>(product)) {
                cout << "Name: " << book->getName() << ", Quantity: " << book->getQuantity() << "." << endl;
            }
        }
    }

    void addNewProduct(const string& type, const string& name, double price, int quantity, const vector<string>& additionalInfo) {
        int id = generateRandomId();
        if (type == "Electronics") {
            addProduct(new Electronics(id, name, price, quantity, additionalInfo[0], additionalInfo[1], additionalInfo[2]));
        } else if (type == "Books") {
            addProduct(new Books(id, name, price, quantity, additionalInfo[0], additionalInfo[1], additionalInfo[2]));
        } else if (type == "Clothing") {
            addProduct(new Clothing(id, name, price, quantity, additionalInfo[0], additionalInfo[1], additionalInfo[2]));
        } else {
            cout << "Invalid product type" << endl;
        }
    }

    void removeProduct(int productId) {
        auto it = find_if(products_.begin(), products_.end(), [productId](Product* product) {
            return product->getId() == productId;
        });
        if (it != products_.end()) {
            delete *it;
            products_.erase(it, products_.end());
            cout << "Product with ID " << productId << " has been removed." << endl;
        } else {
            cout << "Product not found" << endl;
        }
    }

    void updateGeneralInfo(int productId, int quantity, double newPrice) {
        for (auto& product : products_) {
            if (product->getId() == productId) {
                if (quantity != 0) {
                    if (product->getQuantity() + quantity >= 0) {
                        product->updateQuantity(quantity);
                        cout << product->getName() << ": New quantity - " << product->getQuantity() << endl;
                    } else {
                        cout << "Invalid quantity. Max quantity slice available is " << 0 - product->getQuantity() << endl;
                    }
                }
                if (newPrice != 0.0) {
                    product->setPrice(newPrice);
                    cout << product->getName() << ": New price - " << product->getPrice() << "$" << endl;
                }
                return;
            }
        }
        cout << "Product not found" << endl;
    }

    void updateSpecificInfo(int productId, const string& fieldToUpdate, const string& newValue) {
        for (auto& product : products_) {
            if (product->getId() == productId) {
                bool invalid = product->updateField(fieldToUpdate, newValue);
                if (!invalid) {
                    cout << "Product successfully updated" << endl;
                }
                return;
            }
        }
        cout << "Product not found" << endl;
    }

    void reduceBooks(int quantity) {
        for (auto& product : products_) {
            if (auto* book = dynamic_cast<Books*>(product)) {
                if (book->getQuantity() - quantity < 0) {
                    cout << "Invalid quantity slice for " << book->getName() << ". Max quantity slice available is " << product->getQuantity() << endl;
                    return;
                }
            }
        }
        for (auto& product : products_) {
            if (auto* book = dynamic_cast<Books*>(product)) {
                book->updateQuantity(-quantity);
            }
        }
        cout << "Books quantity updated" << endl;
    }

    void viewInventory() {
        for (const auto& product : products_) {
            product->viewProduct();
            cout << "Total Cost: $" << product->calculateTotalCost() << endl;
            cout << "-------------" << endl;
        }
    }

    void viewCategory(const string& type) {
        bool found = false;
        for (const auto& product : products_) {
            if (product->getType() == type) {
                product->viewProduct();
                cout << "Total Cost: $" << product->calculateTotalCost() << endl;
                cout << "-------------" << endl;
                found = true;
            }
        }
        if (found) {
            return;
        }
        cout << "Invalid type" << endl;
    }

    void viewProduct(int productId) {
        for (const auto& product : products_) {
            if (product->getId() == productId) {
                product->viewProduct();
                cout << "Total Cost: $" << product->calculateTotalCost() << endl;
                cout << "-------------" << endl;
                return;
            }
        }
        cout << "Invalid name" << endl;
    }

    void checkQuantity() {
        bool allProductsNormal = true;
        for (const auto& product : products_) {
            if (product->getQuantity() < 5) {
                allProductsNormal = false;
                cout << product->getName() << " has low quantity. Current quantity: " << product->getQuantity() << endl;
            }
        }
        if (allProductsNormal) {
            cout << "All products have a sufficient quantity!" << endl;
        }
    }

    ~ProductCatalog() {
        for (const auto& product : products_) {
            delete product;
        }
    }

private:
    vector<Product*> products_;

    map<string, vector<pair<int, string>>> categoryIdName() {
        map<string, vector<pair<int, string>>> categoryProductIdsNames;
        for (const auto& product : products_) {
            const string category = product->getType();
            const int productId = product->getId();
            const string productName = product->getName();
            categoryProductIdsNames[category].emplace_back(productId, productName);
        }
        return categoryProductIdsNames;
    }
};

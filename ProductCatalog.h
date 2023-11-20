#ifndef E_COMMERCE_SYSTEM_PRODUCTCATALOG_H
#define E_COMMERCE_SYSTEM_PRODUCTCATALOG_H
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

    void viewProductsWithIds() {
        cout << "Product List with IDs: \n" << endl;
        for (const auto& product : products_) {
            cout << product->getName() << " = " << product->getId() << endl;
        }
        cout << "" << endl;
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

    void updateProduct(int productId, int quantity, double newPrice) {
        for (auto& product : products_) {
            if (product->getId() == productId) {
                if (quantity != 0) {
                    if (product->getQuantity() + quantity > 0) {
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

    void viewProducts() {
        for (const auto& product : products_) {
            product->viewProduct();
            cout << "Total Cost: $" << product->calculateTotalCost() << endl;
            cout << "-------------" << endl;
        }
    }

    void viewProductsByType(const string& type) {
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

    void checkLowQuantity() {
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
};

#endif //E_COMMERCE_SYSTEM_PRODUCTCATALOG_H

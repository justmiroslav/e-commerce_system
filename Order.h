#pragma once
#include <unordered_map>
#include "ProductCatalog.h"

class Order {
public:
    Order(ProductCatalog* productCatalog) : productCatalog_(productCatalog) {}

    int getOrderCount() const { return orders_.size(); }

    int createOrder() {
        int orderId = generateRandomId();
        orders_[orderId] = vector<Product*>();
        return orderId;
    }

    void viewOrderIds() {
        cout << "Orders: ";
        auto it = orders_.begin();
        while (it != orders_.end()) {
            cout << it->first;
            if (++it != orders_.end()) {
                cout << ", ";
            } else {
                cout << "." << endl;
            }
        }
    }

    bool isValidOrderId(int orderId) const {
        return orders_.find(orderId) != orders_.end();
    }

    void viewProductsWithIds(int orderId) {
        cout << "Order " << orderId << ":" << endl;
        size_t i = 0;
        for (const auto& product : orders_[orderId]) {
            cout << product->getQuantity() << " " << product->getName() << " = " << product->getId();
            if (++i < orders_[orderId].size()) {
                cout << "; ";
            } else {
                cout << "." << endl;
            }
        }
    }

    void addToOrder(int orderId, int productId, int quantity) {
        Product* product = productCatalog_->getProductById(productId);
        if (product) {
            auto& order = orders_[orderId];
            auto newProduct = find_if(order.begin(), order.end(), [productId](Product* p) {
                return p->getId() == productId;
            });
            if (quantity > 0 && product->getQuantity() - quantity >= 0) {
                product->updateQuantity(-quantity);
                if (newProduct != order.end()) {
                    (*newProduct)->updateQuantity(quantity);
                    cout << "New Quantity of product " << (*newProduct)->getName() << ": " << (*newProduct)->getQuantity() << endl;
                } else {
                    order.push_back(product->extractProducts(quantity));
                    cout << quantity << " " << product->getName() << " successfully added to Order " << orderId << "!" << endl;
                }
            } else {
                cout << "Invalid quantity. Max quantity slice available is " << product->getQuantity() << endl;
            }
        } else {
            cout << "Invalid ID" << endl;
        }
    }

    void removeFromOrder(int orderId, int productId, int quantity) {
        auto& order = orders_[orderId];
        auto selectedOrder = find_if(order.begin(), order.end(), [productId](Product* p) {
            return p->getId() == productId;
        });
        if (selectedOrder != order.end()) {
            if (quantity > 0 && quantity <= (*selectedOrder)->getQuantity()) {
                (*selectedOrder)->updateQuantity(-quantity);
                productCatalog_->getProductById(productId)->updateQuantity(quantity);
                if ((*selectedOrder)->getQuantity() == 0) {
                    cout << "Product " << (*selectedOrder)->getName() << " is removed from Order " << orderId << endl;
                    delete *selectedOrder;
                    order.erase(selectedOrder);
                } else {
                    cout << "New Quantity of product " << (*selectedOrder)->getName() << ": " << (*selectedOrder)->getQuantity() << endl;
                }
            } else {
                cout << "Invalid quantity. Max quantity slice available is " << (*selectedOrder)->getQuantity() << endl;
            }
        } else {
            cout << "Product not found in Order " << orderId << endl;
        }
    }

    void viewOrder(int orderId) {
        cout << "Order ID: " << orderId << endl;
        for (const auto &product: orders_[orderId]) {
            product->viewProduct();
            cout << "Total Cost: $" << product->calculateTotalCost() << endl;
            cout << "-------------" << endl;
        }
    }

    void calculateTotalOrderCost(int orderId) {
        double totalCost = 0.0;
        for (const auto& product : orders_[orderId]) {
            totalCost += product->calculateTotalCost();
        }
        if (totalCost != 0.0) {
            cout << "Total Cost for Order " << orderId << ": $" << totalCost << endl;
        } else {
            cout << "Order " << orderId << " is empty" << endl;
        }
    }

    void payOrder(int orderId) {
        auto it = orders_.find(orderId);
        cout << "Order " << orderId << " has been successfully paid." << endl;
        calculateTotalOrderCost(orderId);
        for (const auto& product : it->second) {
            delete product;
        }
        orders_.erase(it);
    }

    ~Order() {
        for (const auto& order : orders_) {
            for (const auto& product : order.second) {
                delete product;
            }
        }
    }

private:
    ProductCatalog* productCatalog_;
    unordered_map<int, vector<Product*>> orders_;
};

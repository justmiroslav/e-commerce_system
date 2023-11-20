#ifndef E_COMMERCE_SYSTEM_ORDER_H
#define E_COMMERCE_SYSTEM_ORDER_H
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

    void viewProductsWithIds(int orderId) {
        auto it = orders_.find(orderId);
        if (it == orders_.end()) {
            cout << "Order not found" << endl;
            return;
        }
        cout << "Order " << orderId << ":\n" << endl;
        for (const auto &product: orders_[orderId]) {
            cout << product->getName() << " = " << product->getId() << endl;
        }
        cout << "" << endl;
    }

    void addToOrder(int orderId, int productId, int quantity) {
        auto it = orders_.find(orderId);
        if (it == orders_.end()) {
            cout << "Order not found" << endl;
            return;
        }
        Product* product = productCatalog_->getProductById(productId);
        if (product) {
            auto& order = orders_[orderId];
            auto newProduct = find_if(order.begin(), order.end(), [productId](Product* p) {
                return p->getId() == productId;
            });

            if (newProduct != order.end()) {
                if (quantity > 0 && (*newProduct)->getQuantity() - quantity >= 0) {
                    (*newProduct)->updateQuantity(quantity);
                    product->updateQuantity(-quantity);
                    cout << "New Quantity of product " << (*newProduct)->getName() << ": " << (*newProduct)->getQuantity() << endl;
                } else {
                    cout << "Invalid quantity. Max quantity slice available is " << (*newProduct)->getQuantity() << endl;
                }
            } else {
                if (quantity > 0 && product->getQuantity() - quantity >= 0) {
                    product->updateQuantity(-quantity);
                    string type = product->getType();
                    if (type == "Electronics") {
                        auto pElectronics = dynamic_cast<Electronics*>(product);
                        order.push_back(new Electronics(product->getId(), product->getName(), product->getPrice(), quantity, pElectronics->getBrand(), pElectronics->getModel(), pElectronics->getPower()));
                    } else if (type == "Books") {
                        auto pBooks = dynamic_cast<Books*>(product);
                        order.push_back(new Books(product->getId(), product->getName(), product->getPrice(), quantity, pBooks->getAuthor(), pBooks->getGenre(), pBooks->getISBN()));
                    } else if (type == "Clothing") {
                        auto pClothing = dynamic_cast<Clothing*>(product);
                        order.push_back(new Clothing(product->getId(), product->getName(), product->getPrice(), quantity, pClothing->getSize(), pClothing->getColor(), pClothing->getMaterial()));
                    }
                    cout << quantity << " " << product->getName() << "'s successfully added to Order " << orderId << "!" << endl;
                } else {
                    cout << "Invalid quantity. Max quantity slice available is " << product->getQuantity() << endl;
                }
            }
        } else {
            cout << "Invalid ID" << endl;
        }
    }

    void removeFromOrder(int orderId, int productId, int quantity) {
        auto it = orders_.find(orderId);
        if (it == orders_.end()) {
            cout << "Order not found" << endl;
            return;
        }
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
        auto it = orders_.find(orderId);
        if (it == orders_.end()) {
            cout << "Order not found" << endl;
            return;
        }
        cout << "Order ID: " << orderId << endl;
        for (const auto &product: orders_[orderId]) {
            product->viewProduct();
            cout << "Total Cost: $" << product->calculateTotalCost() << endl;
            cout << "-------------" << endl;
        }
    }

    void calculateTotalOrderCost(int orderId) {
        auto it = orders_.find(orderId);
        if (it == orders_.end()) {
            cout << "Order not found" << endl;
            return;
        }
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
        if (it == orders_.end()) {
            cout << "Order not found" << endl;
            return;
        }
        cout << "Order " << orderId << " has been successfully paid." << endl;
        calculateTotalOrderCost(orderId);
        for (const auto& product : it->second) {
            delete product;
        }
        orders_.erase(it);
    }
private:
    ProductCatalog* productCatalog_;
    unordered_map<int, vector<Product*>> orders_;
};

#endif //E_COMMERCE_SYSTEM_ORDER_H

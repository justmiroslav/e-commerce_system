#pragma once
#include "Order.h"
#include "FileReader.h"

enum StartCommands {
    inventory = 1,
    order = 2,
    leave = 3
};

enum InventoryCommands {
    addProduct = 1,
    removeProduct = 2,
    updateGeneralInfo = 3,
    updateSpecificInfo = 4,
    minusBook = 5,
    viewInventory = 6,
    viewCategory = 7,
    viewProduct = 8,
    checkQuantity = 9
};

enum OrderCommands {
    createOrder = 1,
    addToOrder = 2,
    removeFromOrder = 3,
    viewOrder = 4,
    calculateTotalOrderCost = 5,
    payOrder = 6,
};

class CommandHandler {
public:
    CommandHandler(Order& orderManager, ProductCatalog& productCatalog)
            : orderManager(orderManager), productCatalog(productCatalog) {}

    void handleInventoryCommand(int inventoryCommand) {
        if (inventoryCommand == addProduct) {
            cout << "Enter product type (Electronics/Books/Clothing):" << endl;
            getline(cin, type);
            cout << "Enter product name:" << endl;
            getline(cin, name);
            cout << "Enter price:" << endl;
            cin >> newPrice;
            cin.ignore();
            cout << "Enter quantity:" << endl;
            cin >> quantity;
            cin.ignore();
            if (type == "Electronics" || type == "Books" || type == "Clothing") {
                for (int i = 0; i < 3; ++i) {
                    cout << "Enter additional info #" << (i + 1) << ":" << endl;
                    string info;
                    getline(cin, info);
                    additionalInfo.push_back(info);
                }
                productCatalog.addNewProduct(type, name, newPrice, quantity, additionalInfo);
                additionalInfo.clear();
                cout << "Product added successfully!" << endl;
            } else {
                cout << "Invalid product type" << endl;
            }
        } else if (inventoryCommand == removeProduct) {
            productCatalog.viewNameQuantityId();
            cout << "Enter product ID:" << endl;
            cin >> productId;
            cin.ignore();
            productCatalog.removeProduct(productId);
        } else if (inventoryCommand == updateGeneralInfo) {
            productCatalog.viewNameQuantityId();
            cout << "Enter product ID:" << endl;
            cin >> productId;
            cin.ignore();
            cout << "Enter quantity (- to keep current quantity):" << endl;
            getline(cin, quantityInput);
            quantity = (quantityInput != "-") ? stoi(quantityInput) : 0;
            cout << "Enter price (- to keep current price):" << endl;
            getline(cin, priceInput);
            newPrice = (priceInput != "-") ? stod(priceInput) : 0;
            productCatalog.updateGeneralInfo(productId, quantity, newPrice);
        } else if (inventoryCommand == updateSpecificInfo) {
            productCatalog.viewCategoryIdName();
            cout << "Enter product ID:" << endl;
            cin >> productId;
            cin.ignore();
            cout << "Enter field to update:" << endl;
            getline(cin, fieldToUpdate);
            cout << "Enter new value:" << endl;
            getline(cin, newValue);
            productCatalog.updateSpecificInfo(productId, fieldToUpdate, newValue);
        } else if (inventoryCommand == minusBook) {
            productCatalog.viewBooks();
            cout << "Enter quantity:" << endl;
            cin >> quantity;
            cin.ignore();
            productCatalog.reduceBooks(quantity);
        } else if (inventoryCommand == viewInventory) {
            productCatalog.viewInventory();
        } else if (inventoryCommand == viewCategory) {
            cout << "Enter product type (Electronics/Books/Clothing):" << endl;
            getline(cin, type);
            productCatalog.viewCategory(type);
        } else if (inventoryCommand == viewProduct) {
            productCatalog.viewNameQuantityId();
            cout << "Enter product ID:" << endl;
            cin >> productId;
            cin.ignore();
            productCatalog.viewProduct(productId);
        } else if (inventoryCommand == checkQuantity) {
            productCatalog.checkQuantity();
        } else {
            cout << "Enter a valid command!" << endl;
        }
    }

    void handleOrderCommand(int orderCommand) {
        if (orderCommand == createOrder) {
            orderId = orderManager.createOrder();
            cout << "Order " << orderId << " created successfully!" << endl;
        } else if (orderCommand == addToOrder || orderCommand == removeFromOrder || orderCommand == viewOrder || orderCommand == calculateTotalOrderCost || orderCommand == payOrder) {
            if (orderManager.getOrderCount() == 0) {
                cout << "No orders available. Please create an order first" << endl;
                return;
            }
            orderManager.viewOrderIds();
            cout << "Enter Order ID:" << endl;
            cin >> orderId;
            cin.ignore();
            if (!orderManager.isValidOrderId(orderId)) {
                cout << "Order not found" << endl;
                return;
            }
            if (orderCommand == addToOrder) {
                productCatalog.viewNameQuantityId();
                cout << "Enter product ID:" << endl;
                cin >> productId;
                cin.ignore();
                cout << "Enter quantity:" << endl;
                cin >> quantity;
                cin.ignore();
                orderManager.addToOrder(orderId, productId, quantity);
            } else if (orderCommand == removeFromOrder) {
                orderManager.viewProductsWithIds(orderId);
                cout << "Enter product ID:" << endl;
                cin >> productId;
                cin.ignore();
                cout << "Enter quantity:" << endl;
                cin >> quantity;
                cin.ignore();
                orderManager.removeFromOrder(orderId, productId, quantity);
            } else if (orderCommand == viewOrder) {
                orderManager.viewOrder(orderId);
            } else if (orderCommand == calculateTotalOrderCost) {
                orderManager.calculateTotalOrderCost(orderId);
            } else {
                orderManager.payOrder(orderId);
            }
        } else {
            cout << "Enter a valid command!" << endl;
        }
    }

private:
    Order& orderManager;
    ProductCatalog& productCatalog;
    int quantity{}, productId{}, orderId{};
    double newPrice{};
    string type, name, quantityInput, priceInput, fieldToUpdate, newValue;
    vector<string> additionalInfo;
};


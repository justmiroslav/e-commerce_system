#include "ProductCatalog.h"
#include "Order.h"
#include "FileReader.h"

enum StartCommands {
    inventory = 1,
    order = 2,
    leave = 3
};

enum InventoryCommands {
    addNewProduct = 1,
    removeProduct = 2,
    updateProduct = 3,
    viewProducts = 4,
    viewProductsByType = 5,
    checkLowQuantity = 6
};

enum OrderCommands {
    createOrder = 1,
    addToOrder = 2,
    removeFromOrder = 3,
    viewOrder = 4,
    calculateTotalOrderCost = 5,
    payOrder = 6,
};

int main() {
    ProductCatalog productCatalog;
    FileReader fileReader(R"(C:\Users\Admin\CLionProjects\e-commerce_system\products.txt)", &productCatalog);
    Order orderManager(&productCatalog);
    fileReader.readFile();
    int StartCommand, inventoryCommand, orderCommand, quantity, productId, orderId;
    double newPrice;
    string type, name, quantityInput, priceInput;
    vector<string> additionalInfo;
    while (true) {
        cout << "1->inventory/2->order/3-leave:" << endl;
        cin >> StartCommand;
        cin.ignore();
        if (StartCommand == inventory) {
            cout << "1->addNewProduct/2->removeProduct/3->updateProduct/4->viewProducts/5->viewProductsByType/6-checkLowQuantity:" << endl;
            cin >> inventoryCommand;
            cin.ignore();
            if (inventoryCommand == addNewProduct) {
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
                productCatalog.viewProductsWithIds();
                cout << "Enter product ID:" << endl;
                cin >> productId;
                cin.ignore();
                productCatalog.removeProduct(productId);
            } else if (inventoryCommand == updateProduct) {
                productCatalog.viewProductsWithIds();
                cout << "Enter product ID:" << endl;
                cin >> productId;
                cin.ignore();
                cout << "Enter quantity (- to keep current quantity):" << endl;
                getline(cin, quantityInput);
                quantity = (quantityInput != "-") ? stoi(quantityInput) : 0;
                cout << "Enter price (- to keep current price):" << endl;
                getline(cin, priceInput);
                newPrice = (priceInput != "-") ? stod(priceInput) : 0;
                productCatalog.updateProduct(productId, quantity, newPrice);
            } else if (inventoryCommand == viewProducts) {
                productCatalog.viewProducts();
            } else if (inventoryCommand == viewProductsByType) {
                cout << "Enter product type (Electronics/Books/Clothing):" << endl;
                getline(cin, type);
                productCatalog.viewProductsByType(type);
            } else if (inventoryCommand == checkLowQuantity) {
                productCatalog.checkLowQuantity();
            } else {
                cout << "Enter a valid command!" << endl;
            }
        } else if (StartCommand == order) {
            cout << "1->CreateOrder/2->addToOrder/3->removeFromOrder/4->viewOrder/5->calculateTotalOrderCost/6->payOrder:" << endl;
            cin >> orderCommand;
            cin.ignore();
            if (orderCommand == createOrder) {
                orderId = orderManager.createOrder();
                cout << "Order " << orderId << " created successfully!" << endl;
            } else if (orderCommand == addToOrder || orderCommand == removeFromOrder || orderCommand == viewOrder || orderCommand == calculateTotalOrderCost || orderCommand == payOrder) {
                if (orderManager.getOrderCount() == 0) {
                    cout << "No orders available. Please create an order first" << endl;
                    continue;
                }
                orderManager.viewOrderIds();
                cout << "Enter Order ID:" << endl;
                cin >> orderId;
                cin.ignore();
                if (orderCommand == addToOrder) {
                    productCatalog.viewProductsWithIds();
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
                } else if (orderCommand == payOrder) {
                    orderManager.payOrder(orderId);
                }
            } else {
                cout << "Enter a valid command!" << endl;
            }
        } else if (StartCommand == leave) {
            cout << "Program stopped!" << endl;
            break;
        } else {
            cout << "Enter a valid command!" << endl;
        }
    }
    return 0;
}
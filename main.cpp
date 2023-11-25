#include "CommandHandler.h"

int main() {
    ProductCatalog productCatalog;
    FileReader fileReader(R"(C:\Users\Admin\CLionProjects\e-commerce_system\products.txt)", &productCatalog);
    Order orderManager(&productCatalog);
    fileReader.readFile();
    CommandHandler commandHandler(orderManager, productCatalog);
    int StartCommand, inventoryCommand, orderCommand;
    while (true) {
        cout << "1->inventory/2->order/3-leave:" << endl;
        cin >> StartCommand;
        cin.ignore();
        if (StartCommand == inventory) {
            cout << "1->addProduct/2->removeProduct/3->updateGeneralInfo/4->updateSpecificInfo/5->minusBook/6->viewInventory/7->viewCategory/8->viewProduct/9-checkQuantity:" << endl;
            cin >> inventoryCommand;
            cin.ignore();
            commandHandler.handleInventoryCommand(inventoryCommand);
        } else if (StartCommand == order) {
            cout << "1->CreateOrder/2->addToOrder/3->removeFromOrder/4->viewOrder/5->calculateTotalOrderCost/6->payOrder:" << endl;
            cin >> orderCommand;
            cin.ignore();
            commandHandler.handleOrderCommand(orderCommand);
        } else if (StartCommand == leave) {
            cout << "Program stopped!" << endl;
            break;
        } else {
            cout << "Enter a valid command!" << endl;
        }
    }
    return 0;
}

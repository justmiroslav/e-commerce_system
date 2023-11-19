#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <sstream>

using namespace std;

class Product {
public:
    Product(int id, const string& name, double price, int quantity)
            : productId_(id), name_(name), price_(price), quantity_(quantity) {}

    int getId() const { return productId_; }

    string getName() const { return name_; }

    double getPrice() const { return price_; }

    int getQuantity() const { return quantity_; }

    double calculateTotalCost() const { return price_ * quantity_; }

    virtual void viewProduct() const = 0;

    virtual string getType() const = 0;

    void setPrice(double p) { price_ = p; }

    void appendProduct(int q) { quantity_ += q; }

    void removeProduct() {
        if (quantity_ > 0) {
            quantity_--;
        }
        else {
            cout << "Impossible to remove this product" << endl;
        }
    }
private:
    int productId_;
    string name_;
    double price_;
    int quantity_;
};

class Electronics : public Product {
public:
    Electronics(int id, const string& name, double price, int quantity, const string& brand, const string& model, const string& power)
            : Product(id, name, price, quantity), brand_(brand), model_(model), power_(power) {}

    void viewProduct() const override {
        cout << getName() << " " << brand_ << " " << model_ << "." << endl;
        cout << "price: " << getPrice() << ", quantity: " << getQuantity() << "." << endl;
    }

    string getType() const override {
        return "Electronics";
    }
private:
    string brand_;
    string model_;
    string power_;
};

class Books : public Product {
public:
    Books(int id, const string& name, double price, int quantity, const string& author, const string& genre, const string& isbn)
            : Product(id, name, price, quantity), author_(author), genre_(genre), ISBN_(isbn) {}

    void viewProduct() const override {
        cout << getName() << " by " << author_ << ", genre: " << genre_ << "." << endl;
        cout << "price: " << getPrice() << ", quantity: " << getQuantity() << "." << endl;
    }

    string getType() const override {
        return "Books";
    }
private:
    string author_;
    string genre_;
    string ISBN_;
};

class Clothing : public Product {
public:
    Clothing(int id, const string& name, double price, int quantity, const string& size, const string& color, const string& material)
            : Product(id, name, price, quantity), size_(size), color_(color), material_(material) {}

    void viewProduct() const override {
        cout << size_ << " " << color_ << " " << getName() << ", material: " << material_ << "." << endl;
        cout << "price: " << getPrice() << ", quantity: " << getQuantity() << "." << endl;
    }

    string getType() const override {
        return "Clothing";
    }
private:
    string size_;
    string color_;
    string material_;
};

class ProductCatalog {
public:
    void addProduct(Product* product) {
        products_.push_back(product);
    }

    void viewProductListWithIds() {
        cout << "Product List with IDs: \n" << endl;
        for (const auto& product : products_) {
            cout << product->getName() << " = " << product->getId() << endl;
        }
        cout << "" << endl;
    }

    void appendProduct(int productId, int quantity) {
        for (const auto& product : products_) {
            if (product->getId() == productId) {
                product->appendProduct(quantity);
                cout << "New " << product->getName() << " quantity: " << product->getQuantity() << endl;
                return;
            }
        }
        cout << "Product is not found" << endl;
    }

    void removeProduct(int productId) {
        for (const auto& product : products_) {
            if (product->getId() == productId) {
                product->removeProduct();
                cout << "Product " << product->getName() << " was removed" << endl;
                return;
            }
        }
        cout << "Product is not found" << endl;
    }

    void updateProduct(int productId, double newPrice) {
        for (const auto& product : products_) {
            if (product->getId() == productId) {
                product->setPrice(newPrice);
                cout << "New " << product->getName() << " price: $" << product->getPrice() << endl;
                return;
            }
        }
        cout << "Product is not found" << endl;
    }

    void viewProducts() {
        for (const auto& product : products_) {
            product->viewProduct();
            cout << "Total Cost: $" << product->calculateTotalCost() << endl;
            cout << "-------------" << endl;
        }
    }

    void viewProductsByType(const string& type) {
        int counter;
        for (const auto& product : products_) {
            if (product->getType() == type) {
                product->viewProduct();
                cout << "Total Cost: $" << product->calculateTotalCost() << endl;
                cout << "-------------" << endl;
                counter++;
            }
            if (counter == 3) {
                return;
            }
        }
        cout << "Invalid type" << endl;
    }

    ~ProductCatalog() {
        for (const auto& product : products_) {
            delete product;
        }
    }
private:
    vector<Product*> products_;
};

class FileReader {
public:
    FileReader(const string& filename, ProductCatalog* productCatalog) : filename_(filename), productCatalog_(productCatalog) {}

    static int generateRandomTicketId() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(10000, 99999);
        return dis(gen);
    }

    void readFile() {
        ifstream file(filename_);
        vector<Product*> products;
        string line;
        while (getline(file, line)) {
            vector<string> tokens;
            istringstream iss(line);
            string token;
            while (getline(iss, token, ','))
                tokens.push_back(token);

            int id = generateRandomTicketId();
            double price = stod(tokens[2]);
            int quantity = stoi(tokens[3]);

            if (tokens[0] == "Electronics") {
                productCatalog_->addProduct(new Electronics(id, tokens[1], price, quantity, tokens[4], tokens[5], tokens[6]));
            } else if (tokens[0] == "Books") {
                productCatalog_->addProduct(new Books(id, tokens[1], price, quantity, tokens[4], tokens[5], tokens[6]));
            } else if (tokens[0] == "Clothing") {
                productCatalog_->addProduct(new Clothing(id, tokens[1], price, quantity, tokens[4], tokens[5], tokens[6]));
            }
        }
        file.close();
    }
private:
    string filename_;
    ProductCatalog* productCatalog_;
};

enum StartCommands {
    inventory = 1,
    order = 2,
    leave = 3
};

enum InventoryCommands {
    appendProduct = 1,
    removeProduct = 2,
    updateProduct = 3,
    viewProducts = 4,
    viewProductsByType = 5
};

int main() {
    ProductCatalog productManager;
    FileReader fileReader(R"(C:\Users\Admin\CLionProjects\e-commerce_system\products.txt)", &productManager);
    fileReader.readFile();
    int command, inventoryCommand, quantity, productId;
    double newPrice;
    string type;
    while (true) {
        cout << "1->inventory/2->order/3-leave:" << endl;
        cin >> command;
        cin.ignore();
        if (command == inventory) {
            cout << "1->addProduct/2->removeProduct/3->updateProduct/4->viewProducts/5->viewProductsByType:" << endl;
            cin >> inventoryCommand;
            cin.ignore();
            if (inventoryCommand == appendProduct) {
                productManager.viewProductListWithIds();
                cout << "Enter the product ID:" << endl;
                cin >> productId;
                cin.ignore();
                cout << "Enter the quantity:" << endl;
                cin >> quantity;
                cin.ignore();
                productManager.appendProduct(productId, quantity);
            } else if (inventoryCommand == removeProduct) {
                productManager.viewProductListWithIds();
                cout << "Enter the product ID:" << endl;
                cin >> productId;
                cin.ignore();
                productManager.removeProduct(productId);
            } else if (inventoryCommand == updateProduct) {
                productManager.viewProductListWithIds();
                cout << "Enter the product ID:" << endl;
                cin >> productId;
                cin.ignore();
                cout << "Enter the price:" << endl;
                cin >> newPrice;
                cin.ignore();
                productManager.updateProduct(productId, newPrice);
            } else if (inventoryCommand == viewProducts) {
                productManager.viewProducts();
            } else if (inventoryCommand == viewProductsByType) {
                cout << "Enter the product type:" << endl;
                getline(cin, type);
                productManager.viewProductsByType(type);
            } else {
                cout << "Enter a valid command!" << endl;
            }
        } else if (command == order) {
            cout << "nothing to show" << endl;
        } else if (command == leave) {
            cout << "Program finished!" << endl;
            break;
        } else {
            cout << "Enter a valid command!" << endl;
        }
    }
    return 0;
}
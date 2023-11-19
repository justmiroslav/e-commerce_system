#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
vector<int> generatedIds;

int generateRandomId() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(10000, 99999);
    int id;
    do {
        id = dis(gen);
    } while (find(generatedIds.begin(), generatedIds.end(), id) != generatedIds.end());
    generatedIds.push_back(id);
    return id;
}

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

    void updateQuantity(int q) { quantity_ += q; }
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
        auto it = remove_if(products_.begin(), products_.end(), [productId](Product* product) {
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
        for (const auto& product : products_) {
            if (product->getId() == productId) {
                if ((product->getQuantity() + quantity) > 0) {
                    product->updateQuantity(quantity);
                    product->setPrice(newPrice);
                    cout << product->getName() << ": New price - " << product->getPrice() << "$, New quantity - " << product->getQuantity() << endl;
                } else {
                    cout << "Invalid quantity. Max quantity slice available is " << 0 - product->getQuantity() << endl;
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

class FileReader {
public:
    FileReader(const string& filename, ProductCatalog* productCatalog) : filename_(filename), productCatalog_(productCatalog) {}

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

            int id = generateRandomId();
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
    addNewProduct = 1,
    removeProduct = 2,
    updateProduct = 3,
    viewProducts = 4,
    viewProductsByType = 5,
    checkLowQuantity = 6
};

int main() {
    ProductCatalog productCatalog;
    FileReader fileReader(R"(C:\Users\Admin\CLionProjects\e-commerce_system\products.txt)", &productCatalog);
    fileReader.readFile();
    int command, inventoryCommand, quantity, productId;
    double newPrice;
    string type, name;
    vector<string> additionalInfo;
    while (true) {
        cout << "1->inventory/2->order/3-leave:" << endl;
        cin >> command;
        cin.ignore();
        if (command == inventory) {
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
                productCatalog.viewProductListWithIds();
                cout << "Enter product ID:" << endl;
                cin >> productId;
                cin.ignore();
                productCatalog.removeProduct(productId);
            } else if (inventoryCommand == updateProduct) {
                productCatalog.viewProductListWithIds();
                cout << "Enter product ID:" << endl;
                cin >> productId;
                cin.ignore();
                cout << "Enter quantity:" << endl;
                cin >> quantity;
                cin.ignore();
                cout << "Enter price:" << endl;
                cin >> newPrice;
                cin.ignore();
                productCatalog.updateProduct(productId, quantity, newPrice);
            } else if (inventoryCommand == viewProducts) {
                productCatalog.viewProducts();
            } else if (inventoryCommand == viewProductsByType) {
                cout << "Enter the product type:" << endl;
                getline(cin, type);
                productCatalog.viewProductsByType(type);
            } else if (inventoryCommand == checkLowQuantity) {
                productCatalog.checkLowQuantity();
            } else {
                cout << "Enter a valid command!" << endl;
            }
        } else if (command == order) {
            cout << "nothing to show" << endl;
        } else if (command == leave) {
            cout << "Program stopped!" << endl;
            break;
        } else {
            cout << "Enter a valid command!" << endl;
        }
    }
    return 0;
}
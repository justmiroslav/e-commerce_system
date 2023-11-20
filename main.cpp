#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>

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

    string getBrand() const { return brand_; }
    string getModel() const { return model_; }
    string getPower() const { return power_; }

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

    string getAuthor() const { return author_; }
    string getGenre() const { return genre_; }
    string getISBN() const { return ISBN_; }

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

    string getSize() const { return size_; }
    string getColor() const { return color_; }
    string getMaterial() const { return material_; }

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
        cout << "Order IDs: ";
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
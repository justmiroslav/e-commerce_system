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
            : id(id), name(name), price(price), quantity(quantity) {}

    int getId() { return id; }

    string getName() const { return name; }

    double getPrice() const { return price; }

    int getQuantity() const { return quantity; }

    double calculateTotalCost() { return price * quantity; }

    virtual void viewProduct() const = 0;

    void setPrice(double p) { price = p; }

    void addProduct() { quantity++; }

    void removeProduct() {
        if (quantity > 0) {
            quantity--;
        }
        else {
            cout << "Impossible to remove this product" << endl;
        }
    }
private:
    int id;
    string name;
    double price;
    int quantity;
};

class Electronics : public Product {
public:
    Electronics(int id, const string& name, double price, int quantity, const string& brand, const string& model, const string& power)
            : Product(id, name, price, quantity), brand(brand), model(model), powerConsumption(power) {}

    void viewProduct() const override {
        cout << "Electronics:" << endl;
        cout << getName() << " " << brand << " " << model << "." << endl;
        cout << "price: " << getPrice() << ", quantity: " << getQuantity() << "." << endl;
    }
private:
    string brand;
    string model;
    string powerConsumption;
};

class Books : public Product {
public:
    Books(int id, const string& name, double price, int quantity, const string& author, const string& genre, const string& isbn)
            : Product(id, name, price, quantity), author(author), genre(genre), ISBN(isbn) {}

    void viewProduct() const override {
        cout << "Books:" << endl;
        cout << getName() << " by " << author << ", genre: " << genre << "." << endl;
        cout << "price: " << getPrice() << ", quantity: " << getQuantity() << "." << endl;
    }
private:
    string author;
    string genre;
    string ISBN;
};

class Clothing : public Product {
public:
    Clothing(int id, const string& name, double price, int quantity, const string& size, const string& color, const string& material)
            : Product(id, name, price, quantity), size(size), color(color), material(material) {}

    void viewProduct() const override {
        cout << "Clothing:" << endl;
        cout << size << " " << color << " " << getName() << ", material: " << material << "." << endl;
        cout << "price: " << getPrice() << ", quantity: " << getQuantity() << "." << endl;
    }

private:
    string size;
    string color;
    string material;
};

class FileReader {
public:

    static int generateRandomTicketId() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(10000, 99999);
        return dis(gen);
    }

    vector<Product*> readFile(const string& filename) {
        ifstream file(filename);
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
                products.push_back(new Electronics(id, tokens[1], price, quantity, tokens[4], tokens[5], tokens[6]));
            } else if (tokens[0] == "Books") {
                products.push_back(new Books(id, tokens[1], price, quantity, tokens[4], tokens[5], tokens[6]));
            } else if (tokens[0] == "Clothing") {
                products.push_back(new Clothing(id, tokens[1], price, quantity, tokens[4], tokens[5], tokens[6]));
            }
        }
        file.close();
        return products;
    }
};

int main() {
    FileReader fileReader;
    vector<Product*> products = fileReader.readFile(R"(C:\Users\Admin\CLionProjects\e-commerce_system\products.txt)");
    for (const auto& product : products) {
        product->viewProduct();
        cout << "Total Cost: $" << product->calculateTotalCost() << endl;
        cout << "------------------------" << endl;
    }
    for (const auto& product : products) {
        delete product;
    }
    return 0;
}
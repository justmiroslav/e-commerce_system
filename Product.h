#ifndef E_COMMERCE_SYSTEM_PRODUCT_H
#define E_COMMERCE_SYSTEM_PRODUCT_H
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
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

#endif //E_COMMERCE_SYSTEM_PRODUCT_H

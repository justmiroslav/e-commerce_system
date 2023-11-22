#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Product {
public:
    Product(int id, const string& name, double price, int quantity);

    int getId() const;
    string getName() const;
    double getPrice() const;
    int getQuantity() const;
    double calculateTotalCost() const;
    virtual void viewProduct() const;
    virtual Product* extractProducts(int quantity) const = 0;
    virtual string getType() const = 0;
    void setPrice(double p);
    void updateQuantity(int q);

private:
    int productId_;
    string name_;
    double price_;
    int quantity_;
};

class Electronics : public Product {
public:
    Electronics(int id, const string& name, double price, int quantity, const string& brand, const string& model, const string& power);

    string getBrand() const;
    string getModel() const;
    string getPower() const;
    void viewProduct() const override;
    Electronics* extractProducts(int quantity) const override;
    string getType() const override;
private:
    string brand_;
    string model_;
    string power_;
};

class Books : public Product {
public:
    Books(int id, const string& name, double price, int quantity, const string& author, const string& genre, const string& isbn);

    string getAuthor() const;
    string getGenre() const;
    string getISBN() const;
    void viewProduct() const override;
    Books* extractProducts(int quantity) const override;
    string getType() const override;
private:
    string author_;
    string genre_;
    string ISBN_;
};

class Clothing : public Product {
public:
    Clothing(int id, const string& name, double price, int quantity, const string& size, const string& color, const string& material);

    string getSize() const;
    string getColor() const;
    string getMaterial() const;
    void viewProduct() const override;
    Clothing* extractProducts(int quantity) const override;
    string getType() const override;
private:
    string size_;
    string color_;
    string material_;
};

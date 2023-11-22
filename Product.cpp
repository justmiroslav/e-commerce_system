#include "Product.h"

Product::Product(int id, const string& name, double price, int quantity)
        : productId_(id), name_(name), price_(price), quantity_(quantity) {}

int Product::getId() const {
    return productId_;
}

string Product::getName() const {
    return name_;
}

double Product::getPrice() const {
    return price_;
}

int Product::getQuantity() const {
    return quantity_;
}

double Product::calculateTotalCost() const {
    return price_ * quantity_;
}

void Product::viewProduct() const {
    cout << "price: " << getPrice() << ", quantity: " << getQuantity() << "." << endl;
}

void Product::setPrice(double p) {
    price_ = p;
}

void Product::updateQuantity(int q) {
    quantity_ += q;
}

Electronics::Electronics(int id, const string& name, double price, int quantity, const string& brand, const string& model, const string& power)
        : Product(id, name, price, quantity), brand_(brand), model_(model), power_(power) {}

string Electronics::getBrand() const {
    return brand_;
}

string Electronics::getModel() const {
    return model_;
}

string Electronics::getPower() const {
    return power_;
}

void Electronics::viewProduct() const {
    cout << getName() << " " << brand_ << " " << model_ << "." << endl;
    Product::viewProduct();
}

Electronics* Electronics::extractProducts(int quantity) const {
    return new Electronics(getId(), getName(), getPrice(), quantity, getBrand(), getModel(), getPower());
}

string Electronics::getType() const {
    return "Electronics";
}

Books::Books(int id, const string& name, double price, int quantity, const string& author, const string& genre, const string& isbn)
        : Product(id, name, price, quantity), author_(author), genre_(genre), ISBN_(isbn) {}

string Books::getAuthor() const {
    return author_;
}

string Books::getGenre() const {
    return genre_;
}

string Books::getISBN() const {
    return ISBN_;
}

void Books::viewProduct() const {
    cout << getName() << " by " << author_ << ", genre: " << genre_ << "." << endl;
    Product::viewProduct();
}

Books* Books::extractProducts(int quantity) const {
    return new Books(getId(), getName(), getPrice(), quantity, getAuthor(), getGenre(), getISBN());
}

string Books::getType() const {
    return "Books";
}

Clothing::Clothing(int id, const string& name, double price, int quantity, const string& size, const string& color, const string& material)
        : Product(id, name, price, quantity), size_(size), color_(color), material_(material) {}

string Clothing::getSize() const {
    return size_;
}

string Clothing::getColor() const {
    return color_;
}

string Clothing::getMaterial() const {
    return material_;
}

void Clothing::viewProduct() const {
    cout << size_ << " " << color_ << " " << getName() << ", material: " << material_ << "." << endl;
    Product::viewProduct();
}

Clothing* Clothing::extractProducts(int quantity) const {
    return new Clothing(getId(), getName(), getPrice(), quantity, getSize(), getColor(), getMaterial());
}

string Clothing::getType() const {
    return "Clothing";
}

#ifndef E_COMMERCE_SYSTEM_FILEREADER_H
#define E_COMMERCE_SYSTEM_FILEREADER_H
#include <sstream>
#include <fstream>
#include "ProductCatalog.h"

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

#endif //E_COMMERCE_SYSTEM_FILEREADER_H

#include <iostream>
#include <string>
#include <iomanip>
#include <list>
#include <vector>

using namespace std;

struct Product {
    int id;
    string name;
    double price;
    int quantity;

    Product(int id, const string& name, double price, int quantity)
        : id(id), name(name), price(price), quantity(quantity) {}
};

class CatalogInventorySystem {
private:
    static const int DEFAULT_BUCKETS = 10;
    int tableSize;

    // Doubly linked list holding all active products in the catalog
    list<Product> catalog;

    // Hash table using chaining: each bucket stores iterators pointing to nodes in catalog
    vector<list<list<Product>::iterator>> hashTable;

    int hashFunction(int productId) const {
        int h = productId % tableSize;
        return (h < 0) ? h + tableSize : h;
    }

    // Helper to find iterator in catalog by Product ID using the hash table
    list<Product>::iterator findProductIterator(int productId) {
        int bucketIndex = hashFunction(productId);
        for (auto it : hashTable[bucketIndex]) {
            if (it->id == productId) {
                return it;
            }
        }
        return catalog.end();
    }

public:
    explicit CatalogInventorySystem(int size = DEFAULT_BUCKETS)
        : tableSize(size > 0 ? size : DEFAULT_BUCKETS), hashTable(tableSize) {}

    // 1. Add a new product
    bool addProduct(int id, const string& name, double price, int quantity) {
        if (findProductIterator(id) != catalog.end()) {
            cout << "[Error] Product with ID " << id << " already exists.\n";
            return false;
        }
        if (price < 0 || quantity < 0) {
            cout << "[Error] Price and quantity cannot be negative.\n";
            return false;
        }

        catalog.emplace_back(id, name, price, quantity);
        auto newNodeIt = prev(catalog.end());

        int bucketIndex = hashFunction(id);
        hashTable[bucketIndex].push_back(newNodeIt);

        cout << "[Success] Added product ID " << id << " (" << name << ").\n";
        return true;
    }

    // 2. Update existing product details
    bool updateProduct(int id, const string& newName, double newPrice, int newQuantity) {
        auto it = findProductIterator(id);
        if (it == catalog.end()) {
            cout << "[Error] Product with ID " << id << " not found.\n";
            return false;
        }
        if (newPrice < 0 || newQuantity < 0) {
            cout << "[Error] Price and quantity cannot be negative.\n";
            return false;
        }

        it->name = newName;
        it->price = newPrice;
        it->quantity = newQuantity;

        cout << "[Success] Updated product ID " << id << ".\n";
        return true;
    }

    // 3. Remove a discontinued product
    bool removeProduct(int id) {
        auto it = findProductIterator(id);
        if (it == catalog.end()) {
            cout << "[Error] Product with ID " << id << " not found.\n";
            return false;
        }

        int bucketIndex = hashFunction(id);
        auto& bucket = hashTable[bucketIndex];
        for (auto bIt = bucket.begin(); bIt != bucket.end(); ++bIt) {
            if (*bIt == it) {
                bucket.erase(bIt);
                break;
            }
        }

        catalog.erase(it);
        cout << "[Success] Removed discontinued product ID " << id << ".\n";
        return true;
    }

    // 4. Display all available products
    void displayAllProducts() const {
        if (catalog.empty()) {
            cout << "\n[Catalog is empty]\n";
            return;
        }

        cout << "\n==================== PRODUCT CATALOG ====================\n";
        cout << left << setw(10) << "ID"
             << left << setw(24) << "Name"
             << right << setw(10) << "Price ($)"
             << right << setw(12) << "Quantity" << "\n";
        cout << "---------------------------------------------------------\n";

        for (const auto& p : catalog) {
            cout << left << setw(10) << p.id
                 << left << setw(24) << p.name
                 << right << setw(10) << fixed << setprecision(2) << p.price
                 << right << setw(12) << p.quantity << "\n";
        }
        cout << "=========================================================\n";
    }
};

int main() {
    CatalogInventorySystem system(7);

    system.addProduct(101, "Wireless Mouse", 24.99, 50);
    system.addProduct(108, "Mechanical Keyboard", 89.50, 30);
    system.addProduct(115, "USB-C Hub", 39.00, 45);
    system.addProduct(102, "Gaming Headset", 59.99, 20); // Collides with 109 if mod 7

    system.displayAllProducts();

    system.updateProduct(108, "Mechanical Keyboard RGB", 99.99, 25);
    system.removeProduct(115);

    system.displayAllProducts();

    return 0;
}

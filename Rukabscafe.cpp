/*
    Project: Rukaab Cafe Management System
    Developer: Muhammad Taha
    Language: C++
    Description: A console-based application to manage cafe orders, 
                 billing, taxes, donations, and reporting.
*/

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <windows.h> // Windows specific headers
#include <limits>
#include <algorithm>

using namespace std;

// ===================== Utility Functions =====================

// Robust Input Validation Function
int getValidInt() {
    int x;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\tInvalid input. Please enter a number: ";
    }
    return x;
}

void clearScreen() {
    system("cls");
}

void printHeader(const string& title) {
    cout << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // Cyan
    cout << "\t" << string(80, '=') << "\n";
    cout << "\t" << string((80 - title.length()) / 2, ' ') << title << "\n";
    cout << "\t" << string(80, '=') << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset
}

string centerText(const string& text, int width) {
    if (text.length() >= width) return text;
    int padding = (width - text.length()) / 2;
    return string(padding, ' ') + text + string(width - text.length() - padding, ' ');
}

void printMenuOption(int num, const string& text) {
    cout << "\t[" << num << "] " << text << "\n";
}

void printError(const string& message) {
    cout << "\n\t";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Red
    cout << "X " << message;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White
    Sleep(1500);
}

void printSuccess(const string& message) {
    cout << "\n\t";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Green
    cout << "V " << message;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White
    Sleep(1500);
}

// ===================== Data Structures =====================
struct OrderItem {
    string type;
    string name;
    int quantity;
    double price;
    vector<string> details;
};

struct Order {
    int orderId;
    string customerName;
    time_t orderTime;
    vector<OrderItem> items;
    double subtotal;
    double tax;
    double donation;
    double total;
    string paymentMethod;
    bool isPaid;
};

// ===================== Database Class =====================
class Database {
private:
    vector<Order> orders;
    int nextOrderId;

public:
    Database() {
        // Initialize nextOrderId based on existing file or default
        nextOrderId = 1001;
    }

    void saveOrder(const Order& order) {
        orders.push_back(order);
        
        ofstream file("orders.txt", ios::app);
        if (file.is_open()) {
            file << "Order ID: " << order.orderId << "\n";
            file << "Customer: " << order.customerName << "\n";
            char* dt = ctime(&order.orderTime);
            file << "Time: " << (dt ? dt : "N/A");
            file << "Items:\n";
            for (const auto& item : order.items) {
                file << "  - " << item.name << " x" << item.quantity 
                     << " @ Rs." << item.price << "\n";
            }
            file << "Subtotal: Rs." << order.subtotal << "\n";
            file << "Tax: Rs." << order.tax << "\n";
            file << "Donation: Rs." << order.donation << "\n";
            file << "Total: Rs." << order.total << "\n";
            file << "Payment: " << order.paymentMethod << "\n";
            file << "Status: " << (order.isPaid ? "Paid" : "Pending") << "\n";
            file << string(40, '-') << "\n";
            file.close();
        }
    }
    
    vector<Order> getOrders() const { return orders; }
    
    int getNextOrderId() { return nextOrderId++; }
    
    double getDailyRevenue() {
        double revenue = 0;
        time_t now = time(0);
        tm* today = localtime(&now);
        
        for (const auto& order : orders) {
            tm* orderTime = localtime(&order.orderTime);
            if (orderTime->tm_year == today->tm_year &&
                orderTime->tm_mon == today->tm_mon &&
                orderTime->tm_mday == today->tm_mday) {
                revenue += order.total;
            }
        }
        return revenue;
    }
};

// ===================== Rukaab Cafe Class =====================
class RukaabCafe {
private:
    Database db;
    string shopName = "Rukaab Cafe & Ice Cream";
    string shopAddress = "123 Food Street, Karachi";
    string shopPhone = "021-12345678";
    
    struct MenuItem {
        string category;
        string name;
        string description;
        double price;
        vector<string> options;
    };
    
    vector<MenuItem> menu = {
        {"Ice Cream", "Chocolate Dream", "Rich chocolate ice cream", 100, {"Single Scoop", "Double Scoop", "Triple Scoop"}},
        {"Ice Cream", "Vanilla Bliss", "Classic vanilla ice cream", 100, {"Single Scoop", "Double Scoop", "Triple Scoop"}},
        {"Ice Cream", "Strawberry Fields", "Fresh strawberry ice cream", 120, {"Single Scoop", "Double Scoop", "Triple Scoop"}},
        {"Ice Cream", "Mango Tango", "Tropical mango delight", 120, {"Single Scoop", "Double Scoop", "Triple Scoop"}},
        {"Shakes", "Chocolate Shake", "Rich chocolate milkshake", 250, {"Small", "Medium", "Large"}},
        {"Shakes", "Strawberry Shake", "Creamy strawberry milkshake", 250, {"Small", "Medium", "Large"}},
        {"Desserts", "Rukaab Brownie", "Special brownie with nuts", 300, {"Plain", "With Ice Cream"}},
        {"Beverages", "Coffee", "Hot/Cold coffee", 150, {"Hot", "Cold"}},
        {"Beverages", "Karak Chai", "Special strong tea", 100, {"Regular", "Masala"}}
    };
    
    vector<string> toppings = {
        "Chocolate Syrup (Rs.20)", "Strawberry Syrup (Rs.20)", "Caramel (Rs.25)",
        "Oreo Crumbles (Rs.30)", "Almonds (Rs.30)", "Whipped Cream (Rs.25)", "No Topping"
    };

    string getCurrentTime() {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }
    
public:
    void showWelcome() {
    clearScreen();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // Cyan Color
    
    // Developer Name ASCII Art
    cout << "\n\n";
    cout << "\t __  __       _                                       _   _____     _           \n";
    cout << "\t|  \\/  |     | |                                     | | |_   _|   | |          \n";
    cout << "\t| \\  / |_   _| |__   __ _ _ __ ___  _ __ ___   __ _  | |   | | __ _| |__   __ _ \n";
    cout << "\t| |\\/| | | | | '_ \\ / _` | '_ ` _ \\| '_ ` _ \\ / _` | | |   | |/ _` | '_ \\ / _` |\n";
    cout << "\t| |  | | |_| | | | | (_| | | | | | | | | | | | (_| | | |   | | (_| | | | | (_| |\n";
    cout << "\t|_|  |_|\\__,_|_| |_|\\__,_|_| |_| |_|_| |_| |_|\\__,_| |_|   |_|\\__,_|_| |_|\\__,_|\n";
    
    cout << "\n\t" << string(80, '-') << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Yellow
    cout << "\t" << centerText("RUKAAB CAFE MANAGEMENT SYSTEM", 80) << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // White

    cout << "\n\tLoading System ";
    for(int i = 0; i < 4; i++) {
        cout << ".";
        Sleep(400);
    }
    clearScreen();
}
    
    void showMainMenu() {
        clearScreen();
        printHeader("MAIN MENU");
        
        cout << "\n";
        printMenuOption(1, "Place New Order");
        printMenuOption(2, "View Menu");
        printMenuOption(3, "View Orders & Reports");
        printMenuOption(4, "Customer Feedback");
        printMenuOption(5, "Exit");
        
        cout << "\n\t" << string(78, '-') << "\n";
        cout << "\tSelect an option: ";
    }
    
    void showMenu() {
        clearScreen();
        printHeader("OUR MENU");
        
        string currentCategory = "";
        for (const auto& item : menu) {
            if (item.category != currentCategory) {
                currentCategory = item.category;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                cout << "\n\t--- " << currentCategory << " ---\n";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            
            cout << "\t" << left << setw(30) << item.name 
                 << "Rs." << setw(8) << right << fixed << setprecision(0) << item.price << "\n";
        }
        cout << "\n\n\tPress Enter to continue...";
        cin.ignore(); cin.get();
    }
    
    Order takeOrder() {
        clearScreen();
        printHeader("PLACE NEW ORDER");
        
        Order order;
        order.orderId = db.getNextOrderId();
        order.orderTime = time(0);
        order.isPaid = false;
        
        cin.ignore(); // Clear buffer from previous input
        cout << "\n\tEnter customer name: ";
        getline(cin, order.customerName);
        
        char addMore;
        do {
            showMenuForOrder();
            cout << "\n\tSelect item (1-" << menu.size() << "): ";
            int choice = getValidInt();
            
            if (choice < 1 || choice > (int)menu.size()) {
                printError("Invalid selection!");
                continue;
            }
            
            MenuItem selected = menu[choice - 1];
            OrderItem item;
            item.type = selected.category;
            item.name = selected.name;
            item.price = selected.price;
            
            cout << "\tEnter quantity: ";
            item.quantity = getValidInt();
            
            // Handle Ice Cream Options
            if (selected.category == "Ice Cream" && !selected.options.empty()) {
                cout << "\n\tSelect size:\n";
                for (size_t i = 0; i < selected.options.size(); i++) {
                    cout << "\t  " << (i + 1) << ". " << selected.options[i] << "\n";
                }
                cout << "\tChoice: ";
                int sizeChoice = getValidInt();
                if(sizeChoice > 0 && sizeChoice <= (int)selected.options.size())
                    item.details.push_back(selected.options[sizeChoice - 1]);
                
                // Toppings
                cout << "\n\tSelect topping (1-" << toppings.size() << "): ";
                for (size_t i = 0; i < toppings.size(); i++) cout << "\n\t  " << i+1 << ". " << toppings[i];
                cout << "\n\tChoice: ";
                int topChoice = getValidInt();
                if(topChoice > 0 && topChoice <= (int)toppings.size())
                    item.details.push_back(toppings[topChoice - 1]);
            }
            
            order.items.push_back(item);
            cout << "\n\tAdd another item? (y/n): ";
            cin >> addMore;
            
        } while (addMore == 'y' || addMore == 'Y');
        
        calculateOrderTotal(order);
        askDonation(order);
        selectPaymentMethod(order);
        return order;
    }
    
    void showMenuForOrder() {
        clearScreen();
        printHeader("SELECT ITEMS");
        for (size_t i = 0; i < menu.size(); i++) {
            cout << "\t" << setw(3) << right << (i + 1) << ". " 
                 << left << setw(25) << menu[i].name 
                 << "Rs." << menu[i].price << "\n";
        }
    }
    
    void calculateOrderTotal(Order& order) {
        order.subtotal = 0;
        for (const auto& item : order.items) {
            order.subtotal += item.price * item.quantity;
        }
        order.tax = order.subtotal * 0.13; // 13% tax
        order.donation = 0;
        order.total = order.subtotal + order.tax;
    }
    
    void askDonation(Order& order) {
        clearScreen();
        printHeader("CHARITY");
        cout << "\n\tWould you like to donate Rs. 100 to Palestine Relief? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            order.donation = 100;
            order.total += order.donation;
            printSuccess("Thank you for your generosity!");
        }
    }
    
    void selectPaymentMethod(Order& order) {
        clearScreen();
        printHeader("PAYMENT");
        cout << "\n\t1. Cash\n\t2. Card\n\t3. Online\n\tChoice: ";
        int choice = getValidInt();
        if (choice == 1) order.paymentMethod = "Cash";
        else if (choice == 2) order.paymentMethod = "Card";
        else order.paymentMethod = "Online";
        order.isPaid = true;
    }
    
    void generateReceipt(const Order& order) {
        clearScreen();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "\n\t" << centerText("RUKAAB CAFE - RECEIPT", 60) << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "\t" << string(60, '-') << "\n";
        cout << "\tOrder #: " << order.orderId << "\n";
        cout << "\tCustomer: " << order.customerName << "\n";
        cout << "\t" << string(60, '-') << "\n";
        
        for (const auto& item : order.items) {
            cout << "\t" << left << setw(30) << item.name << " x" << item.quantity 
                 << right << setw(10) << (item.price * item.quantity) << "\n";
            for(const auto& d : item.details) cout << "\t   + " << d << "\n";
        }
        
        cout << "\t" << string(60, '-') << "\n";
        cout << "\tSubtotal:   Rs." << order.subtotal << "\n";
        cout << "\tTax (13%):  Rs." << order.tax << "\n";
        if(order.donation > 0) cout << "\tDonation:   Rs." << order.donation << "\n";
        cout << "\tTOTAL:      Rs." << order.total << "\n";
        cout << "\t" << string(60, '-') << "\n";
        cout << "\n\tPress Enter to continue...";
        cin.ignore(); cin.get();
    }
    
    void showReports() {
        clearScreen();
        printHeader("REPORTS");
        cout << "\tTotal Daily Revenue: Rs." << db.getDailyRevenue() << "\n";
        cout << "\tTotal Orders stored in RAM: " << db.getOrders().size() << "\n";
        cout << "\n\tPress Enter to continue...";
        cin.ignore(); cin.get();
    }
    
    void handleFeedback() {
        clearScreen();
        printHeader("FEEDBACK");
        cout << "\n\t1. Give Feedback\n\t2. View Feedback\n\tChoice: ";
        int choice = getValidInt();
        
        if (choice == 1) {
            cin.ignore();
            string name, msg;
            cout << "\tName: "; getline(cin, name);
            cout << "\tFeedback: "; getline(cin, msg);
            ofstream file("feedback.txt", ios::app);
            file << "[" << getCurrentTime() << "] " << name << ": " << msg << "\n";
            printSuccess("Feedback Saved!");
        } else if (choice == 2) {
            ifstream file("feedback.txt");
            string line;
            cout << "\n";
            if(file.is_open()) {
                while(getline(file, line)) cout << "\t" << line << "\n";
                file.close();
            } else {
                printError("No feedback found.");
            }
            cout << "\n\tPress Enter to continue...";
            cin.ignore(); cin.get();
        }
    }

    void run() {
        showWelcome();
        while (true) {
            showMainMenu();
            int choice = getValidInt();
            
            switch (choice) {
                case 1: {
                    Order order = takeOrder();
                    db.saveOrder(order);
                    generateReceipt(order);
                    break;
                }
                case 2: showMenu(); break;
                case 3: showReports(); break;
                case 4: handleFeedback(); break;
                case 5: 
                    cout << "\n\tGoodbye! Dev: Muhammad Taha\n"; 
                    return;
                default: printError("Invalid option.");
            }
        }
    }
};

int main() {
    system("title Rukaab Cafe System - Dev: Muhammad Taha");
    system("color 0B"); 
    
    // Hide Cursor
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
    
    RukaabCafe cafe;
    cafe.run();
    return 0;
}
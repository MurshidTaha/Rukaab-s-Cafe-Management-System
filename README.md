# ☕ Rukaab Cafe Management System

A robust C++ console-based application designed to manage day-to-day operations of a cafe. This system handles order taking, menu management, automated billing with tax calculation, donation processing, and file handling for data persistence.

**Developer:** Muhammad Taha  
**Language:** C++

## ✨ Features

* **Interactive Menu:** Visual and easy-to-navigate console menu.
* **Order Management:** Process orders with multiple items, quantities, and customizable options (e.g., Ice cream scoops, toppings).
* **Automated Billing:** Automatically calculates subtotal, 13% tax, and generates a detailed receipt.
* **Charity Integration:** Option to add donations (Palestine Relief) directly to the bill.
* **Data Persistence:** Saves orders and customer feedback to `.txt` files automatically.
* **Admin Reports:** View daily revenue and order summaries.

## 🚀 How to Run

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/YourUsername/RukaabCafe-System.git](https://github.com/YourUsername/RukaabCafe-System.git)
    ```
2.  **Compile the code:**
    You can use any C++ compiler (GCC, MinGW, or Visual Studio).
    ```bash
    g++ main.cpp -o cafe_system
    ```
3.  **Run the application:**
    ```bash
    ./cafe_system
    ```
    *(Note: This application is optimized for Windows Console)*

## 📂 Project Structure

* `main.cpp`: Contains the core logic, classes (Database, Order, RukaabCafe), and utility functions.
* `orders.txt`: Auto-generated file storing order history.
* `feedback.txt`: Auto-generated file storing customer reviews.

## 📸 Usage

* **Navigating:** Use Number keys (1-5) to select menu options.
* **Input:** Follow on-screen prompts for names and quantities.

## 📜 License

This project is open-source and available for educational purposes.

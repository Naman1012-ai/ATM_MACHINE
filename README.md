# Modular ATM System in C

A robust, terminal-based Automated Teller Machine (ATM) simulation built in C. This project features a modular architecture, secure PIN masking, and a file-based database system to persist user data and transaction history.

## 🚀 Features

* **Automatic Account Generation:** New users are assigned a unique, permanent 6-digit account number.
* **Secure Authentication:** PIN masking during entry and a 3-attempt lockout system.
* **Core Banking Operations:**
    * **Deposit:** Add funds to your account.
    * **Withdraw:** Remove funds (with balance validation).
    * **Balance Inquiry:** View current balance and total transaction count.
* **Transaction History:** View a detailed log of your past deposits and withdrawals with timestamps.
* **Persistent Storage:** All data is saved to `.txt` files, so your balance remains saved even after closing the program.
* **Colorful UI:** Uses ANSI escape codes for a clean, color-coded terminal experience.

## 📂 Project Structure

The project is split into modules for better maintainability:

* `main.c`: The entry point and primary menu controller.
* `auth.c`: Handles login logic and account creation.
* `atm_logic.c`: Manages banking operations (Withdraw/Deposit) and File I/O.
* `utils.c`: Helper functions for UI (borders, centering) and input validation.
* `common.h`: Shared structures, constants, and library includes.



## 🛠️ How to Compile and Run

### Prerequisites
* A C compiler (like **GCC**).
* A Windows environment (due to the use of `<conio.h>` for `getch()`).

### Compilation
Open your terminal in the project folder and run:

bash
gcc main.c utils.c atm_logic.c auth.c -o ATM_System

### After Compilation
Run in the same terminal

bash
./ATM_System.exe

Data Management
The system creates two files to store data:

users_data_1.txt: Stores usernames, encrypted PINs (plain text in this version), and balances.

trans.txt: Stores every transaction linked to specific account numbers.

Note: These files are ignored by Git via .gitignore to keep your local database private.

⚠️ Requirements
Operating System: Windows (required for system("cls") and getch()).

Compiler: GCC or any standard C99 compatible compiler.

🤝 Contributing
Feel free to fork this project and add features like:

Transferring money between accounts.

Password encryption (hashing).

Admin panel to view all users.

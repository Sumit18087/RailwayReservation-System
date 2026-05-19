#include <iostream>
#include <string>
#include <limits>
#include "Train.h"
#include "Booking.h"
#include "Admin.h"
#include "Passenger.h"
using namespace std;

// ============================================
//   UTILITY FUNCTIONS
// ============================================
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void clearScreen() {
    // Works on both Windows and Linux
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showBanner() {
    cout << "\n";
    cout << "  " << string(50, '=') << "\n";
    cout << "                                                  \n";
    cout << "      ██████╗  █████╗ ██╗██╗     ██╗    ██╗      \n";
    cout << "      ██╔══██╗██╔══██╗██║██║     ██║    ██║      \n";
    cout << "      ██████╔╝███████║██║██║     ██║ █╗ ██║      \n";
    cout << "      ██╔══██╗██╔══██║██║██║     ██║███╗██║      \n";
    cout << "      ██║  ██║██║  ██║██║███████╗╚███╔███╔╝      \n";
    cout << "      ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚══════╝ ╚══╝╚══╝       \n";
    cout << "                                                  \n";
    cout << "        RAILWAY RESERVATION SYSTEM v1.0           \n";
    cout << "          Built with C++ | File Handling          \n";
    cout << "                                                  \n";
    cout << "  " << string(50, '=') << "\n";
}

void showMainMenu() {
    cout << "\n  " << string(45, '-') << "\n";
    cout << "            MAIN MENU\n";
    cout << "  " << string(45, '-') << "\n";
    cout << "\n  1. Admin Login\n";
    cout << "  2. Passenger Portal\n";
    cout << "  3. About\n";
    cout << "  0. Exit\n";
    cout << "\n  Enter choice: ";
}

void showAbout() {
    cout << "\n  " << string(50, '=') << "\n";
    cout << "              ABOUT THIS PROJECT\n";
    cout << "  " << string(50, '=') << "\n";
    cout << "\n  Project  : Railway Reservation System\n";
    cout << "  Language : C++ (OOP)\n";
    cout << "  Concepts : Classes, Inheritance, STL,\n";
    cout << "             File Handling, Encapsulation\n";
    cout << "\n  Features :\n";
    cout << "  - Admin can add/delete/view trains\n";
    cout << "  - Passengers can search trains by route\n";
    cout << "  - Book tickets with seat allocation\n";
    cout << "  - Cancel tickets with seat restoration\n";
    cout << "  - Data persists via file handling\n";
    cout << "  - Auto-generated unique Booking IDs\n";
    cout << "\n  Default Admin Credentials:\n";
    cout << "  Username : admin\n";
    cout << "  Password : admin123\n";
    cout << "\n  " << string(50, '=') << "\n";
}

// ============================================
//   MAIN FUNCTION
// ============================================
int main() {
    // These two objects manage ALL data
    // and automatically load from files on start
    TrainManager   trainManager;
    BookingManager bookingManager;

    Admin     admin;
    Passenger passenger;

    int choice;

    clearScreen();
    showBanner();

    cout << "\n  Loading data from files...\n";
    cout << "  ✔ System ready!\n";

    do {
        showMainMenu();
        cin >> choice;
        clearInput();

        switch (choice) {

            // ---- ADMIN ----
            case 1: {
                clearScreen();
                if (admin.login()) {
                    admin.showMenu(trainManager, bookingManager);
                }
                break;
            }

            // ---- PASSENGER ----
            case 2: {
                clearScreen();
                cout << "\n  " << string(45, '=') << "\n";
                cout << "         PASSENGER PORTAL\n";
                cout << "  " << string(45, '=') << "\n";
                cout << "\n  Welcome! You can search and book\n";
                cout << "  trains without logging in.\n";
                passenger.showMenu(trainManager, bookingManager);
                break;
            }

            // ---- ABOUT ----
            case 3: {
                showAbout();
                cout << "\n  Press Enter to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            // ---- EXIT ----
            case 0: {
                cout << "\n  " << string(50, '=') << "\n";
                cout << "    Thank you for using Railway Reservation!\n";
                cout << "    Data saved. Goodbye!\n";
                cout << "  " << string(50, '=') << "\n\n";
                break;
            }

            default: {
                cout << "\n  ✘ Invalid choice! Try again.\n";
            }
        }

    } while (choice != 0);

    return 0;
}
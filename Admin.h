#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include <limits>
#include "Train.h"
#include "Booking.h"
using namespace std;

// ============================================
//   ADMIN CLASS
//   All admin operations
// ============================================
class Admin {
private:
    string username;
    string password;

    // ---------- Input helpers ----------
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void pressEnter() {
        cout << "\n  Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // ---------- Header ----------
    void showHeader() {
        cout << "\n  " << string(45, '=') << "\n";
        cout << "         ADMIN PANEL\n";
        cout << "  " << string(45, '=') << "\n";
    }

public:
    Admin() {
        // Hardcoded admin credentials
        // In real world this would come from a file/db
        username = "admin";
        password = "admin123";
    }

    // ----------------------------------------
    //   LOGIN
    // ----------------------------------------
    bool login() {
        string uname, pwd;
        int attempts = 3;

        cout << "\n  " << string(45, '-') << "\n";
        cout << "            ADMIN LOGIN\n";
        cout << "  " << string(45, '-') << "\n";

        while (attempts > 0) {
            cout << "\n  Username : ";
            cin >> uname;
            cout << "  Password : ";
            cin >> pwd;

            if (uname == username && pwd == password) {
                cout << "\n  ✔ Login successful! Welcome, Admin.\n";
                clearInput();
                return true;
            } else {
                attempts--;
                if (attempts > 0) {
                    cout << "\n  ✘ Wrong credentials! "
                         << attempts << " attempt(s) left.\n";
                } else {
                    cout << "\n  ✘ Too many failed attempts!\n";
                }
            }
        }
        clearInput();
        return false;
    }

    // ----------------------------------------
    //   ADMIN MENU
    // ----------------------------------------
    void showMenu(TrainManager &tm, BookingManager &bm) {
        int choice;
        do {
            showHeader();
            cout << "\n  1. Add Train\n";
            cout << "  2. View All Trains\n";
            cout << "  3. Delete Train\n";
            cout << "  4. View All Bookings\n";
            cout << "  5. Search Booking by ID\n";
            cout << "  6. View Dashboard Summary\n";
            cout << "  0. Logout\n";
            cout << "\n  Enter choice: ";
            cin >> choice;
            clearInput();

            switch (choice) {
                case 1: addTrain(tm);              break;
                case 2: viewTrains(tm);            break;
                case 3: deleteTrain(tm);           break;
                case 4: viewBookings(bm);          break;
                case 5: searchBooking(bm);         break;
                case 6: showDashboard(tm, bm);     break;
                case 0: cout << "\n  Logged out.\n"; break;
                default: cout << "\n  ✘ Invalid choice!\n";
            }

            if (choice != 0) pressEnter();

        } while (choice != 0);
    }

    // ----------------------------------------
    //   ADD TRAIN
    // ----------------------------------------
    void addTrain(TrainManager &tm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "            ADD NEW TRAIN\n";
        cout << "  " << string(45, '-') << "\n";

        int trainNo, seats;
        string name, src, dest;
        double fare;

        cout << "\n  Train Number   : ";
        cin >> trainNo;
        clearInput();

        // check duplicate
        if (tm.trainExists(trainNo)) {
            cout << "\n  ✘ Train number already exists!\n";
            return;
        }

        cout << "  Train Name     : ";
        getline(cin, name);

        cout << "  Source         : ";
        getline(cin, src);

        cout << "  Destination    : ";
        getline(cin, dest);

        cout << "  Total Seats    : ";
        cin >> seats;

        cout << "  Fare (Rs.)     : ";
        cin >> fare;
        clearInput();

        Train t(trainNo, name, src, dest, seats, fare);
        tm.addTrain(t);

        cout << "\n  Train Details:\n";
        cout << "  " << string(45, '-') << "\n";
        cout << "  Train No   : " << trainNo << "\n";
        cout << "  Name       : " << name    << "\n";
        cout << "  Route      : " << src << " → " << dest << "\n";
        cout << "  Seats      : " << seats   << "\n";
        cout << "  Fare       : Rs." << fare << "\n";
    }

    // ----------------------------------------
    //   VIEW ALL TRAINS
    // ----------------------------------------
    void viewTrains(TrainManager &tm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "           ALL TRAINS\n";
        cout << "  " << string(45, '-') << "\n";
        tm.viewAllTrains();
    }

    // ----------------------------------------
    //   DELETE TRAIN
    // ----------------------------------------
    void deleteTrain(TrainManager &tm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "           DELETE TRAIN\n";
        cout << "  " << string(45, '-') << "\n";

        tm.viewAllTrains();

        int trainNo;
        cout << "\n  Enter Train Number to delete: ";
        cin >> trainNo;
        clearInput();

        char confirm;
        cout << "  Are you sure? (y/n): ";
        cin >> confirm;
        clearInput();

        if (confirm == 'y' || confirm == 'Y') {
            tm.deleteTrain(trainNo);
        } else {
            cout << "\n  Deletion cancelled.\n";
        }
    }

    // ----------------------------------------
    //   VIEW ALL BOOKINGS
    // ----------------------------------------
    void viewBookings(BookingManager &bm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "          ALL BOOKINGS\n";
        cout << "  " << string(45, '-') << "\n";
        bm.viewAllBookings();
    }

    // ----------------------------------------
    //   SEARCH BOOKING BY ID
    // ----------------------------------------
    void searchBooking(BookingManager &bm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "         SEARCH BOOKING\n";
        cout << "  " << string(45, '-') << "\n";

        string bookingId;
        cout << "\n  Enter Booking ID (e.g. BK1001): ";
        getline(cin, bookingId);

        bm.printTicket(bookingId);
    }

    // ----------------------------------------
    //   DASHBOARD SUMMARY
    // ----------------------------------------
    void showDashboard(TrainManager &tm, BookingManager &bm) {
        cout << "\n  " << string(45, '=') << "\n";
        cout << "           DASHBOARD SUMMARY\n";
        cout << "  " << string(45, '=') << "\n";
        cout << "\n  Total Trains   : " << tm.getTotalTrains()   << "\n";
        cout << "  Total Bookings : " << bm.getTotalBookings() << "\n";
        cout << "\n  " << string(45, '=') << "\n";
    }
};

#endif
#ifndef PASSENGER_H
#define PASSENGER_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "Train.h"
#include "Booking.h"
using namespace std;

// ============================================
//   PASSENGER CLASS
//   All passenger operations
// ============================================
class Passenger {
private:
    string name;
    int    age;
    char   gender;

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
        cout << "         PASSENGER PANEL\n";
        cout << "  " << string(45, '=') << "\n";
    }

    // ---------- Validate date format dd/mm/yyyy ----------
    bool isValidDate(string date) {
        if (date.length() != 10) return false;
        if (date[2] != '/' || date[5] != '/') return false;
        int day   = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year  = stoi(date.substr(6, 4));
        if (day < 1 || day > 31)   return false;
        if (month < 1 || month > 12) return false;
        if (year < 2024)           return false;
        return true;
    }

public:
    Passenger() {}

    // ----------------------------------------
    //   PASSENGER DETAILS ENTRY
    // ----------------------------------------
    bool enterDetails() {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "        ENTER YOUR DETAILS\n";
        cout << "  " << string(45, '-') << "\n";

        cout << "\n  Your Name   : ";
        getline(cin, name);

        if (name.empty()) {
            cout << "\n  ✘ Name cannot be empty!\n";
            return false;
        }

        cout << "  Your Age    : ";
        cin >> age;

        if (age < 1 || age > 120) {
            cout << "\n  ✘ Invalid age!\n";
            clearInput();
            return false;
        }

        cout << "  Gender(M/F) : ";
        cin >> gender;
        gender = toupper(gender);

        if (gender != 'M' && gender != 'F') {
            cout << "\n  ✘ Invalid gender! Enter M or F.\n";
            clearInput();
            return false;
        }

        clearInput();
        return true;
    }

    // ----------------------------------------
    //   PASSENGER MENU
    // ----------------------------------------
    void showMenu(TrainManager &tm, BookingManager &bm) {
        int choice;
        do {
            showHeader();
            cout << "\n  1. Search Trains\n";
            cout << "  2. Book Ticket\n";
            cout << "  3. View My Bookings\n";
            cout << "  4. Cancel Ticket\n";
            cout << "  5. Print Ticket\n";
            cout << "  0. Back to Main Menu\n";
            cout << "\n  Enter choice: ";
            cin >> choice;
            clearInput();

            switch (choice) {
                case 1: searchTrains(tm);          break;
                case 2: bookTicket(tm, bm);        break;
                case 3: viewMyBookings(bm);        break;
                case 4: cancelTicket(tm, bm);      break;
                case 5: printTicket(bm);           break;
                case 0: cout << "\n  Going back...\n"; break;
                default: cout << "\n  ✘ Invalid choice!\n";
            }

            if (choice != 0) pressEnter();

        } while (choice != 0);
    }

    // ----------------------------------------
    //   SEARCH TRAINS
    // ----------------------------------------
    void searchTrains(TrainManager &tm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "          SEARCH TRAINS\n";
        cout << "  " << string(45, '-') << "\n";

        string src, dest;
        cout << "\n  From (Source)      : ";
        getline(cin, src);
        cout << "  To   (Destination) : ";
        getline(cin, dest);

        vector<Train> results = tm.searchTrains(src, dest);

        if (results.empty()) {
            cout << "\n  ✘ No trains found for "
                 << src << " → " << dest << "\n";
            return;
        }

        cout << "\n  Found " << results.size() << " train(s):\n\n";
        cout << string(75, '-') << "\n";
        cout << left
             << setw(8)  << "TrainNo"
             << setw(22) << "Train Name"
             << setw(15) << "Source"
             << setw(15) << "Destination"
             << setw(10) << "Seats"
             << "Fare"
             << "\n";
        cout << string(75, '-') << "\n";
        for (auto t : results) {
            t.display();
        }
        cout << string(75, '-') << "\n";
    }

    // ----------------------------------------
    //   BOOK TICKET
    // ----------------------------------------
    void bookTicket(TrainManager &tm, BookingManager &bm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "           BOOK TICKET\n";
        cout << "  " << string(45, '-') << "\n";

        // Step 1: Search train
        string src, dest;
        cout << "\n  From (Source)      : ";
        getline(cin, src);
        cout << "  To   (Destination) : ";
        getline(cin, dest);

        vector<Train> results = tm.searchTrains(src, dest);

        if (results.empty()) {
            cout << "\n  ✘ No trains found for this route!\n";
            return;
        }

        // Step 2: Show available trains
        cout << "\n  Available Trains:\n\n";
        cout << string(75, '-') << "\n";
        cout << left
             << setw(8)  << "TrainNo"
             << setw(22) << "Train Name"
             << setw(15) << "Source"
             << setw(15) << "Destination"
             << setw(10) << "Seats"
             << "Fare"
             << "\n";
        cout << string(75, '-') << "\n";
        for (auto t : results) {
            t.display();
        }
        cout << string(75, '-') << "\n";

        // Step 3: Select train
        int trainNo;
        cout << "\n  Enter Train Number to book: ";
        cin >> trainNo;
        clearInput();

        Train* selectedTrain = tm.findTrain(trainNo);
        if (!selectedTrain) {
            cout << "\n  ✘ Invalid train number!\n";
            return;
        }

        // Step 4: Check seat availability
        if (selectedTrain->getAvailableSeats() == 0) {
            cout << "\n  ✘ Sorry! No seats available on this train.\n";
            return;
        }

        // Step 5: Journey date
        string journeyDate;
        cout << "  Journey Date (dd/mm/yyyy): ";
        getline(cin, journeyDate);

        if (!isValidDate(journeyDate)) {
            cout << "\n  ✘ Invalid date format! Use dd/mm/yyyy\n";
            return;
        }

        // Step 6: Passenger details
        cout << "\n  --- Passenger Details ---\n";
        if (!enterDetails()) return;

        // Step 7: Confirm booking
        int seatNo = selectedTrain->getTotalSeats()
                   - selectedTrain->getAvailableSeats() + 1;

        cout << "\n  " << string(45, '-') << "\n";
        cout << "         BOOKING SUMMARY\n";
        cout << "  " << string(45, '-') << "\n";
        cout << "  Train      : " << selectedTrain->getTrainName() << "\n";
        cout << "  Route      : " << src << " → " << dest           << "\n";
        cout << "  Date       : " << journeyDate                     << "\n";
        cout << "  Passenger  : " << name                           << "\n";
        cout << "  Seat No    : " << seatNo                         << "\n";
        cout << "  Fare       : Rs." << selectedTrain->getFare()    << "\n";
        cout << "  " << string(45, '-') << "\n";

        char confirm;
        cout << "\n  Confirm booking? (y/n): ";
        cin >> confirm;
        clearInput();

        if (confirm != 'y' && confirm != 'Y') {
            cout << "\n  Booking cancelled.\n";
            return;
        }

        // Step 8: Book seat
        if (selectedTrain->bookSeat()) {
            tm.saveToFile();  // update available seats in file

            string bId = bm.addBooking(
                trainNo,
                selectedTrain->getTrainName(),
                src, dest,
                name, age, gender,
                seatNo, journeyDate,
                selectedTrain->getFare()
            );

            cout << "\n  ✔ Booking Confirmed!\n";
            cout << "  Your Booking ID: " << bId << "\n";
            cout << "  (Save this ID to view/cancel your ticket)\n";

            // Print ticket immediately
            bm.printTicket(bId);
        } else {
            cout << "\n  ✘ Booking failed! Please try again.\n";
        }
    }

    // ----------------------------------------
    //   VIEW MY BOOKINGS
    // ----------------------------------------
    void viewMyBookings(BookingManager &bm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "          MY BOOKINGS\n";
        cout << "  " << string(45, '-') << "\n";

        string pName;
        cout << "\n  Enter your name: ";
        getline(cin, pName);

        bm.viewMyBookings(pName);
    }

    // ----------------------------------------
    //   CANCEL TICKET
    // ----------------------------------------
    void cancelTicket(TrainManager &tm, BookingManager &bm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "          CANCEL TICKET\n";
        cout << "  " << string(45, '-') << "\n";

        string bookingId;
        cout << "\n  Enter Booking ID: ";
        getline(cin, bookingId);

        Booking* b = bm.findBooking(bookingId);
        if (!b) {
            cout << "\n  ✘ Booking ID not found!\n";
            return;
        }

        if (b->getStatus() == "CANCELLED") {
            cout << "\n  ✘ This ticket is already cancelled!\n";
            return;
        }

        // Show ticket before cancelling
        bm.printTicket(bookingId);

        char confirm;
        cout << "\n  Are you sure you want to cancel? (y/n): ";
        cin >> confirm;
        clearInput();

        if (confirm == 'y' || confirm == 'Y') {
            // restore seat in train
            Train* t = tm.findTrain(b->getTrainNo());
            if (t) {
                t->cancelSeat();
                tm.saveToFile();
            }

            bm.cancelBooking(bookingId);
            cout << "\n  ✔ Ticket cancelled successfully!\n";
            cout << "  Refund will be processed within 3-5 days.\n";
        } else {
            cout << "\n  Cancellation aborted.\n";
        }
    }

    // ----------------------------------------
    //   PRINT TICKET
    // ----------------------------------------
    void printTicket(BookingManager &bm) {
        cout << "\n  " << string(45, '-') << "\n";
        cout << "           PRINT TICKET\n";
        cout << "  " << string(45, '-') << "\n";

        string bookingId;
        cout << "\n  Enter Booking ID: ";
        getline(cin, bookingId);

        bm.printTicket(bookingId);
    }
};

#endif
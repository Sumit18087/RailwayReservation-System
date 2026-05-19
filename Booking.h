#ifndef BOOKING_H
#define BOOKING_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

// ============================================
//   BOOKING CLASS
//   Stores one ticket booking record
// ============================================
class Booking {
private:
    string bookingId;
    int    trainNo;
    string trainName;
    string source;
    string destination;
    string passengerName;
    int    passengerAge;
    char   passengerGender;  // M / F
    int    seatNo;
    string journeyDate;
    string bookingDate;
    double fare;
    string status;           // "CONFIRMED" or "CANCELLED"

public:
    // ---------- Constructors ----------
    Booking() {}

    Booking(string bId, int tNo, string tName, string src, string dest,
            string pName, int pAge, char pGender,
            int seat, string jDate, double f) {
        bookingId       = bId;
        trainNo         = tNo;
        trainName       = tName;
        source          = src;
        destination     = dest;
        passengerName   = pName;
        passengerAge    = pAge;
        passengerGender = pGender;
        seatNo          = seat;
        journeyDate     = jDate;
        fare            = f;
        status          = "CONFIRMED";

        // auto set today as booking date
        time_t now = time(0);
        tm *ltm = localtime(&now);
        bookingDate = to_string(ltm->tm_mday) + "/" +
                      to_string(1 + ltm->tm_mon) + "/" +
                      to_string(1900 + ltm->tm_year);
    }

    // ---------- Getters ----------
    string getBookingId()      { return bookingId; }
    int    getTrainNo()        { return trainNo; }
    string getPassengerName()  { return passengerName; }
    string getStatus()         { return status; }
    string getJourneyDate()    { return journeyDate; }

    // ---------- Cancel this booking ----------
    void cancel() { status = "CANCELLED"; }

    // ---------- Print Ticket ----------
    void printTicket() {
        cout << "\n";
        cout << "  " << string(50, '=') << "\n";
        cout << "        INDIAN RAILWAY - E-TICKET\n";
        cout << "  " << string(50, '=') << "\n";
        cout << "  Booking ID   : " << bookingId      << "\n";
        cout << "  Booking Date : " << bookingDate     << "\n";
        cout << "  Status       : " << status          << "\n";
        cout << "  " << string(50, '-') << "\n";
        cout << "  Train No     : " << trainNo         << "\n";
        cout << "  Train Name   : " << trainName       << "\n";
        cout << "  From         : " << source          << "\n";
        cout << "  To           : " << destination     << "\n";
        cout << "  Journey Date : " << journeyDate     << "\n";
        cout << "  " << string(50, '-') << "\n";
        cout << "  Passenger    : " << passengerName   << "\n";
        cout << "  Age          : " << passengerAge    << "\n";
        cout << "  Gender       : " << passengerGender << "\n";
        cout << "  Seat No      : " << seatNo          << "\n";
        cout << "  " << string(50, '-') << "\n";
        cout << "  Fare Paid    : Rs." << fixed << setprecision(2) << fare << "\n";
        cout << "  " << string(50, '=') << "\n";
    }

    // ---------- Display one row in list ----------
    void displayRow() {
        cout << left
             << setw(14) << bookingId
             << setw(8)  << trainNo
             << setw(18) << passengerName
             << setw(12) << journeyDate
             << setw(6)  << seatNo
             << status
             << "\n";
    }

    // ---------- Save to file ----------
    // pipe '|' separated
    void saveToFile(ofstream &out) {
        out << bookingId       << "|"
            << trainNo         << "|"
            << trainName       << "|"
            << source          << "|"
            << destination     << "|"
            << passengerName   << "|"
            << passengerAge    << "|"
            << passengerGender << "|"
            << seatNo          << "|"
            << journeyDate     << "|"
            << bookingDate     << "|"
            << fare            << "|"
            << status          << "\n";
    }

    // ---------- Load from file ----------
    void loadFromFile(ifstream &in) {
        char pipe;
        getline(in, bookingId,     '|');
        in >> trainNo >> pipe;
        getline(in, trainName,     '|');
        getline(in, source,        '|');
        getline(in, destination,   '|');
        getline(in, passengerName, '|');
        in >> passengerAge >> pipe
           >> passengerGender >> pipe
           >> seatNo >> pipe;
        getline(in, journeyDate,   '|');
        getline(in, bookingDate,   '|');
        in >> fare >> pipe;
        getline(in, status);
    }
};


// ============================================
//   BOOKING MANAGER CLASS
//   Handles all booking operations + file I/O
// ============================================
class BookingManager {
private:
    vector<Booking> bookings;
    string filename = "bookings.txt";
    int lastBookingNo;   // to generate unique booking IDs

    // Generate unique booking ID like "BK1001", "BK1002"
    string generateBookingId() {
        lastBookingNo++;
        return "BK" + to_string(1000 + lastBookingNo);
    }

public:
    BookingManager() {
        lastBookingNo = 0;
        loadFromFile();
    }

    ~BookingManager() {
        saveToFile();
    }

    // ---------- Add new booking ----------
    string addBooking(int tNo, string tName, string src, string dest,
                      string pName, int pAge, char pGender,
                      int seat, string jDate, double fare) {

        string bId = generateBookingId();
        Booking b(bId, tNo, tName, src, dest,
                  pName, pAge, pGender,
                  seat, jDate, fare);
        bookings.push_back(b);
        saveToFile();
        return bId;  // return booking ID to show user
    }

    // ---------- Find booking by ID ----------
    Booking* findBooking(string bookingId) {
        for (auto &b : bookings) {
            if (b.getBookingId() == bookingId) {
                return &b;
            }
        }
        return nullptr;
    }

    // ---------- Cancel a booking ----------
    bool cancelBooking(string bookingId) {
        Booking* b = findBooking(bookingId);
        if (b && b->getStatus() == "CONFIRMED") {
            b->cancel();
            saveToFile();
            return true;
        }
        return false;
    }

    // ---------- View all bookings (Admin) ----------
    void viewAllBookings() {
        if (bookings.empty()) {
            cout << "\n  No bookings found.\n";
            return;
        }
        cout << "\n";
        cout << string(70, '-') << "\n";
        cout << left
             << setw(14) << "Booking ID"
             << setw(8)  << "TrainNo"
             << setw(18) << "Passenger"
             << setw(12) << "Journey"
             << setw(6)  << "Seat"
             << "Status"
             << "\n";
        cout << string(70, '-') << "\n";
        for (auto &b : bookings) {
            b.displayRow();
        }
        cout << string(70, '-') << "\n";
        cout << "  Total Bookings: " << bookings.size() << "\n";
    }

    // ---------- View bookings by passenger name ----------
    void viewMyBookings(string passengerName) {
        bool found = false;
        cout << "\n";
        cout << string(70, '-') << "\n";
        cout << left
             << setw(14) << "Booking ID"
             << setw(8)  << "TrainNo"
             << setw(18) << "Passenger"
             << setw(12) << "Journey"
             << setw(6)  << "Seat"
             << "Status"
             << "\n";
        cout << string(70, '-') << "\n";
        for (auto &b : bookings) {
            if (b.getPassengerName() == passengerName) {
                b.displayRow();
                found = true;
            }
        }
        if (!found) {
            cout << "  No bookings found for " << passengerName << "\n";
        }
        cout << string(70, '-') << "\n";
    }

    // ---------- Print ticket ----------
    void printTicket(string bookingId) {
        Booking* b = findBooking(bookingId);
        if (b) {
            b->printTicket();
        } else {
            cout << "\n  ✘ Booking ID not found!\n";
        }
    }

    // ---------- Get total confirmed bookings for a train ----------
    int getConfirmedCount(int trainNo) {
        int count = 0;
        for (auto &b : bookings) {
            if (b.getTrainNo() == trainNo &&
                b.getStatus() == "CONFIRMED") {
                count++;
            }
        }
        return count;
    }

    // ---------- Save all bookings ----------
    void saveToFile() {
        ofstream out(filename);
        for (auto &b : bookings) {
            b.saveToFile(out);
        }
        out.close();
    }

    // ---------- Load all bookings ----------
    void loadFromFile() {
        ifstream in(filename);
        if (!in) return;
        while (in.peek() != EOF) {
            Booking b;
            b.loadFromFile(in);
            if (in) {
                bookings.push_back(b);
                lastBookingNo++;  // keep count in sync
            }
        }
        in.close();
    }

    int getTotalBookings() { return bookings.size(); }
};

#endif
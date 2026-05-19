#ifndef TRAIN_H
#define TRAIN_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

// ============================================
//   TRAIN CLASS
//   Stores all details about a train
// ============================================
class Train {
private:
    int trainNo;
    string trainName;
    string source;
    string destination;
    int totalSeats;
    int availableSeats;
    double fare;

public:
    // ---------- Constructors ----------
    Train() {}

    Train(int tNo, string tName, string src, string dest, int seats, double f) {
        trainNo        = tNo;
        trainName      = tName;
        source         = src;
        destination    = dest;
        totalSeats     = seats;
        availableSeats = seats;  // initially all seats available
        fare           = f;
    }

    // ---------- Getters ----------
    int    getTrainNo()         { return trainNo; }
    string getTrainName()       { return trainName; }
    string getSource()          { return source; }
    string getDestination()     { return destination; }
    int    getTotalSeats()      { return totalSeats; }
    int    getAvailableSeats()  { return availableSeats; }
    double getFare()            { return fare; }

    // ---------- Setters ----------
    void setAvailableSeats(int s) { availableSeats = s; }

    // ---------- Book / Cancel a seat ----------
    bool bookSeat() {
        if (availableSeats > 0) {
            availableSeats--;
            return true;   // booking success
        }
        return false;      // train is full
    }

    void cancelSeat() {
        if (availableSeats < totalSeats) {
            availableSeats++;
        }
    }

    // ---------- Display one train row ----------
    void display() {
        cout << left
             << setw(8)  << trainNo
             << setw(22) << trainName
             << setw(15) << source
             << setw(15) << destination
             << setw(10) << availableSeats
             << "Rs." << fare
             << endl;
    }

    // ---------- Save to file ----------
    // Format: trainNo|trainName|source|destination|totalSeats|availableSeats|fare
    void saveToFile(ofstream &out) {
        out << trainNo        << "|"
            << trainName      << "|"
            << source         << "|"
            << destination    << "|"
            << totalSeats     << "|"
            << availableSeats << "|"
            << fare           << "\n";
    }

    // ---------- Load from file ----------
    void loadFromFile(ifstream &in) {
        char pipe;  // to read the '|' separator
        in >> trainNo >> pipe;
        getline(in, trainName,   '|');
        getline(in, source,      '|');
        getline(in, destination, '|');
        in >> totalSeats >> pipe
           >> availableSeats >> pipe
           >> fare;
        in.ignore();  // skip newline
    }
};


// ============================================
//   TRAIN MANAGER CLASS
//   Handles all train operations + file I/O
// ============================================
class TrainManager {
private:
    vector<Train> trains;          // all trains in memory
    string filename = "trains.txt";

public:
    // Load trains from file when program starts
    TrainManager() {
        loadFromFile();
    }

    // Save trains to file when program ends
    ~TrainManager() {
        saveToFile();
    }

    // ---------- Add a new train ----------
    void addTrain(Train t) {
        trains.push_back(t);
        saveToFile();
        cout << "\n  ✔ Train added successfully!\n";
    }

    // ---------- View all trains ----------
    void viewAllTrains() {
        if (trains.empty()) {
            cout << "\n  No trains available.\n";
            return;
        }
        cout << "\n";
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
        for (auto &t : trains) {
            t.display();
        }
        cout << string(75, '-') << "\n";
    }

    // ---------- Search trains by source & destination ----------
    vector<Train> searchTrains(string src, string dest) {
        vector<Train> result;
        for (auto &t : trains) {
            // case-insensitive would need more work; keeping simple for now
            if (t.getSource() == src && t.getDestination() == dest) {
                result.push_back(t);
            }
        }
        return result;
    }

    // ---------- Find a train by number ----------
    Train* findTrain(int trainNo) {
        for (auto &t : trains) {
            if (t.getTrainNo() == trainNo) {
                return &t;
            }
        }
        return nullptr;  // not found
    }

    // ---------- Delete a train ----------
    void deleteTrain(int trainNo) {
        for (int i = 0; i < trains.size(); i++) {
            if (trains[i].getTrainNo() == trainNo) {
                trains.erase(trains.begin() + i);
                saveToFile();
                cout << "\n  ✔ Train deleted successfully!\n";
                return;
            }
        }
        cout << "\n  ✘ Train not found!\n";
    }

    // ---------- Save all trains to file ----------
    void saveToFile() {
        ofstream out(filename);
        for (auto &t : trains) {
            t.saveToFile(out);
        }
        out.close();
    }

    // ---------- Load all trains from file ----------
    void loadFromFile() {
        ifstream in(filename);
        if (!in) return;  // file doesn't exist yet, that's ok
        while (in.peek() != EOF) {
            Train t;
            t.loadFromFile(in);
            if (in)  // only add if read was successful
                trains.push_back(t);
        }
        in.close();
    }

    // ---------- Check if train number already exists ----------
    bool trainExists(int trainNo) {
        return findTrain(trainNo) != nullptr;
    }

    int getTotalTrains() { return trains.size(); }
};

#endif
#pragma once
#include <iostream>
#include "09_Payment.cpp"
using namespace std;

// Inheritance: each subclass IS-A Payment and fulfils pay() independently.
// Compile-time polymorphism: two constructors per class (default success
// path, and a demo-only path that lets us simulate a failed payment for
// edge-case testing without touching BookingService).

class UpiPayment : public Payment {
private:
    bool simulateFailure;

public:
    UpiPayment() : simulateFailure(false) {}
    explicit UpiPayment(bool simulateFailure) : simulateFailure(simulateFailure) {}

    bool pay(double amount) override {
        if (simulateFailure) {
            cout << "  [UPI] payment FAILED" << endl;
            return false;
        }
        cout << "  [UPI] Rs." << amount << " paid successfully" << endl;
        return true;
    }
};

class CardPayment : public Payment {
private:
    bool simulateFailure;

public:
    CardPayment() : simulateFailure(false) {}
    explicit CardPayment(bool simulateFailure) : simulateFailure(simulateFailure) {}

    bool pay(double amount) override {
        if (simulateFailure) {
            cout << "  [CARD] payment FAILED" << endl;
            return false;
        }
        cout << "  [CARD] Rs." << amount << " paid successfully" << endl;
        return true;
    }
};

class CashPayment : public Payment {
private:
    bool simulateFailure;

public:
    CashPayment() : simulateFailure(false) {}
    explicit CashPayment(bool simulateFailure) : simulateFailure(simulateFailure) {}

    bool pay(double amount) override {
        if (simulateFailure) {
            cout << "  [CASH] payment FAILED" << endl;
            return false;
        }
        cout << "  [CASH] Rs." << amount << " paid successfully" << endl;
        return true;
    }
};

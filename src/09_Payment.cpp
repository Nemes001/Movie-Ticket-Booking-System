#pragma once

// Abstraction: declares the contract every payment method must fulfil,
// with no implementation of its own.
class Payment {
public:
    virtual bool pay(double amount) = 0; // pure virtual
    virtual ~Payment() {}
};

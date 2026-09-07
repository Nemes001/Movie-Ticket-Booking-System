#pragma once
#include <vector>
#include "06_ShowSeat.cpp"
using namespace std;

// Single responsibility: this class does ONE thing - pricing.
// It knows nothing about Booking, Payment, or Show.
class PriceCalculator {
public:
    static double calculateTotal(const vector<ShowSeat*>& seats) {
        double total = 0.0;
        for (ShowSeat* s : seats) {
            total += priceForSeatType(s->getSeat().getSeatType());
        }
        return total;
    }
};

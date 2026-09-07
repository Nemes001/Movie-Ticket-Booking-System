# Design Document — Movie Ticket Booking System

TCS-504 (System Design) — Assignment 1

---

## Step A — Requirement Analysis

### Functional Requirements

- **FR1** — Movie listing: the system displays every movie currently flagged
  as playing, showing title, language, and duration. If no movies are
  playing, it displays a clear "no movies available" message instead of an
  empty list.
- **FR2** — Show listing: given a chosen movie, the system lists that
  movie's shows (screen number + start time). If the movie has no shows, it
  says so rather than showing nothing.
- **FR3** — Seat layout: given a chosen show, the system displays that
  show's seats grouped by type, each marked AVAILABLE or BOOKED. Choosing an
  invalid show id returns an error and displays no layout.
- **FR4** — Booking: a customer selects one or more seat numbers for a
  show. If **any** selected seat is already BOOKED, the whole booking is
  rejected and no seat changes state. Booking is confirmed **only after**
  payment succeeds.
- **FR5** — Pricing: SILVER = ₹150, GOLD = ₹250, PLATINUM = ₹400 per seat.
  The total is the sum of the selected seats' type-prices, computed before
  payment is attempted, and is never computed on already-rejected seats.
- **FR6** — Payment: exactly one method (UPI / Card / Cash) per booking. If
  payment fails, seats are released and booking status becomes FAILED.
- **FR7** — Ticket printing: only for a booking whose status is CONFIRMED,
  the system prints booking id, movie, screen, show time, seat numbers, and
  total amount. No ticket is printed for a FAILED or CANCELLED booking.
- **FR8** — Cancellation: given a CONFIRMED booking id, the system sets its
  status to CANCELLED and marks its seats AVAILABLE for that show. An
  unknown or already-cancelled booking id is rejected with an error and
  changes nothing.

### Non-Functional Requirements

- **NFR1 (Modularity)** — Each class is defined in its own file; no class
  exceeds one clear responsibility (verified against Step C).
- **NFR2 (Extensibility)** — Adding a new payment method (e.g.
  NetBanking) requires only a new class extending `Payment`; zero lines
  change in `BookingService` or any existing class.
- **NFR3 (Robustness)** — Invalid menu choices, invalid seat numbers, and
  invalid ids never crash the program; each produces a clear message and
  returns to the menu.
- **NFR4 (Consistency)** — A booking never ends up in a state where seats
  are marked BOOKED but the booking itself is not CONFIRMED (i.e. a failed
  payment always fully releases its seats).

---

## Step B — Noun–Verb Analysis

| Noun found | Keep as a class? | Reason |
|---|---|---|
| Movie | yes | has its own data and identity |
| Seat | yes | has number, type, price |
| "seat layout" | no | a view of a Show's seats, not a thing → a print method |
| Cinema | yes | the theatre itself; owns screens |
| Screen | yes | one auditorium; has a number, owns seats |
| Show | yes | a screening = movie + screen + time; distinct identity from either |
| Customer | yes | has name and phone; identity separate from a booking |
| Seat status (AVAILABLE/BOOKED) | no | a property of a seat *for a specific show* → becomes the `ShowSeat` class instead of a bare attribute |
| Booking | yes | has its own id, total, status — exists independently of any one class above |
| Ticket | no | not a stored/stateful object — a formatted printout of a Booking → a method on `TicketPrinter` |
| Payment | yes | has its own behavior (`pay()`) and varies by method → an abstract class |
| UPI / Card / Cash | yes | each has distinct payment logic → subclasses of `Payment` |
| Price / amount | no | a computed number → a method on `PriceCalculator`, and a field on `Booking` |
| Menu / choice | no | console I/O, not a domain concept → lives in `main`, not a business class |

**Verbs found → methods:** listMovies(), chooseShow(), showSeatLayout(),
bookSeats(), pay(amount), printTicket(), cancelBooking()

---

## Step C — Class Responsibilities

### Core entity classes

| Class | Knows | Does | Must NOT |
|---|---|---|---|
| Movie | title, language, duration | exposes its details | know about shows, screens, or bookings |
| Seat | seat number, seat type | exposes its number and type | know whether it's booked — that's per-show |
| Screen | screen number, its Seats | provides access to its seat layout | know which movie/show is playing on it |
| Cinema | name, its Screens | provides access to screens | handle booking logic or payment |
| Show | Movie ref, Screen ref, start time, ShowSeats | reports seat availability for this screening | store movie/seat data itself |
| ShowSeat | a Seat ref, status (AVAILABLE/BOOKED) *for this show* | toggles its own status | know about pricing or payment |
| Customer | name, phone | exposes its identity | hold booking history or payment logic |
| Booking | booking id, Show ref, booked ShowSeats, total, status | tracks its own state transitions | calculate price, process payment, or print itself |

### Behaviour / service classes

| Class | Knows | Does | Must NOT |
|---|---|---|---|
| Payment (abstract) | nothing concrete | declares `pay(amount)` as pure virtual | implement any actual payment logic |
| UpiPayment / CardPayment / CashPayment | method-specific detail | implements `pay(amount)` | touch Booking or ShowSeat state directly |
| PriceCalculator | nothing persistent | turns a seat list into a total | know about Booking, Payment, or Show |
| TicketPrinter | nothing persistent | formats and prints a ticket | modify any state |
| BookingService | nothing persistent (orchestrator) | runs the full booking flow end to end | implement pricing or payment logic itself |
| Cinema / main menu | nothing domain-specific | reads input, drives the menu loop | contain business logic |

---

## Step D — Relationships

| Pair | Choice | Justification (lifetime test) |
|---|---|---|
| Cinema — Screen | Composition | A Screen has no meaning outside its Cinema; destroy the Cinema, its Screens go too. |
| Screen — Seat | Composition | A Seat is built into one Screen; removing the Screen removes its seats. |
| Show — Movie | Aggregation | A Show borrows a Movie. Cancel the show and the movie still exists and still plays elsewhere. |
| Show — Screen | Aggregation | A Show uses a Screen for a slot but doesn't own it; the Screen survives the Show. |
| Show — ShowSeat | Composition | A ShowSeat only means something for its one Show; it isn't reused by another show. |
| Booking — Customer | Association | Neither owns the other's lifecycle; the Customer exists before/after the Booking. |
| Booking — ShowSeat | Aggregation | The Booking borrows existing ShowSeats; on cancellation they survive and flip back to AVAILABLE. |
| Booking — Payment | Composition | A Payment attempt is created for one Booking and has no meaning detached from it. |
| Payment — UpiPayment | Inheritance | UpiPayment *is a* Payment, fulfilling the same contract via specialization. |
| BookingService — Booking | Association | BookingService operates on Bookings but doesn't own their long-term lifecycle. |

---

## Step H — SOLID Mapping

- **Single Responsibility** — `Booking` doesn't print itself (`TicketPrinter`
  does) and doesn't price itself (`PriceCalculator` does). A pricing-rule
  change or a receipt-format change never touches `Booking`.
- **Open/Closed** — Adding `NetBankingPayment` requires exactly one new
  class extending `Payment`; `BookingService::bookSeats()` never changes,
  since it only calls `payment->pay(total)` through the abstraction.
- **Liskov Substitution** — `UpiPayment`, `CardPayment`, and `CashPayment`
  all satisfy `bool pay(double amount)` with no extra setup call and no
  widened preconditions.
- **Interface Segregation** — `Payment` deliberately declares only `pay()`,
  not `refund()` — forcing `refund()` on every subclass would make
  `CashPayment` implement a method it can't meaningfully support.
- **Dependency Inversion** — `BookingService` depends on the `Payment`
  abstraction, never on a concrete subclass directly; the concrete class is
  chosen only at the call site.

**One thing deliberately NOT done:** `refund()` is not part of `Payment` —
only some methods would meaningfully support it (see ISP above), and the
assignment scope doesn't call for partial refunds. A "simulate payment
failure" prompt was added to `main.cpp` purely for demo purposes, so the
FR6 failure path can be triggered live without a real payment gateway.

---

## Edge cases verified against the code

1. Booking a seat that is already BOOKED → rejected, nothing changes.
2. A failed payment → booking NOT confirmed, seats released.
3. Cancelling a booking → those seats show AVAILABLE again.
4. An invalid seat number or menu choice → a clear message, no crash.

#define _CRT_SECURE_NO_WARNINGS
#include "FlightSystem.h"
#include "Utility.h"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <thread>

using namespace std;

FlightSystem::FlightSystem() {
  passengerCount = 0;
  isAdmin = false;
}

void FlightSystem::setAdmin(bool status) { isAdmin = status; }

void FlightSystem::initialize() {
  // Logic moved to initializeFlights or handled by constructors
  passengerCount = 0;
}

void FlightSystem::initializeFlights() {
  const string countries[] = {
      "USA",         "UK",           "France",   "Germany",     "Japan",
      "China",       "Australia",    "Canada",   "Brazil",      "India",
      "Russia",      "Italy",        "Spain",    "Mexico",      "Indonesia",
      "Netherlands", "Saudi Arabia", "Turkey",   "Switzerland", "Sweden",
      "UAE",         "Singapore",    "Thailand", "Vietnam",     "Malaysia",
      "South Korea", "Egypt",        "Greece",   "Argentina",   "Portugal"};
  const string cities[] = {
      "New York",  "London",    "Paris",    "Berlin",       "Tokyo",
      "Beijing",   "Sydney",    "Toronto",  "Rio",          "Mumbai",
      "Moscow",    "Rome",      "Madrid",   "Mexico City",  "Jakarta",
      "Amsterdam", "Riyadh",    "Istanbul", "Zurich",       "Stockholm",
      "Dubai",     "Singapore", "Bangkok",  "Ho Chi Minh",  "Kuala Lumpur",
      "Seoul",     "Cairo",     "Athens",   "Buenos Aires", "Lisbon"};

  srand(time(0));

  for (int i = 0; i < MAX_FLIGHTS; i++) {
    FlightStruct f;
    f.id = 1 + i;
    int locIdx = rand() % 30;
    f.destination = cities[locIdx] + ", " + countries[locIdx];

    int day = 1 + (rand() % 31);
    string dayStr = (day < 10 ? "0" : "") + to_string(day);
    f.date = dayStr + "/" + "01/2026";

    int hour = 8 + (rand() % 14);
    string hourStr = (hour < 10 ? "0" : "") + to_string(hour);
    f.time = hourStr + ":00";

    f.price = 100 + (rand() % 900);
    f.firstClassPrice = f.price * 2.0;

    // Add to Linked List
    flightList.insert(f);
  }
}

void FlightSystem::startupSequence() {
  clearScreen();
  this_thread::sleep_for(chrono::milliseconds(200));
  displayHeader();
  cout << endl;
  loadingBar("    Initializing Data Structures...", 1000);
  loadingBar("    Allocating Linked Lists & Hash Tables...", 800);
  loadingBar("    System ready!", 500);

  // Load Bookings from File
  cout << Color::DIM << "\n    Loading saved bookings..." << Color::RESET
       << endl;
  loadBookings(this);

  cout << Color::GREEN
       << "\n    ✓ Welcome to AeroLogic Airline Reservation System!"
       << Color::RESET << endl;
  initializeFlights();
  this_thread::sleep_for(chrono::milliseconds(1000));
}

void FlightSystem::exitSequence() {
  clearScreen();
  displayHeader();
  cout << Color::CYAN;
  typingAnimation("\n    Thank you for using AeroLogic!", 40);
  typingAnimation("    We hope to see you again soon.", 40);
  cout << Color::RESET;
  cout << "\n\n                        Safe travels! Goodbye!\n\n" << endl;
  this_thread::sleep_for(chrono::milliseconds(1500));
}

int FlightSystem::displayMenu() {
  if (isAdmin) {
    const string options[] = {"[1] Book a Seat",
                              "[2] Cancel Booking",
                              "[3] Modify Booking",
                              "[4] View Seat Map",
                              "[5] Search Passenger (Hash Table)",
                              "[6] Display All (Linked List)",
                              "[7] Display Booking Statistics",
                              "[8] Print Boarding Passes",
                              "[9] Undo Last Booking (Stack)",
                              "[0] Exit System"};
    int selection = displayInteractiveMenu(options, 10, "ADMINISTRATOR PANEL");

    if (selection == 9)
      return 0;
    return selection + 1;
  } else {
    // Restricted Menu for Passengers
    const string options[] = {
        "[1] Book a Seat",           "[2] View Seat Map",
        "[3] Search Passenger",      "[4] Display Statistics",
        "[5] Print Boarding Passes", "[0] Exit System"};

    // Size is 6
    int selection =
        displayInteractiveMenu(options, 6, "PASSENGER SELF-SERVICE");

    // Mapping:
    // 0 (Book) -> 1
    // 1 (Map) -> 4
    // 2 (Search) -> 5
    // 3 (Stats) -> 7
    // 4 (Passes) -> 8
    // 5 (Exit) -> 0

    switch (selection) {
    case 0:
      return 1;
    case 1:
      return 4;
    case 2:
      return 5;
    case 3:
      return 7;
    case 4:
      return 8;
    case 5:
      return 0;
    default:
      return 0;
    }
  }
}

// ============================================================================
// Internal Logic
// ============================================================================

string FlightSystem::getClassType(int row) {
  return (row < FIRST_CLASS_ROWS) ? "First Class" : "Economy";
}

bool FlightSystem::isSeatAvailable(Flight *flight, int row, int col) {
  if (!flight)
    return false;
  return flight->cabin[row][col].empty();
}

int FlightSystem::countAvailableSeats(Flight *flight, const string &classType) {
  if (!flight)
    return 0;
  int count = 0;
  int startRow = 0, endRow = ROWS;

  if (classType == "First Class")
    endRow = FIRST_CLASS_ROWS;
  else if (classType == "Economy")
    startRow = FIRST_CLASS_ROWS;

  for (int r = startRow; r < endRow; r++) {
    for (int c = 0; c < COLS; c++) {
      if (flight->cabin[r][c].empty())
        count++;
    }
  }
  return count;
}

int FlightSystem::countOccupiedSeats(Flight *flight, const string &classType) {
  if (!flight)
    return 0;
  int count = 0;
  int startRow = 0, endRow = ROWS;

  if (classType == "First Class")
    endRow = FIRST_CLASS_ROWS;
  else if (classType == "Economy")
    startRow = FIRST_CLASS_ROWS;

  for (int r = startRow; r < endRow; r++) {
    for (int c = 0; c < COLS; c++) {
      if (!flight->cabin[r][c].empty())
        count++;
    }
  }
  return count;
}

void FlightSystem::addPassenger(const string &name, int row, int col,
                                const string &date, const string &time,
                                const string &origin, const string &destination,
                                int flightId, Flight *flight,
                                bool saveChanges) {

  // Handle Null Flight (e.g. when loading from file)
  if (!flight) {
    flight = flightList.search(flightId);
  }

  // Create Passenger Object
  Passenger p(name, formatSeatNumber(row, col), getClassType(row), row, col,
              date, time, origin, destination, flightId);

  // 1. Add to Linked List
  passengerList.insert(p);

  // 2. Add to Hash Table for fast lookup
  passengerMap.insert(p);

  // 3. Add to Undo Stack
  undoStack.push(p);

  // Update Flight Cabin (Visual)
  if (flight) {
    flight->cabin[row][col] = name;
  }
  passengerCount++;

  // Save to File
  if (saveChanges) {
    saveBookings(&passengerList);
  }
}

void FlightSystem::removePassenger(string name) {
  // Find passenger first to get flight details before deleting
  Passenger *p = passengerMap.search(name);
  if (!p)
    return;

  int flightId = p->flightId;
  Flight *flight = flightList.search(flightId);

  // Clear Seat in Cabin
  if (flight) {
    flight->cabin[p->row][p->col] = "";
  }

  // Remove from Hash Table
  passengerMap.remove(name);

  // Remove from Linked List
  passengerList.remove(name);

  passengerCount--;

  // Save changes
  saveBookings(&passengerList);

  // CHECK WAITLIST (Queue)
  if (flight && !flight->waitlist.isEmpty()) {
    cout << Color::YELLOW
         << "\n    ! Seat became available. Checking Waitlist..."
         << Color::RESET << endl;
    Passenger waiter;
    // Retrieve from specific flight's waitlist
    if (flight->waitlist.dequeue(waiter)) {
      cout << Color::GREEN << "    ✓ Automatically booking " << waiter.name
           << " from Waitlist!" << Color::RESET << endl;
      addPassenger(waiter.name, p->row, p->col, p->bookingDate, p->bookingTime,
                   p->origin, p->destination, flightId, flight);
    }
  }
}

// ============================================================================
// Feature Implementations
// ============================================================================

void FlightSystem::displayFlightSchedule(int page) {
  FlightNode *head = flightList.getHead();
  int totalFlights = flightList.getSize();

  if (page < 1)
    page = 1;
  int itemsPerPage = 10;
  int totalPages = (totalFlights + itemsPerPage - 1) / itemsPerPage;
  if (page > totalPages)
    page = totalPages;

  int startIdx = (page - 1) * itemsPerPage;
  int endIdx = startIdx + itemsPerPage;

  clearScreen();
  displayHeader();

  cout << Color::BOLD << Color::CYAN;
  cout << "\n    "
          "╔══════╦══════════════════════════════════╦════════════════════╦════"
          "═══════╦═══════════╗"
       << endl;

  string title = "FLIGHT SELECTION SCHEDULE";
  int contentWidth = 86;
  int titlePadLeft = (contentWidth - title.length()) / 2;
  int titlePadRight = contentWidth - title.length() - titlePadLeft;

  cout << "    ║" << string(titlePadLeft, ' ') << title
       << string(titlePadRight, ' ') << "║" << endl;

  string pageStr = "Page " + to_string(page) + " of " + to_string(totalPages);
  int pagePadLeft = (contentWidth - pageStr.length()) / 2;
  int pagePadRight = contentWidth - pageStr.length() - pagePadLeft;

  cout << "    ║" << string(pagePadLeft, ' ') << pageStr
       << string(pagePadRight, ' ') << "║" << endl;

  cout << "    "
          "╠══════╦══════════════════════════════════╦════════════════════╦════"
          "═══════╦═══════════╣"
       << endl;
  cout << "    ║  ID  ║  DESTINATION                     ║  DATE / TIME       "
          "║  ECON($)  ║  1ST($)   ║"
       << endl;
  cout << "    "
          "╠══════╬══════════════════════════════════╬════════════════════╬════"
          "═══════╬═══════════╣"
       << endl;
  cout << Color::RESET;

  FlightNode *current = head;
  int currentIndex = 0;
  while (current && currentIndex < endIdx) {
    if (currentIndex >= startIdx) {
      FlightStruct &f = current->data;
      cout << "    ║ " << setw(4) << f.id << " ║ " << left << setw(32)
           << f.destination << " ║ " << f.date << " | " << f.time << " ║ "
           << "$" << setw(8) << fixed << setprecision(2) << f.price << " ║ "
           << "$" << setw(8) << fixed << setprecision(2) << f.firstClassPrice
           << " ║" << endl;
    }
    current = current->next;
    currentIndex++;
  }

  cout << "    "
          "╚══════╩══════════════════════════════════╩════════════════════╩════"
          "═══════╩═══════════╝"
       << endl;
  cout << "\n    Commands: [N] Next Page | [P] Prev Page | [ID] Enter Flight "
          "ID | [0] Cancel"
       << endl;
}

void FlightSystem::displaySeatMap() {
  Flight *selectedFlight = nullptr;
  int page = 1;

  while (selectedFlight == nullptr) {
    displayFlightSchedule(page);
    cout << "\n    Enter command: ";
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(input[0]) == 'N')
      page++;
    else if (toupper(input[0]) == 'P')
      page--;
    else if (input == "0")
      return;
    else {
      try {
        int id = stoi(input);
        Flight *f = flightList.search(id);
        if (f) {
          selectedFlight = f;
        } else {
          cout << Color::RED << "    ✗ Invalid Flight ID!" << Color::RESET;
          this_thread::sleep_for(chrono::milliseconds(1000));
        }
      } catch (...) {
        cout << Color::RED << "    ✗ Invalid Input!" << Color::RESET;
        this_thread::sleep_for(chrono::milliseconds(1000));
      }
    }
  }
  displaySeatMap(selectedFlight);
  pressEnterToContinue();
}

// ... Same visual map code ...
void FlightSystem::displaySeatMap(Flight *flight) {
  if (!flight)
    return;
  clearScreen();
  displayHeader();
  // ... Same visual map code ...
  cout << Color::BOLD << Color::WHITE;
  cout << "\n    "
          "+-------------------------------------------------------------------"
          "----+"
       << endl;
  cout
      << "    |                      AIRCRAFT SEAT MAP                        |"
      << endl;
  cout << "    "
          "+-------------------------------------------------------------------"
          "----+"
       << endl;
  cout << "    |     " << Color::GREEN << "[ ] Available" << Color::WHITE
       << "          " << Color::RED << "[X] Occupied" << Color::WHITE
       << "                    |" << endl;
  cout << "    "
          "+-------------------------------------------------------------------"
          "----+"
       << Color::RESET << endl;

  cout << "\n              ";
  for (int c = 0; c < COLS; c++) {
    char colLetter = 'A' + c;
    cout << "   " << colLetter << "  ";
    if (c == 2)
      cout << "   ";
  }
  cout << endl;

  cout << "            +";
  for (int c = 0; c < COLS; c++) {
    cout << "------";
    if (c == 2)
      cout << "---";
  }
  cout << "+" << endl;

  for (int r = 0; r < ROWS; r++) {
    if (r == 0)
      cout << Color::MAGENTA << "  FIRST     " << Color::RESET << "║";
    else if (r == 1)
      cout << Color::MAGENTA << "  CLASS     " << Color::RESET << "║";
    else if (r == 2) {
      cout << "            ╠";
      for (int c = 0; c < COLS; c++) {
        cout << "══════";
        if (c == 2)
          cout << "═══";
      }
      cout << "╣" << endl;
      cout << Color::BLUE << "  ECONOMY   " << Color::RESET << "║";
    } else
      cout << "            ║";

    for (int c = 0; c < COLS; c++) {
      string seatLabel = formatSeatNumber(r, c);
      if (flight->cabin[r][c].empty()) {
        cout << Color::GREEN << " [" << setw(3) << seatLabel << "]"
             << Color::RESET;
      } else {
        cout << Color::RED << " [" << setw(2) << "X" << " ]" << Color::RESET;
      }
      if (c == 2)
        cout << "   ";
    }
    cout << "║ Row " << setw(2) << (r + 1) << endl;
  }
  cout << "            ╚";
  for (int c = 0; c < COLS; c++) {
    cout << "══════";
    if (c == 2)
      cout << "═══";
  }
  cout << "╝" << endl;
}

void FlightSystem::bookSeat() {
  Flight *selectedFlight = nullptr;
  int page = 1;

  while (selectedFlight == nullptr) {
    displayFlightSchedule(page);
    cout << "\n    Enter command: ";
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(input[0]) == 'N')
      page++;
    else if (toupper(input[0]) == 'P')
      page--;
    else if (input == "0")
      return;
    else {
      try {
        int id = stoi(input);
        Flight *f = flightList.search(id);
        if (f) {
          selectedFlight = f;
        } else {
          cout << Color::RED << "    ✗ Invalid Flight ID!" << Color::RESET;
          this_thread::sleep_for(chrono::milliseconds(1000));
        }
      } catch (...) {
        cout << Color::RED << "    ✗ Invalid Input!" << Color::RESET;
        this_thread::sleep_for(chrono::milliseconds(1000));
      }
    }
  }

  clearScreen();
  displayHeader();

  cout << Color::GREEN
       << "\n    Selected Flight: " << selectedFlight->destination
       << Color::RESET << endl;

  string name;
  while (true) {
    cout << "\n    Enter passenger name (Full Triple Name): ";
    getline(cin, name);
    if (validateTripleName(name))
      break;
    cout << Color::RED << "\n    ✗ Invalid name! Must be Triple Name."
         << Color::RESET << endl;
  }

  // HASH TABLE SEARCH (O(1))
  if (passengerMap.search(name) != nullptr) {
    cout << Color::RED
         << "\n    ✗ A passenger with this name already has a booking!"
         << Color::RESET << endl;
    pressEnterToContinue();
    return;
  }

  // CHECK FULL FLIGHT FOR WAITLIST
  int totalAvail = countAvailableSeats(selectedFlight, "First Class") +
                   countAvailableSeats(selectedFlight, "Economy");
  if (totalAvail == 0) {
    cout << Color::RED << "\n    ✗ This flight is fully booked!" << Color::RESET
         << endl;
    cout << "    Would you like to join the " << Color::YELLOW << "WAITLIST"
         << Color::RESET << " (Queue)? (Y/N): ";
    char resp;
    cin >> resp;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (toupper(resp) == 'Y') {
      Passenger p(name, "WL", "Any", -1, -1, selectedFlight->date,
                  selectedFlight->time, "KUL", selectedFlight->destination,
                  selectedFlight->id);
      selectedFlight->waitlist.enqueue(p);
      cout << Color::GREEN << "\n    ✓ Added to waitlist position #"
           << selectedFlight->waitlist.getCount() << Color::RESET << endl;
    }
    pressEnterToContinue();
    return;
  }

  // Class selection (same as before)
  cout << "\n    Select class type:\n    [1] First Class\n    [2] Economy\n    "
          "Enter choice (1-2): ";
  int classChoice = getIntInput();

  while (classChoice != 1 && classChoice != 2) {
    cout << Color::RED << "    Invalid choice. Enter 1 or 2: " << Color::RESET;
    classChoice = getIntInput();
  }

  string requestedClass = (classChoice == 1) ? "First Class" : "Economy";
  if (countAvailableSeats(selectedFlight, requestedClass) == 0) {
    cout << Color::RED << "    Selected class is full." << Color::RESET << endl;
    pressEnterToContinue();
    return;
  }

  displaySeatMap(selectedFlight);
  cout << "\n    Enter seat number (e.g., 1A, 5C): ";
  string seatInput;
  getline(cin, seatInput);

  int row, col;
  if (!parseSeatNumber(seatInput, row, col) ||
      getClassType(row) != requestedClass ||
      !isSeatAvailable(selectedFlight, row, col)) {
    cout << Color::RED << "\n    ✗ Invalid or Occupied Seat!" << Color::RESET
         << endl;
    pressEnterToContinue();
    return;
  }

  addPassenger(name, row, col, selectedFlight->date, selectedFlight->time,
               "KUL", selectedFlight->destination, selectedFlight->id,
               selectedFlight);

  cout << endl;
  typingAnimation("    ✓ Booking confirmed successfully!", 30);
  pressEnterToContinue();
}

void FlightSystem::cancelBooking() {
  clearScreen();
  displayHeader();
  cout << Color::BOLD << Color::RED << "\n    CANCELLATION PORTAL"
       << Color::RESET << endl;

  if (passengerCount == 0) {
    cout << Color::YELLOW << "\n    ⚠ No bookings to cancel." << Color::RESET
         << endl;
    pressEnterToContinue();
    return;
  }

  cout << "\n    Enter passenger name to cancel: ";
  string name;
  getline(cin, name);

  // HASH TABLE SEARCH
  Passenger *p = passengerMap.search(name);

  if (!p) {
    cout << Color::RED << "\n    ✗ Passenger not found!" << Color::RESET
         << endl;
    pressEnterToContinue();
    return;
  }

  cout << Color::CYAN << "\n    Booking found: " << p->name << " ("
       << p->seatNumber << ")" << Color::RESET << endl;
  cout << "\n    Confirm Cancellation? (Y/N): ";
  char confirm;
  cin >> confirm;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  if (toupper(confirm) == 'Y') {
    removePassenger(name);
    typingAnimation("\n    ✓ Booking cancelled.", 30);
  }

  pressEnterToContinue();
}

void FlightSystem::undoLastBooking() {
  clearScreen();
  displayHeader();
  cout << Color::BOLD << Color::YELLOW
       << "\n    UNDO LAST BOOKING (STACK OPERATION)" << Color::RESET << endl;

  if (undoStack.isEmpty()) {
    cout << Color::RED << "\n    ✗ No recent bookings to undo." << Color::RESET
         << endl;
    pressEnterToContinue();
    return;
  }

  Passenger p;
  // Pop checks top of stack
  // Ideally we peek first to show user, but let's pop and confirm logic
  // To implement "Undo", we essentially perform "cancelBooking" for the person
  // at top of stack.

  // However, since `undoStack` holds copies, we need to find the real record.
  if (undoStack.pop(p)) {
    Passenger *realRecord = passengerMap.search(p.name);
    if (realRecord) {
      cout << "\n    Undoing booking for: " << Color::CYAN << p.name
           << Color::RESET << endl;
      removePassenger(p.name);
      cout << Color::GREEN << "    ✓ Undo successful." << Color::RESET << endl;
    } else {
      cout << Color::YELLOW << "\n    ⚠ Booking for " << p.name
           << " was already cancelled manually." << Color::RESET << endl;
    }
  }
  pressEnterToContinue();
}

void FlightSystem::modifyBooking() {
  clearScreen();
  displayHeader();
  cout << Color::BOLD << Color::MAGENTA << "\n    MODIFY BOOKING"
       << Color::RESET << endl;

  if (passengerCount == 0) {
    cout << Color::YELLOW << "\n    ⚠ No bookings to modify." << Color::RESET
         << endl;
    pressEnterToContinue();
    return;
  }

  cout << "\n    Enter passenger name to modify: ";
  string name;
  getline(cin, name);

  Passenger *p = passengerMap.search(name);
  if (!p) {
    cout << Color::RED << "\n    ✗ Passenger not found." << Color::RESET
         << endl;
    pressEnterToContinue();
    return;
  }

  Flight *flight = flightList.search(p->flightId);
  if (!flight) {
    cout << Color::RED << "\n    ✗ Flight data error." << Color::RESET << endl;
    pressEnterToContinue();
    return;
  }

  cout << Color::CYAN << "\n    found passenger: " << p->name << endl;
  cout << "    Current Flight: AL-" << p->flightId << " (" << p->destination
       << ")" << endl;
  cout << "    Current Seat:   " << p->seatNumber << " (" << p->classType << ")"
       << Color::RESET << endl;

  cout << "\n    Opening Seat Map for modification..." << endl;
  this_thread::sleep_for(chrono::milliseconds(800));

  displaySeatMap(flight);

  cout << "\n    Enter NEW seat number (e.g., 1A, 5C) or [0] to cancel: ";
  string seatInput;
  getline(cin, seatInput);

  if (seatInput == "0")
    return;

  int row, col;
  if (!parseSeatNumber(seatInput, row, col)) {
    cout << Color::RED << "\n    ✗ Invalid seat format!" << Color::RESET
         << endl;
    pressEnterToContinue();
    return;
  }

  if (!isSeatAvailable(flight, row, col)) {
    cout << Color::RED << "\n    ✗ Seat " << seatInput
         << " is already occupied!" << Color::RESET << endl;
    pressEnterToContinue();
    return;
  }

  string newClass = getClassType(row);
  cout << "\n    New Class: " << newClass << ". Confirm Change? (Y/N): ";
  char confirm;
  cin >> confirm;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  if (toupper(confirm) == 'Y') {
    // 1. Clear old seat
    flight->cabin[p->row][p->col] = "";

    // 2. Update Passenger Data
    p->seatNumber = formatSeatNumber(row, col);
    p->row = row;
    p->col = col;
    p->classType = newClass;

    // 3. Occupy new seat
    flight->cabin[row][col] = p->name;

    // 4. Save
    saveBookings(&passengerList);

    cout << Color::GREEN << "\n    ✓ Booking updated successfully!"
         << Color::RESET << endl;
  } else {
    cout << "\n    Modification cancelled." << endl;
  }
  pressEnterToContinue();
}

void FlightSystem::searchPassenger() {
  clearScreen();
  displayHeader();
  cout << Color::BOLD << Color::BLUE << "\n    SEARCH PASSENGER (HASH TABLE)"
       << Color::RESET << endl;

  cout << "\n    Enter passenger name: ";
  string searchName;
  getline(cin, searchName);

  auto start = chrono::high_resolution_clock::now();
  Passenger *p = passengerMap.search(searchName);
  auto end = chrono::high_resolution_clock::now();

  if (p) {
    cout << Color::GREEN << "\n    ✓ Found in "
         << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
         << "ns!" << Color::RESET << endl;
    cout << "    Name: " << p->name << "\n    Seat: " << p->seatNumber
         << "\n    Flight ID: " << p->flightId << endl;
  } else {
    cout << Color::RED << "\n    ✗ Passenger not found." << Color::RESET
         << endl;
  }
  pressEnterToContinue();
}

void FlightSystem::displayAllPassengers() {
  clearScreen();
  displayHeader();
  cout << Color::BOLD << Color::MAGENTA << "\n    PASSENGER LIST (LINKED LIST)"
       << Color::RESET << endl;

  if (passengerList.getSize() == 0) {
    cout << Color::YELLOW << "\n    Empty List." << Color::RESET << endl;
  } else {
    cout << Color::DIM << "\n    Sorting Linked List (Merge Sort)..."
         << Color::RESET << endl;
    passengerList.sort();
    passengerList.display();
    cout << Color::CYAN << "\n    Total passengers: " << passengerList.getSize()
         << Color::RESET << endl;
  }
  pressEnterToContinue();
}

void FlightSystem::displayStatistics() {
  // Keep existing logic or iterate linked list to calculate.
  Flight *selectedFlight = nullptr;
  int page = 1;

  while (selectedFlight == nullptr) {
    displayFlightSchedule(page);
    cout << "\n    Enter command: ";
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (toupper(input[0]) == 'N')
      page++;
    else if (toupper(input[0]) == 'P')
      page--;
    else if (input == "0")
      return;
    else {
      try {
        int id = stoi(input);
        Flight *f = flightList.search(id);
        if (f)
          selectedFlight = f;
      } catch (...) {
      }
    }
  }

  // Stats logic
  int bookedEcon = countOccupiedSeats(selectedFlight, "Economy");
  int bookedFirst = countOccupiedSeats(selectedFlight, "First Class");
  int totalEcon = countAvailableSeats(selectedFlight, "Economy") + bookedEcon;
  int totalFirst =
      countAvailableSeats(selectedFlight, "First Class") + bookedFirst;

  // Basic calculation for total seats might be fixed conceptually or
  // dynamically count For now we trust counts. Waitlist count
  int waitCount = selectedFlight->waitlist.getCount();

  clearScreen();
  displayHeader();
  cout << "\n    STATISTICS FOR FLIGHT AL-" << selectedFlight->id << " ("
       << selectedFlight->destination << ")" << endl;
  cout << "    --------------------------------------------------" << endl;
  cout << "    Total Bookings: " << (bookedEcon + bookedFirst) << endl;
  cout << "    - Economy:     " << bookedEcon << " / " << totalEcon << endl;
  cout << "    - First Class: " << bookedFirst << " / " << totalFirst << endl;
  cout << "    Waitlist Size:  " << waitCount << endl;

  cout << "\n    Statistics displayed." << endl;
  pressEnterToContinue();
}

void FlightSystem::displayBoardingPass(Passenger *p) {
  // Adjusted to take pointer
  string gate = "B04";
  cout << "\n    -----------------------------------";
  cout << "\n    BOARDING PASS: " << p->name;
  cout << "\n    FLIGHT: AL-" << p->flightId;
  cout << "\n    SEAT: " << p->seatNumber << " (" << p->classType << ")";
  cout << "\n    -----------------------------------" << endl;
}

void FlightSystem::printBoardingPasses() {
  clearScreen();
  displayHeader();
  cout << "\n    Printing Boarding Passes..." << endl;

  Node *current = passengerList.getHead();
  while (current) {
    displayBoardingPass(&current->data);
    this_thread::sleep_for(chrono::milliseconds(200));
    current = current->next;
  }
  pressEnterToContinue();
}
#ifdef _WIN32
#ifndef _AMD64_
#define _AMD64_
#endif
#ifndef _M_AMD64
#define _M_AMD64
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <conio.h> // For _getch()
#endif

#define _CRT_SECURE_NO_WARNINGS
#include "Utility.h"
#include <cctype>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <thread>

using namespace std;

void setupConsole() {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE)
    return;

  DWORD dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode))
    return;

  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
  SetConsoleOutputCP(65001);
#endif
}

void clearScreen() { cout << "\033[2J\033[1;1H"; }

void pressEnterToContinue() {
  cout << Color::DIM << "\n    Press Enter to continue..." << Color::RESET;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cin.get();
}

void typingAnimation(const string &message, int delay) {
  for (char c : message) {
    cout << c << flush;
    this_thread::sleep_for(chrono::milliseconds(delay));
  }
  cout << endl;
}

void loadingBar(const string &message, int duration) {
  cout << "\n                          " << Color::CYAN << message
       << Color::RESET << endl;

  int barWidth = 30;
  int sleepTime = duration / barWidth;

  for (int i = 0; i <= barWidth; ++i) {
    cout << "\r                          " << Color::WHITE << "[";
    for (int j = 0; j < i; ++j)
      cout << "=";
    cout << Color::CYAN << "\u2708" << Color::WHITE;
    for (int j = i; j < barWidth; ++j)
      cout << " ";
    cout << "] " << setw(3) << (i * 100 / barWidth) << "%" << Color::RESET
         << flush;
    this_thread::sleep_for(chrono::milliseconds(sleepTime));
  }
  cout << endl;
  this_thread::sleep_for(chrono::milliseconds(500));
}

void displayHeader() {
  cout << Color::CYAN << Color::BOLD;
  cout << R"(
    ╔═══════════════════════════════════════════════════════════════════════════╗
    ║                                                                           ║
    ║      █████╗ ███████╗██████╗  ██████╗ ██╗      ██████╗  ██████╗ ██╗ ██████╗║
    ║     ██╔══██╗██╔════╝██╔══██╗██╔═══██╗██║     ██╔═══██╗██╔════╝ ██║██╔════╝║
    ║     ███████║█████╗  ██████╔╝██║   ██║██║     ██║   ██║██║  ███╗██║██║     ║
    ║     ██╔══██║██╔══╝  ██╔══██╗██║   ██║██║     ██║   ██║██║   ██║██║██║     ║
    ║     ██║  ██║███████╗██║  ██║╚██████╔╝███████╗╚██████╔╝╚██████╔╝██║╚██████╗║
    ║     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝ ╚═════╝║
    ║                                                                           ║
    ║                    ->  AIRLINE RESERVATION SYSTEM  <-                     ║
    ║                            TDS6213 - Project                              ║
    ╚═══════════════════════════════════════════════════════════════════════════╝
    )" << Color::RESET
       << endl;

}

void centerText(const string& text, int width) {
    int padding = (width - text.length()) / 2;
    if (padding < 0) padding = 0;
    cout << string(padding, ' ') << text << endl;
}

int displayInteractiveMenu(const string options[], int optionCount, const string& title) {
    int selected = 0;
    bool selecting = true;

    while (selecting) {
        clearScreen();
        displayHeader();
        
        cout << Color::BOLD << Color::YELLOW;
        cout << "\n    ╔═══════════════════════════════════════════════════════════════════════════╗" << endl;
        cout << "    ║";
        int pad = (75 - title.length()) / 2;
        cout << string(pad, ' ') << title << string(75 - pad - title.length(), ' ') << "║" << endl;
        cout << "    ╠═══════════════════════════════════════════════════════════════════════════╣" << endl;

        for (int i = 0; i < optionCount; ++i) {
            cout << "    ║ ";
            if (i == selected) {
                cout << Color::BG_BLUE << Color::WHITE << "  > " << left << setw(70) << options[i] << Color::RESET << Color::BOLD << Color::YELLOW << "║" << endl;
            } else {
                cout << "    " << left << setw(72) << options[i] << "║" << endl;
            }
        }
        cout << "    ╚═══════════════════════════════════════════════════════════════════════════╝" << Color::RESET << endl;
        cout << "\n    " << Color::DIM << "[UP/DOWN] Navigate  [ENTER] Select" << Color::RESET << endl;

        int key = _getch();
        if (key == 224) { // Arrow key prefix
            key = _getch();
            if (key == Key::UP) {
                selected--;
                if (selected < 0) selected = optionCount - 1;
            } else if (key == Key::DOWN) {
                selected++;
                if (selected >= optionCount) selected = 0;
            }
        } else if (key == Key::ENTER) {
            return selected; // Return index (0-based)
        }
    }
    return -1;
}

bool validateName(const string &name) {
  if (name.empty())
    return false;
  for (char c : name) {
    if (!isalpha(c) && c != ' ')
      return false;
  }
  bool hasLetter = false;
  for (char c : name) {
    if (isalpha(c)) {
      hasLetter = true;
      break;
    }
  }
  return hasLetter;
}

bool validateTripleName(const string &name) {
  if (!validateName(name))
    return false;

  int spaceCount = 0;
  bool inWord = false;
  int wordCount = 0;

  for (char c : name) {
    if (c != ' ') {
      if (!inWord) {
        wordCount++;
        inWord = true;
      }
    } else {
      inWord = false;
    }
  }
  return wordCount >= 3;
}

string toUpperCase(const string &str) {
  string result = str;
  for (char &c : result)
    c = toupper(c);
  return result;
}

string formatSeatNumber(int row, int col) {
  char colLetter = 'A' + col;
  return to_string(row + 1) + colLetter;
}

bool parseSeatNumber(const string &seatNum, int &row, int &col) {
  if (seatNum.length() < 2 || seatNum.length() > 3)
    return false;
  string rowStr = seatNum.substr(0, seatNum.length() - 1);
  char colChar = toupper(seatNum.back());
  try {
    row = stoi(rowStr) - 1;
    if (row < 0 || row >= ROWS)
      return false;
  } catch (...) {
    return false;
  }
  col = colChar - 'A';
  if (col < 0 || col >= COLS)
    return false;
  return true;
}

string getCurrentDate() {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  char buffer[30];
  strftime(buffer, 30, "%d %b %Y", ltm);
  string dateStr(buffer);
  for (char &c : dateStr)
    c = toupper(c);
  return dateStr;
}

string getCurrentTime() {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  char buffer[30];
  strftime(buffer, 30, "%I:%M %p", ltm);
  return string(buffer);
}
// ... (Existing functions)

// ============================================================================
// File I/O Implementation
// ============================================================================
#include "DataStructures.h"
#include "FlightSystem.h"
#include <fstream>
#include <sstream>
#include <vector>

void saveBookings(void* passengerListPtr) {
    if (!passengerListPtr) return;
    PassengerList* list = static_cast<PassengerList*>(passengerListPtr);

    std::ofstream outFile("bookings.txt");
    if (!outFile) {
        std::cerr << "Error: Could not open bookings.txt for writing.\n";
        return;
    }

    Node* current = list->getHead();
    while (current) {
        Passenger& p = current->data;
        // Format: Name,Seat,Row,Col,Date,Time,Origin,Dest,FlightID
        outFile << p.name << ","
                << p.seatNumber << ","
                << p.row << ","
                << p.col << ","
                << p.bookingDate << ","
                << p.bookingTime << ","
                << p.origin << ","
                << p.destination << ","
                << p.flightId << "\n";
        current = current->next;
    }
    outFile.close();
}

void loadBookings(void* flightSystemPtr) {
    if (!flightSystemPtr) return;
    FlightSystem* sys = static_cast<FlightSystem*>(flightSystemPtr);

    std::ifstream inFile("bookings.txt");
    if (!inFile) return; // No file usually means first run

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> parts;

        while (std::getline(ss, segment, ',')) {
            parts.push_back(segment);
        }

        if (parts.size() >= 9) {
            try {
                std::string name = parts[0];
                // Skip seatNumber (parts[1]) as it is derived
                int row = std::stoi(parts[2]);
                int col = std::stoi(parts[3]);
                std::string date = parts[4];
                std::string time = parts[5];
                std::string origin = parts[6];
                std::string dest = parts[7];
                int flightId = std::stoi(parts[8]);

                // Pass nullptr for Flight* - FlightSystem::addPassenger will handle looking it up
                // Pass false for saveChanges to prevent overwriting the file we are currently reading!
                sys->addPassenger(name, row, col, date, time, origin, dest, flightId, nullptr, false);
            } catch (...) {
                // Ignore malformed lines (e.g. from previous crash)
                continue;
            }
        }
    }
    inFile.close();
}

// Rewriting loadBookings to be safer after I verify FlightSystem changes.
// I will implement the Input Validation first.

int getIntInput() {
    int value;
    while (true) {
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Color::RED << "    ✗ Invalid input! Please enter a number: " << Color::RESET;
        }
    }
}

double getDoubleInput() {
    double value;
    while (true) {
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Color::RED << "    ✗ Invalid input! Please enter a number: " << Color::RESET;
        }
    }
}

std::string getLineInput() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

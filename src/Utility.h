#ifndef UTILITY_H
#define UTILITY_H

#include "Constants.h"
#include <string>

// System & UI Wrappers
void setupConsole();
void clearScreen();
void pressEnterToContinue();
void typingAnimation(const std::string &message, int delay = 20);
void loadingBar(const std::string &message, int duration = 2000);
void displayHeader();

// Interactive Menu
namespace Key {
    const int UP = 72;
    const int DOWN = 80;
    const int ENTER = 13;
    const int ESCAPE = 27;
}

int displayInteractiveMenu(const std::string options[], int optionCount, const std::string& title);
void centerText(const std::string& text, int width = 80);


// Validation & Formatting
bool validateName(const std::string &name);
bool validateTripleName(const std::string &name);
std::string toUpperCase(const std::string &str);
std::string formatSeatNumber(int row, int col);
bool parseSeatNumber(const std::string &seatNum, int &row, int &col);

// Date & Time
std::string getCurrentDate();
std::string getCurrentTime();

// File I/O
void saveBookings(void* passengerListPtr); // Using void* to avoid circular dependency in header, will cast in CPP
void loadBookings(void* flightSystemPtr);  // Using void* to call addPassenger

// Enhanced Input Validation
int getIntInput();
double getDoubleInput();
std::string getLineInput();

#endif // UTILITY_H

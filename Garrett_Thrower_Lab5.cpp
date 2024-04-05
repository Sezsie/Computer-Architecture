/*
* TITLE: Computer Architecture Lab 4
* AUTHOR: Garrett Thrower
* PURPOSE: A vending machine program that accepts certain types of coins and dispenses certain sodas, all
* while keeping track of the stock of the sodas. The program also has a technician mode that allows the user
* to see the stock of the machine. The program will loop until the machine is out of stock.
* LAST MODIFIED: 4/4/2024
*/

// IMPORTANT NOTE: In order to access the technician mode, the user must input the string "CREAMSODA4LYFE" when prompted for a soda selection. 
// This is for the sake of the assignment, and I am completely aware of how unsafe it is to have a "secret code" in the source code.

#include <iostream>
#include <cstdio>
#include <string>

// its been forever since I have programmed in C++! its nice to be back, honestly
using namespace std;

// general global variables
int moneyInputted = 0; // total amount of money the user has inputted thus far

// coin value variables
int nickelValue = 5;
int dimeValue = 10;
int quarterValue = 25;
int billValue = 100;

// validation
bool validInput = false;
char inputCurrency;
char inputSoda;

// string variables
string openingMessage = "Welcome to Mr. Zippy’s soft drink vending machine.\n";
string coinPrompt = "Please insert some currency (dollar (B), quarter (Q), dime (D), nickel (N)): ";
string sodaPrompt = "You have enough money to purchase a soda. Please make a selection: \n";
string sodaOptions = "(C) Coke, (S) Sprite, (P) Dr. Pepper, (Z) Coke Zero or (X) to cancel and return all money.";
string invalidInput = "Invalid input. Please try again.\n";
string machineEmptyMessage = "Sorry, the machine is out of stock. Please come back later.\n";
string technicianMessage = "Hello technician. The stock of the machine is as follows: \n";
string technicianCode = "CREAMSODA4LYFE";

// a struct to map the soda's character to its name, stock, and price
struct Soda{
    char sodaChar;
    string sodaName;
    int sodaStock;
};

// an array of sodas, values are the character, name, and stock respectively
Soda sodas[4] = {
    {'c', "Coke", 2},
    {'s', "Sprite", 2},
    {'p', "Dr. Pepper", 2},
    {'z', "Coke Zero", 2},
};

int sodaCost = 55; // the cost of each soda is  the same, which makes our life a helluva lot easier

// prototypes
void currencyLoop();
void sodaLoop();
void printStock(); 
bool coinInput(char coin);
// pass soda by reference so we can modify the stock of the soda
bool modifyStock(Soda& soda);
bool machineIsEmpty();  


// modify the stock of the soda if stock is not 0. returns whether the stock was modified
bool modifyStock(Soda& soda){
    if(soda.sodaStock > 0){
        // change the stock of the soda
        soda.sodaStock--;
        // tell the user the soda is being dispensed
        printf("Dispensing %s...\n", soda.sodaName.c_str());
        // return the change
        int change = moneyInputted - sodaCost;
        printf("Your change is %d cents.\n\n", change);
        moneyInputted = 0;
        return true;
    } else {
        printf("Sorry, we are out of %s.\n", soda.sodaName.c_str());
        return false;
    }
}

// print the contents of the machine's stock
void printStock(){
    printf("%s", technicianMessage.c_str());
    for(int i = 0; i < 4; i++){
        printf("%s: %d\n", sodas[i].sodaName.c_str(), sodas[i].sodaStock);
    }
    printf("\n");
}

// main loop for the soda selection
void sodaLoop(){
    bool breakLoop = false;
    string codeString;

    // empty cin buffer so we dont blow through getline
    cin.ignore();

    while(true){
        // prompting stuff
        printf("%s", sodaPrompt.c_str());
        printf("%s\n", sodaOptions.c_str());

        // get the user input as a string
        getline(cin, codeString);

        // extract the first character of the string
        inputSoda = tolower(codeString[0]);

        // first, check if the user is a technician
        if(codeString == technicianCode){
            printStock();
            codeString = "";

            // override inputSoda with x so it transitions to the next loop
            inputSoda = 'x';
        }

        // second, check if the user wants to cancel
        if(inputSoda == 'x'){
            printf("Cancelling transaction... Your money has been returned.\n");
            moneyInputted = 0;
            breakLoop = true;

        } 
        // finally, check if the user entered a valid soda character
        else if (inputSoda == 'c' || inputSoda == 's' || inputSoda == 'p' || inputSoda == 'z') {
            // find the soda that the user wants
            for(int i = 0; i < 4; i++){
                if(sodas[i].sodaChar == inputSoda){
                    // if the stock changed, set break flag to true
                    if(modifyStock(sodas[i])){
                        breakLoop = true;
                    }
                }
            }

        } 
        // if all else fails, tell the user their input was invalid
        else {
            printf("%s", invalidInput.c_str());
        }

        // jump out of the loop if the user has successfully purchased a soda
        if (breakLoop){
            break;
        }
    }
}

// returns the validity of the input and add the corresponding value of the coin to the total money inputted
bool coinInput(char coin){
    // lower the input character
    char coinLow = tolower(coin);
    // check if the input is valid
    switch(coinLow){
        case 'b':
            moneyInputted += billValue;
            return true;
        case 'q':
            moneyInputted += quarterValue;
            return true;
        case 'd':
            moneyInputted += dimeValue;
            return true;
        case 'n':
            moneyInputted += nickelValue;
            return true;
        default:
            return false;
    }
}

// check if all the sodas are out of stock. if so, return true
bool machineIsEmpty(){
    for(int i = 0; i < 4; i++){
        if(sodas[i].sodaStock > 0){
            return false;
        }
    }
    printf("%s", machineEmptyMessage.c_str());
    return true;
}

// main loop for the currency input
void currencyLoop(){
        printf("%s", openingMessage.c_str());
        while(!machineIsEmpty()){
            printf("%s", coinPrompt.c_str());
            cin >> inputCurrency;
            validInput = coinInput(inputCurrency);

            if (validInput){
                printf("You have inserted %d cents.\n", moneyInputted);
            } else {
                printf("%s\n", invalidInput.c_str());
            }

            // check if the user has enough money to purchase a soda
            if(moneyInputted >= sodaCost){
                sodaLoop();
            }
    }
}

int main(){
    currencyLoop();
    return 0;
}
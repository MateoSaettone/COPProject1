#include "linked_calc.hpp"
#include <cctype>
#include <iostream>

// Default constructor definition
template <typename T>
LinkedCalc<T>::LinkedCalc() : head(nullptr) {}

// Destructor to deallocate memory
template <typename T>
LinkedCalc<T>::~LinkedCalc() {
    Node<T>* current = head;
    while (current != nullptr) {
        Node<T>* nextNode = current->next;  // Save the next node before deleting the current one
        delete current;  // Deallocate the current node
        current = nextNode;  // Move to the next node
    }
    head = nullptr;  // Set head to nullptr after deleting all nodes
}

// Function to insert a new node at the end of the linked list
template <typename T>
void LinkedCalc<T>::insert(const T& value) {
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Helper function to determine if a character is a digit
template <typename T>
bool LinkedCalc<T>::isDigit(const T& c) {
    return (c >= '0' && c <= '9') ;
}

// Function to validate the expression
template <typename T>
bool LinkedCalc<T>::validateExpression() {
    if (head == nullptr) return false;  // Empty expression is invalid

    Node<T>* current = head;
    bool lastWasDigit = false;
    bool lastWasOperator = false;
    bool decimalInNumber = false;

    // Check if the first character is valid
    if (!isDigit(current->data) && current->data != '.') {
        return false;  // Expression cannot start with an operator
    }

    while (current != nullptr) {
        T currentData = current->data;

        if (isDigit(currentData)) {
            lastWasDigit = true;
            lastWasOperator = false;
        } else if (currentData == '.') {
            if (decimalInNumber) {
                return false;  // Invalid if multiple decimals in the current number
            }
            decimalInNumber = true;
            lastWasDigit = false;
            lastWasOperator = false;
        } else if (currentData == '+' || currentData == '-' || currentData == '*' || currentData == '/') {
            if (lastWasOperator || !lastWasDigit) {
                return false;  // Invalid if consecutive operators or operator without preceding number
            }
            lastWasOperator = true;
            lastWasDigit = false;
            decimalInNumber = false;  // Reset decimal flag as we move to a new number
        } else {
            return false;  // Invalid character
        }

        current = current->next;  // Move to the next node
    }

    return !lastWasOperator;  // Expression shouldn't end with an operator
}



// Function to evaluate the expression represented by the linked list
template <typename T>
float LinkedCalc<T>::evaluateExpression() {
    if (!validateExpression()) {
        std::cerr << "Invalid expression!" << std::endl;
        return -1;  // Return -1 to indicate an invalid expression
    }

    Node<T>* current = head;
    float result = 0.0f;
    float currentNumber = 0.0f;
    char lastOperator = '+';  // Start with addition by default
    bool processingFraction = false;  // Flag to handle decimal places
    float decimalFactor = 0.1f;  // Factor to scale digits after the decimal point

    // Traverse the list and evaluate the expression
    while (current != nullptr) {
        if (isDigit(current->data)) {
            if (processingFraction) {
                // For the fractional part, multiply the digit by the decimal factor and add it
                currentNumber += (current->data - '0') * decimalFactor;
                decimalFactor *= 0.1f;  // Move to the next decimal place (0.1, 0.01, etc.)
            } else {
                currentNumber = currentNumber * 10 + (current->data - '0');  // Build the integer part of the number
            }
        } else if (current->data == '.') {
            processingFraction = true;  // Switch to processing the fractional part
            decimalFactor = 0.1f;  // Reset the decimal factor for the fractional digits
        } else {
            // Perform the last operator's operation
            if (lastOperator == '+') {
                result += currentNumber;
            } else if (lastOperator == '-') {
                result -= currentNumber;
            } else if (lastOperator == '*') {
                result *= currentNumber;
            } else if (lastOperator == '/') {
                result /= currentNumber;
            }

            // Reset the number and operator for the next part of the expression
            currentNumber = 0;
            processingFraction = false;  // Reset for the next number
            lastOperator = current->data;  // Update the last operator
        }

        current = current->next;  // Move to the next node
    }

    // Apply the final operation after exiting the loop
    if (lastOperator == '+') {
        result += currentNumber;
    } else if (lastOperator == '-') {
        result -= currentNumber;
    } else if (lastOperator == '*') {
        result *= currentNumber;
    } else if (lastOperator == '/') {
        result /= currentNumber;
    }

    return result;
}



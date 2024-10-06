/*###########################################
#                                           #
#        Mateo Saettone Ascenzo             #
#               U56111212                   #
#                                           #
###########################################*/

#include "linked_calc.hpp"
#include <cctype>
#include <iostream>

// Default constructor definition
template <typename T>
LinkedCalc<T>::LinkedCalc() : head(nullptr) {}

// Destructor to deallocate memory
template <typename T>
LinkedCalc<T>::~LinkedCalc() {
    // Implementation of the function to deallocate memory
    Node<T>* current = head;
    while (current != nullptr) {
        Node<T>* nextNode = current->next;  // Save the next node before deleting current
        delete current;  // Deallocate current
        current = nextNode;  // Go to the next node
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
    // Initialize current to head
    Node<T>* current = head;

    // Declaration of the initial flags
    bool lastWasDigit = false;
    bool lastWasOperator = false;
    bool decimalInNumber = false;
    
    // If the list is empty, the expression is invalid
    if (head == nullptr) {
        return false; 
    }

    // If the expression starts with something other than a digit or '.' it is invalid
    if (!isDigit(current->data) && current->data != '.') {
        return false;
    }

    // Traverse through the linked list
    while (current != nullptr) {
        T currentData = current->data;

        // If the current data is a digit update flags and continue
        if (isDigit(currentData) == true) {
            lastWasDigit = true;
            lastWasOperator = false;
            current = current->next;
            continue;
        }

        // If it is the first decimal dot update flags and continue
        if (currentData == '.') {
            // If there has already been a dot then it is invalid
            if (decimalInNumber == true) {
                return false;  
            }
            // Since it is the first we update flags
            decimalInNumber = true;
            lastWasDigit = false;
            lastWasOperator = false;
            current = current->next;
            continue;
        }

        // Check that it is the first operator and is not consecutive
        if (currentData == '+' || currentData == '-' || currentData == '*' || currentData == '/') {
            // Check if there was an operator before or not a digit
            if (lastWasOperator || !lastWasDigit) {
                return false; 
            }
            // Since it is the first operator update flags
            lastWasOperator = true;
            lastWasDigit = false;
            decimalInNumber = false;
            current = current->next;
            continue;
        }

        // If none of the cases match, it's an invalid character
        return false;
    }

    // If the last character was an operator, the expression is invalid
    if (lastWasOperator == true) {
        return false;
    }

    // If not, then the expression is valid
    else {
        return true;
    }
}

// Function to evaluate the expression represented by the linked list
template <typename T>
float LinkedCalc<T>::evaluateExpression() {
    // Vectors to hold numbers and operators
    std::vector<float> numbers;
    std::vector<char> operators;

    Node<T>* current = head;

    // Parse the expression directly from the linked list
    while (current != nullptr) {
        // Parse number
        std::string numberStr;
        bool hasDecimal = false;

        // Collect digits and decimal point for the current number
        while (current != nullptr && (isDigit(current->data) || current->data == '.')) {
            numberStr += current->data;
            current = current->next;
        }

        // Convert the accumulated string to a float and add to numbers vector
        float num = std::stof(numberStr);
        numbers.push_back(num);

        // If we've reached the end of the list, break out of the loop
        if (current == nullptr) {
            break;
        }

        // Parse operator and add to operators vector
        operators.push_back(current->data);

        // Move to the next node to continue parsing
        current = current->next;
    }

    // First pass: Process multiplication and division from left to right
    size_t operatorIndex = 0;
    while (operatorIndex < operators.size()) {
        if (operators[operatorIndex] == '*' || operators[operatorIndex] == '/') {
            float num1 = numbers[operatorIndex];
            float num2 = numbers[operatorIndex + 1];
            float result;

            //Do the operation given the current operator
            if (operators[operatorIndex] == '*') {
                result = num1 * num2;
            } else {
                result = num1 / num2;
            }

            // Replace the two numbers and operator with the result
            numbers[operatorIndex] = result;
            numbers.erase(numbers.begin() + operatorIndex + 1);
            operators.erase(operators.begin() + operatorIndex);
            // We do not increment operatorIndex since the vectors have been modified
        } else {
            operatorIndex++;
        }
    }

    // Second pass: Process addition and subtraction from left to right
    operatorIndex = 0;
    while (operatorIndex < operators.size()) {
        if (operators[operatorIndex] == '+' || operators[operatorIndex] == '-') {
            float num1 = numbers[operatorIndex];
            float num2 = numbers[operatorIndex + 1];
            float result;

            // Perform the addition or substraction
            if (operators[operatorIndex] == '+') {
                result = num1 + num2;
            } else {
                result = num1 - num2;
            }

            // Replace the two numbers and operator with the result
            numbers[operatorIndex] = result;
            numbers.erase(numbers.begin() + operatorIndex + 1);
            operators.erase(operators.begin() + operatorIndex);
            // Do not increment operatorIndex since the vectors have been modified
        } else {
            operatorIndex++;
        }
    }

    // After processing, there should be one number left, which is the result
    return numbers[0];
}




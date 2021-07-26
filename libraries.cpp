// libraries.cpp 
// PHYS 30762 Programming in C++
// Luis Mestre, last updated 16/05/2021
// libraries class functions

#include"libraries.h"

// functions for user input checks

// input check for integers
int Libraries::int_input_check(std::string item)
{
    int input_int;
    std::cin >> input_int; // take input integer
    //loop while cin fails or value was negative
    while (std::cin.fail() || std::cin.peek() != '\n' || input_int < 0) {
        std::cerr << item << " is a positive number which should be part of the respective library!" << std::endl;
        std::cin.clear(); //clear failed bit
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore any characters up to end of line
        std::cin >> input_int;
    }
    std::cin.clear(); //clear cin fail bit
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore any characters up to end of line
    //return valid integer
    return input_int;
}

// function to check a char input from a list of possible char inputs
char Libraries::char_input_check(std::string char_to_check, std::string allowed_values)
{
    char input_char;
    std::string input_string;
    std::string remaning_string;
    int start_of_string;

    getline(std::cin, input_string);
    start_of_string = input_string.find_first_not_of(" "); // finding the first character
    // checking if there are characters which are not whitespaces
    if (start_of_string != std::string::npos) {
        input_char = tolower(input_string[start_of_string]);
        remaning_string = input_string.substr(start_of_string + 1);
    }
    // loop while there is a blank line, char other than allowed values and any trailing characters
    while (start_of_string == std::string::npos || allowed_values.find(input_char) == std::string::npos || remaning_string.find_first_not_of(" ") != std::string::npos) {
        std::cerr << "Please enter a character part of " << char_to_check << std::endl;
        getline(std::cin, input_string); // take a line of user input
        start_of_string = input_string.find_first_not_of(" "); // find first character
        // if there are characters other than whitespace
        if (start_of_string != std::string::npos) {
            input_char = tolower(input_string[start_of_string]);
            remaning_string = input_string.substr(start_of_string + 1);
        }
    }
    // returns the valid character
    return input_char;
}

double Libraries::double_input_check_min_max(std::string value, double min, double max)
{
    double input_double;
    std::cin >> input_double; // try and take input double
    // loop while input fails or has trailing characters or is negative/zero or is out of range
    while (std::cin.fail() || std::cin.peek() != '\n' || input_double <= 0 || input_double < min || input_double > max) {
        //check if input was out of range
        if (input_double > 0 && input_double < min || input_double > max) {
            std::cerr << value << " is invalid, please enter a number between " << min << " and " << max << "!" << std::endl;
        }
        else {
            //number was invalid
            std::cerr << value << " must be a positive number of sensible bounds!" << std::endl;
        }
        std::cin.clear(); // clear cin fail bit
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore characters in buffer up to new line
        std::cin >> input_double; // take a new input
    }
    std::cin.clear(); // clear cin fail bit
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore characters in buffer up to new line
    // return valid double
    return input_double;
}

// gets components from user through numbers
std::vector<std::shared_ptr<COMPONENTS::base_components>> Libraries::get_library_components()
{
    std::vector<std::shared_ptr<COMPONENTS::base_components>> components_vector;
    int component_number;
    // loop until user enters 0 
    while (1) {
        component_number = int_input_check("component number"); //check user interger input 
        // break loop if user enters 0
        if (component_number == 0) { break; }
        //check that the component is in the  current library 
        if (component_number > component_library_number) {
            std::cerr << "Component is not in the library! Please enter a one that is" << std::endl;
        }
        else {
            // add component pointer to vector (-1 as components are numbered 1, 2 ,3...)
            components_vector.push_back(component_library[component_number - 1]);
        }
    }
    //return a vector of pointers to the base class containing the components entered by user
    return components_vector;
}


// gets circuit number from user through numbers
int Libraries::get_library_circuit(int library_number)
{
    int library_member_number;
    library_member_number = int_input_check("circuit number"); //check user input 
    // check that the component is in the current library 
    while (library_member_number > library_number || library_member_number == 0) {
        std::cerr << "Circuit is not in the library! Please enter a one that is" << std::endl;
        library_member_number = int_input_check("circuit number");
    }
    //return the integer corresponding to the right circuit in the library
    return library_member_number;
}

// functions to create components

// creating a resistor
void Libraries::create_resistor()
{
    char ideal;
    double resistance = 0;
    double parasitic_capacitance = 0;
    double parasitic_inductance = 0;

    // asking for user input for the different paramaters
    std::cout << "Please enter i if your resistor is ideal or n if not" << std::endl;
    ideal = char_input_check("i, n", "in");
    std::cout << "Please enter the resistance in Ohms" << std::endl;
    resistance = double_input_check_min_max("Resistance", 1e-03, 1e9);
    if (ideal == 'i') {
        //construct an ideal resistor and get name of it
        component_library.emplace_back(new COMPONENTS::Resistor(resistance, 0, 0, "R" + std::to_string(resistor_count)));// construct an ideal resistor
    }
    else {
        //get parasitic values and name
        std::cout << "What is the parasitic capacitance in Farads?" << std::endl;
        parasitic_capacitance = double_input_check_min_max("Parasatic capacitance", 0, 1e-9);
        std::cout << "What is the parasitic inductance in Henries?" << std::endl;
        parasitic_inductance = double_input_check_min_max("Parasatic inductance", 0, 1e-9);
        //construct non ideal resistor
        component_library.emplace_back(new COMPONENTS::Resistor(resistance, parasitic_capacitance, parasitic_inductance, "R" + std::to_string(resistor_count))); // construct a non ideal resistor
    }
    ++resistor_count;
    ++component_library_number; // adds 1 component to the library number
}

// creating a capacitor
void Libraries::create_capacitor()
{
    char ideal;
    double capacitance = 0;
    double parasitic_resistance = 0;
    double parasitic_inductance = 0;
    // getting user parameters for capacitor
    std::cout << "Please enter i if your capacitor is ideal or n if not" << std::endl;
    ideal = char_input_check("i, n", "in");
    std::cout << "Please enter the capacitance in Farads " << std::endl;
    capacitance = double_input_check_min_max("Capacitance", 1e-15, 100);
    // checking if component is ideal from user input
    if (ideal == 'i') {
        //construct an ideal capacitor
        component_library.emplace_back(new COMPONENTS::Capacitor(capacitance, 0, 0, "C" + std::to_string(capacitor_count))); // construct ideal capacitor
    }
    else {
        //get parasitic values
        std::cout << "What is the parasitic resistance in Ohms?" << std::endl;
        parasitic_resistance = double_input_check_min_max("Parasatic resistance", 0, 1);
        std::cout << "What is the parasitic inductance in Henries?" << std::endl;
        parasitic_inductance = double_input_check_min_max("Parasatic inductance", 0, 1e-6);
        component_library.emplace_back(new COMPONENTS::Capacitor(capacitance, parasitic_resistance, parasitic_inductance, "C" + std::to_string(capacitor_count)));// construct non ideal capacitor
    }

    ++capacitor_count;
    ++component_library_number; // adds 1 component to the library number
}

// creating an inductor
void Libraries::create_inductor()
{
    char ideal;
    double inductance = 0;
    double parasitic_resistance = 0;
    double parasitic_capacitance = 0;
    std::string name;
    std::cout << "Please enter i if your inductor is ideal or n if not" << std::endl;
    ideal = char_input_check("i, n", "in");
    std::cout << "Please enter the inductance in Henries" << std::endl;
    inductance = double_input_check_min_max("Inductance", 1e-15, 10);
    // checking if inductor is ideal or not
    if (ideal == 'i') {
        component_library.emplace_back(new COMPONENTS::Inductor(inductance, 0, 0, "L" + std::to_string(inductor_count)));// construct an ideal Inductor
    }
    else {
        //get parasitic values
        std::cout << "What is the parasitic resistance in Ohms?" << std::endl;
        parasitic_resistance = double_input_check_min_max("Parasatic resistance", 0, 1000);
        std::cout << "What is the parasitic capacitance in Farads?" << std::endl;
        parasitic_capacitance = double_input_check_min_max("Parasatic capacitance", 0, 1e-9);
        component_library.emplace_back(new COMPONENTS::Inductor(inductance, parasitic_resistance, parasitic_capacitance, "L" + std::to_string(inductor_count)));// construct non ideal inductor
    }
    ++inductor_count;
    ++component_library_number; // adds 1 component to the library number
}

// main_code.cpp 
// PHYS 30762 Programming in C++
// Luis Mestre, last updated 16/05/2021
// libraries class

#ifndef LIBRARIES_H
#define LIBRARIES_H

#include"components.h"
#include"circuits.h"


class Libraries
{
// public to be able to change these in the main program without a bunch of getters and setters makes code more readable
public: 
    // variables to count number of members in each library
    std::size_t component_library_number = 0; // number of components in component library
    std::size_t circuit_library_number = 0; // number of circuits in circuit library
    std::size_t nested_circuit_library_number = 0; // number of nested circuits in nested circuit library
    // counters for component numbers (used for component names deuring creation)
    std::size_t resistor_count = 1;
    std::size_t capacitor_count = 1;
    std::size_t inductor_count = 1;
    // library vectors
    std::vector<std::shared_ptr<COMPONENTS::base_components>> component_library; // library of components
    std::vector<std::shared_ptr<CIRCUITS::circuit>> circuit_library; // library of circuits 
    std::vector<std::shared_ptr<CIRCUITS::nested_circuit>> nested_circuit_library; // library of nested circuits

public:
    Libraries() {};
    ~Libraries() {};
    // methods
    int int_input_check(std::string);
    char char_input_check(std::string char_to_check, std::string allowed_values);
    double double_input_check_min_max(std::string value, double min, double max);
    template<typename number, typename vector>
    std::string repeated_name_check(number library_number, vector library_vector);
    std::vector<std::shared_ptr<COMPONENTS::base_components>> get_library_components();
    template <typename vector>
    void print_library(vector library, int library_number, std::string library_name);
    int get_library_circuit(int library_number);
    void create_resistor();
    void create_capacitor();
    void create_inductor();
};




// library template functions

// template function to check for repeated names in circuit Libraries
template<typename number, typename vector>
std::string Libraries::repeated_name_check(number library_number, vector library_vector)
{
    std::string name;
    std::string repeated_name_check;
    std::string temp_name;
    bool while_loop_control = true;

    // while the entered name matches output error message and ask for name again
    while (while_loop_control == true) {
        // getting name input from user
        std::cin.clear(); //clear clearing input buffer stream before taking new input
        getline(std::cin, name);

        if (library_number == 0) {
            break;
        } // if the library is empty don't compare names 

        // loop through the library and compare entered name to the names of components int the library
        for (auto iterator{ library_vector.begin() }; iterator != library_vector.end(); iterator++) {
            repeated_name_check = (*iterator)->get_name();

            while (name == repeated_name_check || name == temp_name) {
                std::cerr << "That name already exists, please enter a different name" << std::endl;
                std::cin.clear(); // clear clearing input buffer stream before taking new input
                getline(std::cin, name);
                // looping through again to check with all previous entries again
                for (auto iterator{ library_vector.begin() }; iterator != library_vector.end(); iterator++) {
                    temp_name = (*iterator)->get_name();
                    if (name == temp_name) { break; } // breaks for loop that loops through library again
                }
            }
        }
        while_loop_control = false; // stoping while loop
    }

    return name;
}

// prints one of the 3 Libraries library for user to see
template <typename vector>
void Libraries::print_library(vector library, int library_number, std::string library_name)
{
    std::cout << "Your current " << library_name << " is:" << std::endl;
    std::cout << '\n';
    int count_1 = 1; // counter for circuit number in library terminal output
    if (library_number != 0) {
        for (auto iterator{ library.begin() }; iterator != library.end(); iterator++) {
            std::cout << count_1 << ")";
            (*iterator)->basic_info(); // outputting only the basic information about the circuit without the components
            std::cout << '\n';
            ++count_1;
        }
    }
    else {
        std::cout << "Sorry, this library is empty, please type 0 to go back to menu to perform other functions" << std::endl;
    }
}


#endif
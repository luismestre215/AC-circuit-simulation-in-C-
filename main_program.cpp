// main_program.cpp 
// PHYS 30762 Programming in C++
// Luis Mestre, last updated 16/05/2021
// circuits project main program

// header file with classes for components, circuit and libraries class
#include"components.h"
#include"libraries.h"
#include"circuits.h"

#include<stdlib.h> // used for clearing terminal only

// library object for main programm
static Libraries libraries;


// just having a break before the terminal resets to show any messages
void terminal_reset()
{
    std::string terminal_reset;
    std::cout << "Type enter to go back to the menu and do something else" << std::endl;
    getline(std::cin, terminal_reset);
    std::cin.clear();
}

// functions for user interface 

// user interface function to show user the details about a circuit in the library (works for nested and normal circuits)
template <typename vector>
void see_specific_circuit_in_library(vector library, int library_number, std::string library_name)
{
    if (library_number == 0) {
        std::cerr << "This library is empty, please add something first" << std::endl;
    }
    else {
        libraries.print_library(library, library_number, library_name); // see the library 
        // pick circuit to see 
        std::cout << "Type the number of the circuit in the library that you want to see the full information for even if it has no components" << std::endl;
        library[libraries.get_library_circuit(library_number) - 1]->info();
    }
    terminal_reset();
}

// fucntion to add a component to the library
void create_component()
{
    std::cout << "Please enter r, c and l to create a resistor, a capacitor or an inductor respecively" << std::endl;
    char component_letter = libraries.char_input_check("r, l, c", "rlc");
    try {
        switch (component_letter) {

        case 'r':
            libraries.create_resistor();
            break;
        case 'c':
            libraries.create_capacitor();
            break;
        case 'l':
            libraries.create_inductor();
            break;
        }
    }
    catch (const std::bad_alloc&) {
        throw "Memory leak";
    }

    std::cout << "Component created and added to the library " << std::endl;
    terminal_reset();
}

// function to create a circuit and add to the circtui library 
void create_circuit()
{
    // set up circuit power supply
    double frequency = 0;
    double voltage = 0;
    std::string name;

    // asking user for circuit parameters
    std::cout << "What voltage is the power supply in Volts?" << std::endl;
    voltage = libraries.double_input_check_min_max("Voltage", 1e-03, 100e3);
    std::cout << "What frequency is the power supply in Hertz?" << std::endl;
    frequency = libraries.double_input_check_min_max("Frequency", 0.001, 1e10);
    std::cout << "What would you like to name your circuit?" << std::endl;
    name = libraries.repeated_name_check(libraries.circuit_library_number, libraries.circuit_library);
    // add new circuit to the circuit library
    try {
        libraries.circuit_library.emplace_back(new CIRCUITS::circuit(voltage, frequency, name));
    }
    catch (const std::bad_alloc&) {
        throw "Memory leak";
    }
    std::cout << "Your circuit had been added to the circuit library" << std::endl;
    ++libraries.circuit_library_number; // increased the numbers of circuits in the library by 1
    terminal_reset();
}

// function to add components from library in series
int add_component_series()
{
    // checking if there are any components in the library 
    if (libraries.component_library_number == 0) {
        std::cerr << "Your component library is empty please create a component first" << std::endl;

        terminal_reset();
        return 0; // to exit function
    }
    // checking if there are any circuits in the library 
    if (libraries.circuit_library_number == 0) {
        std::cerr << "Circuit library is empty, please create a circuit first" << std::endl;

        terminal_reset();
        return 0; // to exit function
    }
    // getting the circuit the user wants to add components in series to
    libraries.print_library(libraries.circuit_library, libraries.circuit_library_number, "circuit library");
    std::cout << "Which circuit would you like to add a components in series to?" << std::endl;
    std::cout << "Please enter the number of the circuit you want to add components to" << std::endl;
    int circuit_number;
    circuit_number = libraries.int_input_check("circuit number"); //check user input
    // check that the component is in the current library  
    while (circuit_number > libraries.circuit_library_number || circuit_number == 0) {
        std::cerr << "Circuit is not in the library! Please enter a circuit that is" << std::endl;
        circuit_number = libraries.int_input_check("circuit number"); //check user input
    }
    // adding component to user defined circuit 
    libraries.print_library(libraries.component_library, libraries.component_library_number, "component library");
    std::cout << "Please enter the number of the component you want to add in series from the library of components then press enter" << std::endl;
    std::cout << "Keep going to add more components. Once you are done adding components enter 0 to stop and go back to menu" << std::endl;
    // call impedance series sum function from the circuit class pointer
    libraries.circuit_library[circuit_number - 1]->impedance_series_sum(libraries.get_library_components());
    std::cout << "components added in series to " << libraries.circuit_library[circuit_number - 1]->get_name() << std::endl;
    std::cout << '\n';

    terminal_reset();
    return 0; // function must return something
}

// function to add components from library in parallel
int add_component_parallel()
{
    // checking if there are any components in the library
    if (libraries.component_library_number == 0) {
        std::cerr << "Sorry, there are no components in the component library " << std::endl;
        terminal_reset();
        return 0; // to exit function     
    }
    // checking if the circuit library is empty
    if (libraries.circuit_library_number == 0) {
        std::cerr << "Circuit library is empty, please create a circuit first" << std::endl;
        terminal_reset();
        return 0; // to exit function
    }
    // taking in user input to pick circuit the components will be added to
    libraries.print_library(libraries.circuit_library, libraries.circuit_library_number, " circuit library");
    std::cout << "Which circuit would you like to add a components in parallel to?" << std::endl;
    std::cout << "Please enter the number of the circuit you want to add components to" << std::endl;
    int circuit_number;
    circuit_number = libraries.int_input_check("circuit number"); //check user input
    // check that the component is in the current library  
    while (circuit_number > libraries.circuit_library_number || circuit_number == 0) {
        std::cerr << "Circuit is not in the library! Please enter a circuit that is" << std::endl;
        circuit_number = libraries.int_input_check("circuit number"); //check user input
    }

    // checking if there are any components in series in the circuit first
    if (libraries.circuit_library[circuit_number - 1]->get_size() == 0) {
        std::cerr << "Please add some components to this circuit in series first" << std::endl;
        terminal_reset();
        return 0; // return statement to exit function after if completed
    }

    // adding component to user defined circuit 
    libraries.print_library(libraries.component_library, libraries.component_library_number, "component library");
    std::cout << "Please enter the number of the component you want to add in series from the library of components then press enter" << std::endl;
    std::cout << "Keep going to add more components. Once you are done adding components enter 0 to stop and go back to menu" << std::endl;
    // call impedance parallel sum function from the circuit class pointer
    libraries.circuit_library[circuit_number - 1]->impedance_parallel_sum(libraries.get_library_components());
    std::cout << "components added in parallel to " << libraries.circuit_library[circuit_number - 1]->get_name() << std::endl;
    std::cout << '\n';

    terminal_reset();
    return 0; // function must return something
}

// creating a nested circuit
void create_nested_circuit()
{
    //set up circuit power supply
    double frequency = 0;
    double voltage = 0;
    std::string name;
    // asking user for circuit parameters
    std::cout << "What voltage is the power supply in Volts?" << std::endl;
    voltage = libraries.double_input_check_min_max("Voltage", 1e-03, 1e05);
    std::cout << "What frequency is the power supply in Hertz?" << std::endl;
    frequency = libraries.double_input_check_min_max("Frequency", 1e-3, 1e10);
    std::cout << "What would you like to name your nested circuit?" << std::endl;
    name = libraries.repeated_name_check(libraries.nested_circuit_library_number, libraries.nested_circuit_library);
    // add new circuit to the circuit library
    libraries.nested_circuit_library.emplace_back(new CIRCUITS::nested_circuit(voltage, frequency, name));
    std::cout << "Your nested circuit had been added to the circuit library" << std::endl;
    ++libraries.nested_circuit_library_number; // increased the numbers of circuits in the library by 1
    terminal_reset();
}

// gets components from user through numbers
std::vector<std::shared_ptr<CIRCUITS::circuit>> get_library_circuits_nesting()
{
    std::vector<std::shared_ptr<CIRCUITS::circuit>> circuits_vector;
    int circuit_number;
    // loop until user enters 0 
    while (1) {
        circuit_number = libraries.int_input_check("circuit number"); //check user interger input 
        // break loop if user enters 0
        if (circuit_number == 0) { break; }
        //check that the component is in the  current library 
        if (circuit_number > libraries.circuit_library_number) {
            std::cerr << "Circuit is not in the library! Please enter a circuit that is" << std::endl;
        }
        else {
            // add component pointer to vector (-1 as components are numbered 1, 2 ,3...)
            circuits_vector.push_back(libraries.circuit_library[circuit_number - 1]);
        }
    }
    //return a vector of pointers to the base class containing the components entered by user
    return circuits_vector;
}


// adding a circuit in series to a ensted circuit
int add_circuit_in_series_nested()
{
    std::vector<std::shared_ptr<CIRCUITS::circuit>> circuits_to_add_series; // vector to store circuits to add in series to nested circuit
    int nested_circuit_number = 0;
    double frequency_nested_circuit = 0;
    double voltage_nested_circuit = 0;
    // if the nested circuit library is empty don't let user add circuits and go back to menu
    if (libraries.nested_circuit_library_number == 0) {
        std::cerr << "Your nested circuit library is empty, please create a normal unested circuit first" << std::endl;
        terminal_reset();
        return 0; // to exit function
    }
    // checking if there are any circuits in the library 
    if (libraries.circuit_library_number == 0) {
        std::cerr << "Circuit library is empty, please create a circuit first" << std::endl;
        terminal_reset();
        return 0; // to exit function
    }

    // getting the circuit the user wants to add components in series to
    libraries.print_library(libraries.nested_circuit_library, libraries.nested_circuit_library_number, "nested circuit library"); // prints nested circuit library
    std::cout << "Which nested circuit would you like to add a circuit configuration in series to?" << std::endl;
    std::cout << "Please enter the number of the nested circuit you want to add the circuit configuration to" << std::endl;

    // getting number of nested circuit from the library to add other circuits to
    nested_circuit_number = libraries.int_input_check("circuit number"); //check user input
    // check that the circuit is in the current library  
    while (nested_circuit_number > libraries.nested_circuit_library_number || nested_circuit_number == 0) {
        std::cerr << "Circuit is not in the library! Please enter a circuit that is" << std::endl;
        nested_circuit_number = libraries.int_input_check("circuit number"); //check user input
    }


    libraries.print_library(libraries.circuit_library, libraries.circuit_library_number, "circuit library");// prints out circuit library
    std::cout << "Please enter the number of the circuit you want to add in series from the library of circuits then press enter" << std::endl;
    std::cout << "Please make sure the circuits you are adding have the same frequency and voltage as the nested circuit" << std::endl;
    std::cout << "Keep going to add more circuits. Once you are done adding circuits enter 0 to stop and go back to menu" << std::endl;

    frequency_nested_circuit = libraries.nested_circuit_library[nested_circuit_number - 1]->get_frequency(); // gets frequency of nested circuit
    voltage_nested_circuit = libraries.nested_circuit_library[nested_circuit_number - 1]->get_voltage(); // gets voltage of nested circuit
    circuits_to_add_series = get_library_circuits_nesting(); // gets circuits to add in series 

    for (auto iterator{ circuits_to_add_series.begin() }; iterator != circuits_to_add_series.end(); iterator++) {
        if ((*iterator)->get_size() == 0) {
            std::cerr << "Please do not add empty circuits to the nested circuit, go back to menu and add components to the empty circuits" << std::endl;
            terminal_reset();
            return 0;
        }

    }

    // checks the circuits user is adding in series have the same frequency and voltage as the nested circuit
    for (auto iterator{ circuits_to_add_series.begin() }; iterator != circuits_to_add_series.end(); iterator++) {
        double frequency_circuit_to_add = 0; // to store the frequency of circuit that will be added
        double voltage_circuit_to_add = 0; // to store the voltage of the circuit that will be added

        frequency_circuit_to_add = (*iterator)->get_frequency();
        voltage_circuit_to_add = (*iterator)->get_voltage();

        // if the voltage or frequency are different for any of the circuits then exit function and ask user to go back to menu to add circuits
        if (frequency_circuit_to_add != frequency_nested_circuit || voltage_circuit_to_add != voltage_nested_circuit) {
            std::cerr << "The circuits were not added, please go back to menu and add circuits that have the same frequency and voltage as the nested circuit" << std::endl;
            terminal_reset();
            return 0;
        }
    }
    // summing the circuits in series with the nested class function for series sum
    libraries.nested_circuit_library[nested_circuit_number - 1]->nested_impedance_series_sum(circuits_to_add_series);
    std::cout << "Circuit configurations were added in series to " << libraries.nested_circuit_library[nested_circuit_number - 1]->get_name() << std::endl;
    std::cout << '\n';

    terminal_reset();
    return 0; // function must return something
}

// adding a circuit in parallel
int add_circuit_in_parallel_nested()
{
    std::vector<std::shared_ptr<CIRCUITS::circuit>> circuits_to_add_parallel; // vector to store circuits to add in parallel to nested circuit
    int nested_circuit_number = 0;
    double frequency_nested_circuit = 0;
    double voltage_nested_circuit = 0;

    // checking if the nested circuit library is empty
    if (libraries.nested_circuit_library_number == 0) {
        std::cerr << "Your nested circuit library is empty, please create a circuit first" << std::endl;
        terminal_reset();
        return 0; // to exit function
    }
    // checking if circuit library is empty
    if (libraries.circuit_library_number == 0) {
        std::cerr << "Your circuit library is empty, please create a normal unested circuit first" << std::endl;
        terminal_reset();
        return 0; // to exit function
    }


    // taking in user input to pick circuit the components will be added to
    libraries.print_library(libraries.nested_circuit_library, libraries.nested_circuit_library_number, "nested circuit library");
    std::cout << "Which circuit would you like to add a circuit configuration in parallel to?" << std::endl;
    std::cout << "Please enter the number of the nested circuit you want to add the circuit configuration to" << std::endl;

    // getting number of nested circuit from the library to add other circuits to
    nested_circuit_number = libraries.int_input_check("circuit number"); //check user input
    // check that the circuit is in the current library  
    while (nested_circuit_number > libraries.nested_circuit_library_number || nested_circuit_number == 0) {
        std::cerr << "Nested circuit is not in the library! Please enter a nested circuit that is" << std::endl;
        nested_circuit_number = libraries.int_input_check("circuit number"); //check user input
    }

    // checking if there are any components in series in the circuit first
    if (libraries.nested_circuit_library[nested_circuit_number - 1]->get_size() == 0) {
        std::cerr << "Please add some circuit configurations to this circuit in series first" << std::endl;
        terminal_reset();
        return 0; // return statement to exit function after if completed
    }

    // adding component to user defined circuit 
    libraries.print_library(libraries.circuit_library, libraries.circuit_library_number, "circuit library");
    std::cout << "Please enter the number of the circuit you want to add in series from the library of circuits then press enter" << std::endl;
    std::cout << "Keep going to add more circuits. Once you are done adding circuits enter 0 to stop and go back to menu" << std::endl;

    frequency_nested_circuit = libraries.nested_circuit_library[nested_circuit_number - 1]->get_frequency(); // gets frequency of nested circuit
    voltage_nested_circuit = libraries.nested_circuit_library[nested_circuit_number - 1]->get_voltage(); // gets voltage of nested circuit
    circuits_to_add_parallel = get_library_circuits_nesting(); // gets circuits to add in parallel

    // checks the circuits user is adding in parallel have the same frequency and voltage as the nested circuit
    for (auto iterator{ circuits_to_add_parallel.begin() }; iterator != circuits_to_add_parallel.end(); iterator++) {
        double frequency_circuit_to_add = 0; // to store the frequency of circuit that will be added
        double voltage_circuit_to_add = 0; // to store the voltage of the circuit that will be added

        frequency_circuit_to_add = (*iterator)->get_frequency();
        voltage_circuit_to_add = (*iterator)->get_voltage();

        // if the voltage or frequency are different for any of the circuits then exit function and ask user to go back to menu to add circuits
        if (frequency_circuit_to_add != frequency_nested_circuit || voltage_circuit_to_add != voltage_nested_circuit) {
            std::cerr << "The circuits were not added, please go back to menu and add circuits that have the same frequency and voltage as the nested circuit" << std::endl;
            terminal_reset();
            return 0;
        }
    }

    // call impedance parallel sum function from the circuit class pointer
    libraries.nested_circuit_library[nested_circuit_number - 1]->nested_impedance_parallel_sum(circuits_to_add_parallel);
    std::cout << "Circuit configurations were added in parallel to " << libraries.nested_circuit_library[nested_circuit_number - 1]->get_name() << std::endl;
    std::cout << '\n';

    terminal_reset();
    return 0; // function must return something
}

void clear_libraries()
{
    // clearing library vectors
    libraries.component_library.clear();
    libraries.circuit_library.clear();
    libraries.nested_circuit_library.clear();

    // resetting counter variables
    libraries.component_library_number = 0; // number of components in component library
    libraries.circuit_library_number = 0; // number of circuits in circuit library
    libraries.nested_circuit_library_number = 0; // number of nested circuits in nested circuit library
    // counters for component numbers (used for component names during creation)
    libraries.resistor_count = 1;
    libraries.capacitor_count = 1;
    libraries.inductor_count = 1;

    std::cout << "All libraries have been cleared" << std::endl;
    terminal_reset();
}

// function to allow user to create a demo circuit
void create_demo_circuit()
{
    // create demmo circuit
    try {
        libraries.circuit_library.emplace_back(new CIRCUITS::circuit(90, 60, "demo"));
        ++libraries.circuit_library_number;
        // push components into component library
        for (int i = 0; i < 2; i++) {
            libraries.component_library.emplace_back(new COMPONENTS::Resistor(90, 0, 0, "R" + std::to_string(libraries.resistor_count)));
            libraries.component_library.emplace_back(new COMPONENTS::Capacitor(1e-5, 0, 0, "C" + std::to_string(libraries.capacitor_count)));
            libraries.component_library.emplace_back(new COMPONENTS::Inductor(2e-3, 0, 0, "L" + std::to_string(libraries.capacitor_count)));
            ++libraries.resistor_count;
            ++libraries.capacitor_count;
            ++libraries.inductor_count;
        }
        // update component library number
        libraries.component_library_number += 6;
        // add components to demo circuit
        libraries.circuit_library[libraries.circuit_library_number - 1]->impedance_series_sum(libraries.component_library);
        libraries.circuit_library[libraries.circuit_library_number - 1]->impedance_parallel_sum(libraries.component_library);
        libraries.circuit_library[libraries.circuit_library_number - 1]->impedance_parallel_sum(libraries.component_library);

        std::cout << "Your Demo circuit was created, to view it, please go back to the menu and enter 5 to view the circuit library, when there select the demo circuit number" << std::endl;
        terminal_reset();
    }
    catch (std::bad_alloc) {
        throw"bad memory allocation when creating demo circuit";
    }

}



// User interface
void user_interface()
{
    char exit_char = ' ';
    bool demo_bool = false;
    // keep running user interface until user decides to exit 
    while (exit_char != 'e') {

        std::cout << "Please pick an option from the list of actions that you would like to do:" << std::endl;
        if (demo_bool == false) {
            std::cout << '\n';
            std::cout << "  Type d to create a demo circuit add it to the circuit library" << std::endl;
        }

        std::cout << '\n';
        std::cout << "  Type 1 to create a new circuit add it to the circuit library" << std::endl;
        std::cout << "  Type 2 to create a new component and add it to the component library" << std::endl;
        std::cout << "  Type 3 to add a component in parallel to a circuit from the library" << std::endl;
        std::cout << "  Type 4 to add a component in series to a circuit from the library" << std::endl;
        std::cout << "  Type 5 to see the information about a circuit from the circuit library" << std::endl;
        std::cout << '\n';
        std::cout << '\n';
        std::cout << " Nesting circuit options:  " << std::endl;
        std::cout << '\n';
        std::cout << "  Type 6 to create a new nested circuit and add it to the nested circuit library " << std::endl;
        std::cout << "  Type 7 to add the component configuration of a circuit in parallel to a nested circuit " << std::endl;
        std::cout << "  Type 8 to add the component configuration of a circuit in series to a nested circuit " << std::endl;
        std::cout << "  Type 9 to see the information about a nested circuit from the nested circuit library" << std::endl;
        std::cout << '\n';
        std::cout << "Type c to clear all libraries" << std::endl;
        std::cout << "Type e if you want to exit the program" << std::endl;

        char input_char = libraries.char_input_check("1, 2, 3, 4, 5, 6, 7, 8, 9, c, e, d", "123456789ced");

        switch (input_char) {
            // creates a demo circuit and adds it to the circuit library but only once by checking the state of demo_bool
        case 'd':
            system("clear");
            if (demo_bool == false) {// allows demo to run for first time the menu is shown
                create_demo_circuit(); // allows user to create a new demo circuit and add it to the circuit library
                demo_bool = true;
                break;
            }
            else {
                std::string warning = (input_char == 'd') ? "Demo has already been created, please do something else" : "";
                std::cout << warning << std::endl;
                terminal_reset();
                break;
            }
            // creates a circuit and adds it to the component library
        case '1':
            system("clear");
            create_circuit(); // allows user to create a new circuit and add to the circuit library
            break;
            // creates a component and adds it to the component library
        case '2':
            system("clear");
            create_component(); // adds components to the library
            break;
            // adding a component in parallel to a circuit form the circuit library
        case '3':
            system("clear");
            add_component_parallel(); // adds components in parallel to the current circuit
            break;
            // adding a component in series to a circuit form the circuit library
        case '4':
            system("clear");
            add_component_series(); // adds components in series to the current circuit
            break;
            // seeing info about a sepecific circuit in the circuit library
        case '5':
            system("clear");
            see_specific_circuit_in_library(libraries.circuit_library, libraries.circuit_library_number, "circuit library");
            break;
            // creating a nested circuit with circuits from the circuit library
        case '6':
            system("clear");
            create_nested_circuit();
            break;
            // adding a circuit component configuration to a nested circuit in parallel
        case '7':
            system("clear");
            add_circuit_in_parallel_nested();
            break;
            // adding a circuit component configuration to a nested circuit in series
        case '8':
            system("clear");
            add_circuit_in_series_nested();
            break;
            // seeing info about a circuit in the nested circuit library
        case '9':
            system("clear");
            see_specific_circuit_in_library(libraries.nested_circuit_library, libraries.nested_circuit_library_number, "nested circuit library");
            break;
        case 'c':
            system("clear");
            clear_libraries();
            demo_bool = false; // resetting demo cool to allow used to re-run demo after libraries are clear
            break;
            // exiting the program
        case 'e':
            system("clear");
            exit(0); //exit the program without an error code
        }

        system("clear"); // clears terminal after each run of the main while loop 

    } //  end of while loop

} // end of user interface function



int main()
{
    std::cout.precision(3);
    user_interface(); // run the user interface function
    terminal_reset();
    system("clear"); // clearing terminal after program is done running

    return 0;
}




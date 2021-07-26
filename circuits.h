// circuits.h
// PHYS 30762 Programming in C++
// Luis Mestre, last updated 16/05/2021
// circuit and nested circuit class

#ifndef CIRCUITS_H
#define CIRCUITS_H

#include "components.h"


namespace CIRCUITS {

    // circuit class
    class circuit
    {
    protected:
        std::vector<std::shared_ptr<COMPONENTS::base_components>> circuit_components_vector;
        std::complex<double> impedance = 0;
        double voltage = 0;
        double frequency_Hz = 0;
        double current = 0;
        std::string circuit_text;
        std::string name = "None";

    public:
        circuit();
        circuit(double voltage, double frequency, std::string name)
            : voltage{ voltage }, frequency_Hz{ frequency }, name{ name } {};
        circuit(std::shared_ptr<circuit> circuit_ptr);
        ~circuit() {};
        // getters
        std::complex<double> get_impedance() const;
        std::string get_name() const;
        double get_voltage() const;
        double get_frequency() const;
        std::string get_circuit_text() const;
        int get_size() const;
        // other methods
        void impedance_series_sum(std::vector<std::shared_ptr<COMPONENTS::base_components>>);
        void impedance_parallel_sum(std::vector<std::shared_ptr<COMPONENTS::base_components>>);
        std::string calculate_current();
        void info();
        void basic_info();
        void print_table_row(std::string, std::string, std::string,
        double, std::string, double, std::complex<double>, double, bool);
        template<typename vector>
        void none_repeated_info(vector);
        template<typename vector>
        int repeated_name_counter(vector, std::string);
        void print_circuit_diagram() const;
        void table_info();
    }; // end of circuit class






    // nested circuit class
    class nested_circuit : public circuit
    {
    private:
        std::vector<std::shared_ptr<circuit>> vector_of_circuits; // stores circuits that make up nested cirucit
        std::vector<int> vector_of_circuit_sizes; // stores number of components of each circuit in the nested circuit
        std::vector<int> vector_of_line_sizes; // stores how many circuits are in each parallel line
        int parallel_line_number = 0;
    public:
        nested_circuit() {};
        nested_circuit(double voltage, double frequency, std::string name_parameter)
            : circuit(voltage, frequency, name_parameter) {};

        ~nested_circuit() {};

        // methods specific to this class
        void nested_impedance_series_sum(std::vector<std::shared_ptr<circuit>>);
        void nested_impedance_parallel_sum(std::vector<std::shared_ptr<circuit>>);
        void info();
        void circuit_change_check();

        // getters
        int get_size() const;
    }; // end of nested circuit class


} // end of namespace for circuits


// function to check for repeated names in the vector of circuits 
template<typename vector>
int CIRCUITS::circuit::repeated_name_counter(vector vector_of_names, std::string name_to_check)
{
    int counter = 0; // counts number of times a name is repeated in the library to get component number

    // loop through the library and compare entered name to the names of components int the library
    for (auto iterator{ vector_of_names.begin() }; iterator != vector_of_names.end(); iterator++) {
        if (name_to_check == (*iterator)) {
            ++counter; // increases counter 
        }
    }

    return counter; // returns false if there are no matching names
}



// prints table rows for components and circuits but only once per component or circuit 
template<typename vector>
void CIRCUITS::circuit::none_repeated_info(vector vector_of_items)
{
    int counter = 0;
    std::vector<std::string> names_already_printed;
    std::vector<std::string> names_vector;
    // put all the circuit names into a vector for comparison later
    for (auto iterator{ vector_of_items.begin() }; iterator != vector_of_items.end(); iterator++) {
        names_vector.push_back((*iterator)->get_name());
    }
    // print out component characteristics
    for (auto iterator{ vector_of_items.begin() }; iterator != vector_of_items.end(); iterator++) {

        // name of component in vector
        std::string component_to_print_name = (*iterator)->get_name();
        // number of a specific type of component
        int number_of_components = repeated_name_counter(names_vector, component_to_print_name);

        // print out first element in the vector
        if (counter == 0) {
            (*iterator)->table_info();
            std::cout << number_of_components << std::endl;
            names_already_printed.push_back(component_to_print_name);
        }

        // number of times the name was already printed
        int number_of_times_repeated = repeated_name_counter(names_already_printed, component_to_print_name);
        // only print out info if the repeated name check function returns 0 of this name has already been printed
        if (number_of_times_repeated == 0) {
            (*iterator)->table_info();
            std::cout << number_of_components << std::endl;
            names_already_printed.push_back(component_to_print_name);
        }
        ++counter;
    } // end of printing components

}




#endif

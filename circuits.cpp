// circuits.cpp 
// PHYS 30762 Programming in C++
// Luis Mestre, last updated 16/05/2021
// circuit and nested circuit class functions

#include"circuits.h"

// Methods for circuit class

// impedance sum series
void CIRCUITS::circuit::impedance_series_sum(std::vector<std::shared_ptr<COMPONENTS::base_components>> components_series)
{
    std::string temp_string;

    for (auto iterator{ components_series.begin() }; iterator != components_series.end(); iterator++) {

        (*iterator)->set_frequency(frequency_Hz); // sets frequency for each component
        impedance += (*iterator)->get_impedance(); // adding them in series
        temp_string += (*iterator)->get_name() + "|s|-"; // adding the name of the component to the circuit text

        try {
            // using copy contructors to add the correct compoennt to the components vector based on the iterator's type
            if ((*iterator)->get_type() == "Resistor") {
                circuit_components_vector.emplace_back(new COMPONENTS::Resistor((*iterator), frequency_Hz)); // adding resistor to circuit vector 
            }
            if ((*iterator)->get_type() == "Capacitor") {
                circuit_components_vector.emplace_back(new COMPONENTS::Capacitor((*iterator), frequency_Hz)); // adding capacitor to circuit vector 
            }
            if ((*iterator)->get_type() == "Inductor") {
                circuit_components_vector.emplace_back(new COMPONENTS::Inductor((*iterator), frequency_Hz)); // adding inductor to circuit vector 
            }
        }
        catch (const std::bad_alloc&) {
            throw "Memory leak";
        }
    }

    circuit_text += temp_string;
}

// impedance sum parallel
void CIRCUITS::circuit::impedance_parallel_sum(std::vector<std::shared_ptr<COMPONENTS::base_components>> components_parallel)
{
    std::string components_line_text = "";
    std::complex<double> impedance_parallel_line;

    for (auto iterator{ components_parallel.begin() }; iterator != components_parallel.end(); iterator++) {
        (*iterator)->set_frequency(frequency_Hz); // setting freuency of each component 
        impedance_parallel_line += (*iterator)->get_impedance(); // adding up the impedance for the line
        components_line_text += (*iterator)->get_name() + "|p|-"; // adding the component to the text 

        try {
            // using copy contructors to add the correct compoennt to the components vector based on the iterator's type
            if ((*iterator)->get_type() == "Resistor") {
                circuit_components_vector.emplace_back(new COMPONENTS::Resistor((*iterator), frequency_Hz)); // adding resistor to circuit vector 
            }
            if ((*iterator)->get_type() == "Capacitor") {
                circuit_components_vector.emplace_back(new COMPONENTS::Capacitor((*iterator), frequency_Hz)); // adding capacitor to circuit vector 
            }
            if ((*iterator)->get_type() == "Inductor") {
                circuit_components_vector.emplace_back(new COMPONENTS::Inductor((*iterator), frequency_Hz)); // adding inductor to circuit vector 
            }
        }
        catch (const std::bad_alloc&) {
            throw "Memory leak";
        }
    }
    //Z_tot = (1/Z_1 + 1/Z_2)^-1
    impedance = std::pow(std::pow(impedance, -1) + std::pow(impedance_parallel_line, -1), -1);
    circuit_text = circuit_text + "\n" + components_line_text + "---|upline|---";
}

// calculating current
std::string CIRCUITS::circuit::calculate_current()
{
    double impedance_magnitude = std::abs(impedance);
    std::string result = (impedance_magnitude == 0) ? "infinity (due to 0 impedance please add components)" : std::to_string(voltage / impedance_magnitude);
    current = std::stod(result);
    return result;
}



// function to print table of components in circuits
void CIRCUITS::circuit::print_table_row(std::string name, std::string ideal, std::string component_type,
    double value, std::string units, double impedance_magnitude, std::complex<double> impedance, double phase, bool print_component)
{
    const int width_1 = 13;
    const int width_2 = 24;
    const int width_3 = 15;
    const int width_4 = 10;
    print_table_row_element(name, width_1);
    if (print_component == true) {
        if (ideal == "ideal") {
            print_table_row_element("Ideal " + component_type, width_2);
        }
        else {
            print_table_row_element("Non-ideal " + component_type, width_2);
        }
        print_table_row_element(value, width_4);
        print_table_row_element(units, width_1);
    }

    print_table_row_element(impedance, width_2);
    print_table_row_element(impedance_magnitude, width_4);
    print_table_row_element("Ohms", width_3);
    print_table_row_element(phase, width_4);
    print_table_row_element("deg", width_1);
}



void CIRCUITS::circuit::table_info()
{
    print_table_row(name, "", "circuit",
        0, "", std::abs(impedance), impedance, std::arg(impedance) * 180 / M_PI, false);
}

void CIRCUITS::circuit::basic_info()
{
    // print out basic circuit characteristics
    std::cout << "  The circuit's name is: " << name << std::endl;
    std::cout << '\n';
    std::cout << "    Voltage: " << voltage << " V," << " current: " << calculate_current() << " A" << ", frequency: " << frequency_Hz << " Hz" << std::endl;
    std::cout << "    Impedance magnitude of full circuit: " << std::abs(impedance) << " Ohm, " << "Full impedance is " << get_impedance() << " (Re,Im)" << std::endl;
    std::cout << "    Current lags voltage by: " << std::arg(impedance) * 180 / M_PI << " deg" << std::endl;
    std::cout << '\n';
}

// info function
void CIRCUITS::circuit::info()
{
    // print out circuit characteristics
    std::cout << '\n';
    std::cout << "--------------------------------------------------CIRCUIT INFO--------------------------------------------------" << std::endl;
    std::cout << '\n';
    basic_info();
    // checking if there are any components in the circuit
    if (get_size() > 0) {
        std::cout << '\n';
        std::cout << "--------------------------------------------------COMPONENTS----------------------------------------------------------------------------" << std::endl;
        std::cout << "label           component             Value(R/C/L)       impedance(Re,Im)        impedance magnitude        phase difference      Number" << std::endl;
        std::cout << "----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        //printing out table of components that make up the circuit
        none_repeated_info(circuit_components_vector);
        std::cout << "----------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        print_circuit_diagram();
        std::cout << '\n'; // extra space
    }
}




void CIRCUITS::circuit::print_circuit_diagram() const
{
    std::vector<std::string> strings_vector; // stores strings for each line of circuit
    std::string holding_string;

    // copies circuit text to another holding string so circuit text is not modified
    holding_string = circuit_text;
    std::string delimiter = "\n"; // delimeter in circuit text to seperate the lines
    std::size_t pos = 0;
    std::string string_upto_1st_delimeter;
    int token_length = 0;

    // spliting circuit text string
    while ((pos = holding_string.find(delimiter)) != std::string::npos) {

        string_upto_1st_delimeter = holding_string.substr(0, pos); // sets string_upto_1st_delimeter equal to everything in string until position of first delimiter
        strings_vector.push_back(string_upto_1st_delimeter); // push back a line of the circuit into a vector of strings 
        holding_string.erase(0, pos + delimiter.length()); // gets final line

    }

    strings_vector.push_back(holding_string); // pushes back final line of circuit

    std::cout << '\n';
    std::cout << "The circuit diagram for this circuit is:" << std::endl;
    std::cout << '\n';

    int printed_circuit_line_counter = 0; // counts how many lines of the circuit have been printed

    // printing out diagram
    for (auto iterator{ strings_vector.begin() }; iterator != strings_vector.end(); iterator++) {

        // if we are on the first line of the circuit
        if (printed_circuit_line_counter == 0) {
            // if circuit only has one line then print it like this
            if (strings_vector.size() == 1) {
                std::cout << "~V |---" << (*iterator) << "---| GND";
            }
            else {
                std::cout << "~V |---" << (*iterator) << "--|connect upline|";
            }

            // other lines of the circuit
        }
        else {

            if (printed_circuit_line_counter == strings_vector.size() - 1) {
                std::cout << "       " << (*iterator) << "---| GND";
            }
            else {
                std::cout << "       " << (*iterator) << "--|connect upline|";
            }
        }

        ++printed_circuit_line_counter; // counts how many times the loop runs

        // only prints out vertical connection if the printed_circuit_line_counter variable is smaller than the length of the strings vector 
        if (printed_circuit_line_counter < strings_vector.size()) {
            std::cout << '\n';
            std::cout << "       |"; // printing out first vertical wire
        }

        // only prints last line if we have reached the end of the circuit 
        if (printed_circuit_line_counter < strings_vector.size()) {
            std::cout << '\n';
        }

    }
    std::cout << '\n'; // vertical space after last line of circuit 

}

//function to give the number of components
int CIRCUITS::circuit::get_size() const {
    return circuit_components_vector.size(); //length of components vector
}

// getter for impedance
std::complex<double> CIRCUITS::circuit::get_impedance() const
{
    return impedance;
}
// name getter
std::string CIRCUITS::circuit::get_name() const
{
    return name;
}
//voltage getter
double CIRCUITS::circuit::get_voltage()const
{
    return voltage;
}
//frequency getter
double CIRCUITS::circuit::get_frequency()const
{
    return frequency_Hz;
}











// nested circuit class functions

// Recalculates impedance only if any of the circuits that make up the nested circuit change component number 
void CIRCUITS::nested_circuit::circuit_change_check()
{
    int temp_size;
    bool circuit_size_changed = false;
    std::complex<double> impedance_line;
    int parallel_line_counter = 0;
    int loop_counter = 0;

    // looping through vector of circuits to check their sizes
    for (auto iterator{ vector_of_circuits.begin() }; iterator != vector_of_circuits.end(); iterator++) {
        temp_size = (*iterator)->get_size();
        // check if the size of a circuit has changed
        if (temp_size != vector_of_circuit_sizes[loop_counter]) {
            circuit_size_changed = true; // size of a circuit has changed
        }
        vector_of_circuit_sizes[loop_counter] = temp_size; // updating circuit component sizes for next run of impedance update
        ++loop_counter;
    }
    // if circuit_size_changed is true recalculate impedance
    if (circuit_size_changed == true) {
        std::cout << '\n';
        std::cout << "WARNING! ONE OF THE CIRCUITS IN THIS NESTED CIRCUIT HAS BEEN CHANGED SO THE IMPEDANCE CALCULATIONS ARE NO LONGER VALID" << std::endl;
        std::cout << '\n';
    }
}

// impedance sum series
void CIRCUITS::nested_circuit::nested_impedance_series_sum(std::vector<std::shared_ptr<circuit>> circuit_series)
{
    std::string temp_string;

    for (auto iterator{ circuit_series.begin() }; iterator != circuit_series.end(); iterator++) {

        impedance += (*iterator)->get_impedance(); // adding them in series
        temp_string += (*iterator)->get_name() + "|s|--"; // adding the name of the component to the circuit text
        vector_of_circuits.emplace_back((*iterator)); // adding component to circuit vector 
        vector_of_circuit_sizes.push_back((*iterator)->get_size());
        // pushing back size of first line
        if (parallel_line_number == 0) {
            vector_of_line_sizes.push_back(circuit_series.size());
            // if any circuits are added in series to the same parallel line then add them to the size of that line
        }
        else {
            vector_of_line_sizes[parallel_line_number] += circuit_series.size();
        }
    }
    circuit_text += temp_string;
}

// impedance sum parallel
void CIRCUITS::nested_circuit::nested_impedance_parallel_sum(std::vector<std::shared_ptr<circuit>> circuits_parallel)
{
    std::string components_line_text = "";
    std::complex<double> impedance_parallel_line;

    for (auto iterator{ circuits_parallel.begin() }; iterator != circuits_parallel.end(); iterator++) {

        impedance_parallel_line += (*iterator)->get_impedance(); // adding up the impedance for the line
        components_line_text += (*iterator)->get_name() + "|p|--"; // adding the component to the text 
        vector_of_circuits.emplace_back((*iterator)); // adding component to circuit vector 
        vector_of_circuit_sizes.push_back((*iterator)->get_size()); // adding circuit sizes to the sizes vector
    }
    //Z_tot = (1/Z_1 + 1/Z_2)^-1
    impedance = std::pow(std::pow(impedance, -1) + std::pow(impedance_parallel_line, -1), -1);
    circuit_text = circuit_text + "\n" + components_line_text + "---|upline|---";
    ++parallel_line_number;
    // adding the number of circuits that were added to the nested circuit to the vector of parallel line sizes
    vector_of_line_sizes.push_back(circuits_parallel.size());
}

// function to print out  full information about nested circuit 
void CIRCUITS::nested_circuit::info()
{
    std::cout << '\n';
    std::cout << "---------------------------------------------NESTED CIRCUIT INFO------------------------------------------------" << std::endl;
    std::cout << '\n';
    // checking if any of the circuit have changed and output a warning message if so
    circuit_change_check(); // checking if any of the circuit have changed and output a warning message if so
    int counter = 0;
    std::vector<std::string> names_already_printed;
    std::cout << '\n';
    basic_info();
    if (vector_of_circuits.size() > 0) {

        // checking if there are any components in the circuit
        std::cout << '\n';
        std::cout << "--------------------------------------CIRCUITS--------------------------------------------" << std::endl;
        std::cout << "name      impedance(Re,Im)        impedance magnitude        phase difference      Number " << std::endl;
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        //printing out table of circuits that make up the nested circuit
        none_repeated_info(vector_of_circuits);
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        std::cout << '\n';
        print_circuit_diagram();
        std::cout << '\n';
        std::cout << "The information about the circuits that make up this nested circuit is given below" << std::endl;
        std::cout << '\n';

        // printing out full info about the circuits
        int counter = 0;
        std::vector<std::string> names_already_printed;
        // looping through vector of circuits to print out information about each one
        for (auto iterator{ vector_of_circuits.begin() }; iterator != vector_of_circuits.end(); iterator++) {
            std::cout << '\n';
            // prints out information for first circuit member
            if (counter == 0) {
                (*iterator)->info();
                names_already_printed.push_back((*iterator)->get_name());
                std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
                std::cout << '\n';

            }
            // if there circuit's name does not match any of the names in the vector of circuits print info
            if (repeated_name_counter(names_already_printed, (*iterator)->get_name()) == 0) {
                (*iterator)->info(); // print out info of individual circuits in vector
                std::cout << '\n';
                names_already_printed.push_back((*iterator)->get_name());
                std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
            }

            ++counter;
        }
    }
}

// gets number of circuits in the nested circuit
int CIRCUITS::nested_circuit::get_size() const
{
    return vector_of_circuits.size();
}











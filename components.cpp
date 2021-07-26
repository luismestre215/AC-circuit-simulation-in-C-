// components.cpp 
// PHYS 30762 Programming in C++
// Luis Mestre, last updated 16/05/2021
// component class functions

#include"components.h"

// function to print table of components in circuits
void print_table_row(std::string name, std::string ideal, std::string component_type,
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



// Methods for Resistor class

// deep copy constructor
COMPONENTS::Resistor::Resistor(std::shared_ptr<base_components> Resistor_ptr, double frequency)
{
    frequency_Hz = frequency;
    ideal_resistance = Resistor_ptr->get_component_value();
    parasitic_capacitance = Resistor_ptr->get_parasitic_1();
    parasitic_inductance = Resistor_ptr->get_parasitic_2();
    name = Resistor_ptr->get_name();
    impedance = calculate_impedance(frequency);
}

// calculating impedance
std::complex<double> COMPONENTS::Resistor::calculate_impedance(double frequency_Hz)
{
    double frequency_rad = frequency_Hz * 2 * M_PI;
    std::complex<double> numerator(ideal_resistance, frequency_rad * parasitic_inductance);
    double real_part_denominator = 1 - std::pow(frequency_rad, 2) * parasitic_capacitance * parasitic_inductance;
    double im_part_denominator = frequency_rad * ideal_resistance * parasitic_capacitance;
    std::complex<double> denominator(real_part_denominator, im_part_denominator);

    impedance = numerator / denominator;

    return impedance;
}
// info function 
void COMPONENTS::Resistor::table_info()
{
    std::string ideal;
    if (parasitic_inductance == 0 & parasitic_capacitance == 0) {
        ideal = "ideal";
    }
    print_table_row(name, ideal, component_type, ideal_resistance, "Ohms", get_impedance_magnitude(), impedance, get_phase_difference(), true);
}
// outputs info without impedance for library 
void COMPONENTS::Resistor::basic_info()
{
    if (parasitic_capacitance == 0 & parasitic_inductance == 0) {
        std::cout << "   Resistor " << name << " has ideal resistance: " << ideal_resistance << " Ohm" << std::endl;
    }
    else {
        std::cout << "   Non-ideal Resistor " << name << " has ideal resistance: " << ideal_resistance << " Ohm" << std::endl;
        std::cout << "   parasitic capacitance: " << parasitic_capacitance << " F and parasitic inductance: " << parasitic_inductance << " H" << std::endl;
    }
}

void COMPONENTS::Resistor::set_frequency(const double frequency)
{
    frequency_Hz = frequency;
    calculate_impedance(frequency);
}


// getters
// getter for impedance
std::complex<double> COMPONENTS::Resistor::get_impedance() const
{
    return impedance;
}

// getter for phase difference
double COMPONENTS::Resistor::get_phase_difference() const
{
    return std::arg(impedance) * 180 / M_PI;
}
// getter for magnitude of impedance
double COMPONENTS::Resistor::get_impedance_magnitude() const
{
    return std::abs(impedance);
}
// getter for frequency
double COMPONENTS::Resistor::get_frequency() const
{
    return frequency_Hz;
}
// getter for component name 
std::string COMPONENTS::Resistor::get_name() const
{
    return name;
}

// returns parasitic resistance
double COMPONENTS::Resistor::get_component_value() const
{
    return ideal_resistance;
}
// returns parasitic capacitance
double COMPONENTS::Resistor::get_parasitic_1() const
{
    return parasitic_capacitance;
}
// returns parasitic inductance
double COMPONENTS::Resistor::get_parasitic_2() const
{
    return parasitic_inductance;
}
// return type
std::string COMPONENTS::Resistor::get_type() const
{
    return component_type;
}
















// Methods Capacitor


// deep copy constructor
COMPONENTS::Capacitor::Capacitor(std::shared_ptr<base_components> capacitor_ptr, double frequency)
{
    frequency_Hz = frequency;
    ideal_capacitance = capacitor_ptr->get_component_value();
    parasitic_resistance = capacitor_ptr->get_parasitic_1();
    parasitic_inductance = capacitor_ptr->get_parasitic_2();
    name = capacitor_ptr->get_name();
    impedance = calculate_impedance(frequency);
}

// info function 
void COMPONENTS::Capacitor::table_info()
{
    std::string ideal;
    if (parasitic_inductance == 0 & parasitic_inductance == 0) {
        ideal = "ideal";
    }
    print_table_row(name, ideal, component_type, ideal_capacitance, "F", get_impedance_magnitude(), impedance, get_phase_difference(), true);
}
// basic info no impedance 
void COMPONENTS::Capacitor::basic_info()
{
    if (parasitic_resistance == 0 & parasitic_inductance == 0) {
        std::cout << "   Capacitor " << name << " has ideal capacitance " << ideal_capacitance << " F" << std::endl;
    }
    else {
        std::cout << "   Non-ideal Capacitor " << name << " has ideal capacitance " << ideal_capacitance << " F" << std::endl;
        std::cout << "   Parasitic resistance: " << parasitic_resistance << " Ohm and parasitic inductance: " << parasitic_inductance << " H" << std::endl;
    }
}

// impedance calculation function
std::complex<double>COMPONENTS::Capacitor::calculate_impedance(double frequency_Hz)
{
    double frequency_rad = frequency_Hz * 2 * M_PI;
    double im_part = frequency_rad * parasitic_inductance - (1 / (frequency_rad * ideal_capacitance));
    std::complex<double> final_result(parasitic_resistance, im_part);
    impedance = final_result;

    return impedance;

}

// setters 
// set frequency 
void COMPONENTS::Capacitor::set_frequency(const double frequency)
{
    frequency_Hz = frequency;
    calculate_impedance(frequency); // calculates impedance
}

// getters
// getter for frequency 
double COMPONENTS::Capacitor::get_frequency() const
{
    return frequency_Hz;
}

// getter for impedance
std::complex<double> COMPONENTS::Capacitor::get_impedance() const
{
    return impedance;
}

// getter for phase difference
double COMPONENTS::Capacitor::get_phase_difference() const
{
    return std::arg(impedance) * 180 / M_PI;
}
// getter for impedance magnitude
double COMPONENTS::Capacitor::get_impedance_magnitude() const
{
    return std::abs(impedance);
}

// getter for component name 
std::string COMPONENTS::Capacitor::get_name() const
{
    return name;
}

// returns parasitic resistance
double COMPONENTS::Capacitor::get_component_value() const
{
    return ideal_capacitance;
}
// returns parasitic capacitance
double COMPONENTS::Capacitor::get_parasitic_1() const
{
    return parasitic_resistance;
}
// returns parasitic inductance
double COMPONENTS::Capacitor::get_parasitic_2() const
{
    return parasitic_inductance;
}
// return type
std::string COMPONENTS::Capacitor::get_type() const
{
    return component_type;
}














// Methods for Inductor

// deep copy constructor
COMPONENTS::Inductor::Inductor(std::shared_ptr<base_components> Resistor_ptr, double frequency)
{
    frequency_Hz = frequency;
    ideal_inductance = Resistor_ptr->get_component_value();
    parasitic_resistance = Resistor_ptr->get_parasitic_1();
    parasitic_capacitance = Resistor_ptr->get_parasitic_2();
    name = Resistor_ptr->get_name();
    impedance = calculate_impedance(frequency);
}

// method for calculating impedance of non ideal inductor
std::complex<double>COMPONENTS::Inductor::calculate_impedance(double frequency_Hz)
{
    double frequency_rad = frequency_Hz * 2 * M_PI;
    std::complex <double> numerator(parasitic_resistance, frequency_rad * ideal_inductance);
    double real_part_denominator = 1 - (std::pow(frequency_rad, 2) * parasitic_capacitance * ideal_inductance);
    double im_part_denominator = frequency_rad * parasitic_capacitance * parasitic_resistance;
    std::complex <double> denominator(real_part_denominator, im_part_denominator);

    impedance = numerator / denominator;
    return impedance;
}

// method for outputting the information stored in the class
void COMPONENTS::Inductor::table_info()
{
    std::string ideal;
    if (parasitic_resistance == 0 & parasitic_capacitance == 0) {
        ideal = "ideal";
    }
    print_table_row(name, ideal, component_type, ideal_inductance, "H", get_impedance_magnitude(), impedance, get_phase_difference(), true);
}
// method for printing basic info about component to use in component library
void COMPONENTS::Inductor::basic_info()
{
    if (parasitic_capacitance == 0 & parasitic_resistance == 0) {
        std::cout << "   Inductor: " << name << " has ideal inductance: " << ideal_inductance << " H" << std::endl;
    }
    else {
        std::cout << "   Non-ideal Inductor " << name << " has ideal inductance: " << ideal_inductance << " H" << std::endl;
        std::cout << "   parasitic capacitance: " << parasitic_capacitance << " F and parasitic resistance: " << parasitic_resistance << " R" << std::endl;
    }
}

// setters 
// set frequency 
void COMPONENTS::Inductor::set_frequency(const double frequency)
{
    frequency_Hz = frequency;
    calculate_impedance(frequency);
}

// getters
// getter for impedance
std::complex<double>COMPONENTS::Inductor::get_impedance() const
{
    return impedance;
}

// getter for phase difference
double COMPONENTS::Inductor::get_phase_difference() const
{
    return std::arg(impedance) * 180 / M_PI;
}

// getter for magnitude of impedance
double COMPONENTS::Inductor::get_impedance_magnitude() const
{
    return std::abs(impedance);
}

// getter for frequency
double COMPONENTS::Inductor::get_frequency() const
{
    return frequency_Hz;
}

// getter for component name 
std::string COMPONENTS::Inductor::get_name() const
{
    return name;
}


// returns parasitic resistance
double COMPONENTS::Inductor::get_component_value() const
{
    return ideal_inductance;
}
// returns parasitic capacitance
double COMPONENTS::Inductor::get_parasitic_1() const
{
    return parasitic_resistance;
}
// returns parasitic inductance
double COMPONENTS::Inductor::get_parasitic_2() const
{
    return parasitic_capacitance;
}
// return type
std::string COMPONENTS::Inductor::get_type() const
{
    return component_type;
}

// components.h 
// PHYS 30762 Programming in C++
// Luis Mestre, last updated 16/05/2021
// component classes

// component classes
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include<string>
#include<iostream>
#include<iomanip>
#include<complex>
#include<cmath>
#include<vector>
#include <limits>
#include<memory>
#include <algorithm>


// function to print out a table element with correct spacing
template<typename T> void print_table_row_element(T element_to_print, const int& width)
{
    const char seperator = ' ';
    std::cout << std::left << std::setw(width) << std::setfill(seperator) << element_to_print;
}



namespace COMPONENTS {

    // base class for all components
    class base_components
    {
    public:
        // virtual destructor
        virtual ~base_components() {};

        //pure virtual methods
        std::complex<double> virtual calculate_impedance(double) = 0;
        void virtual table_info() = 0;
        void virtual basic_info() = 0;

        // getters
        std::complex<double> virtual get_impedance() const = 0;
        double virtual get_phase_difference() const = 0;
        double virtual get_impedance_magnitude() const = 0;
        double virtual get_frequency() const = 0;
        std::string virtual get_name() const = 0;
        std::string virtual get_type() const = 0;
        double virtual get_component_value() const = 0;
        double virtual get_parasitic_1() const = 0;
        double virtual get_parasitic_2() const = 0;

        // setters
        void virtual set_frequency(const double) = 0;
    }; // end of base class for components




    // component classes

    //non-ideal resistor class
    class Resistor : public base_components
    {
    private:
        std::string name = "R"; // default name is R
        std::string component_type = "Resistor";
        double ideal_resistance = 0;
        double parasitic_capacitance = 0;
        double parasitic_inductance = 0;
        std::complex<double> impedance = 0;
        double frequency_Hz = 0;
    public:
        // constructors
        Resistor() {}
        Resistor(double R_ideal, double C_parasitic, double L_parasitic, std::string name)
            : ideal_resistance{ R_ideal }, parasitic_capacitance{ C_parasitic }, parasitic_inductance{ L_parasitic }, name{ name } {}
        Resistor(std::shared_ptr<base_components>, double); // copy constructor
        ~Resistor() {}

        // methods

        // impedance calculation for non ideal resistor
        std::complex<double>calculate_impedance(double frequency_Hz);
        // info function 
        void table_info();

        // setters 
        void set_frequency(const double frequency);
        void basic_info();

        // getters
        std::complex<double> get_impedance() const;
        double get_phase_difference() const;
        double get_impedance_magnitude() const;
        double get_frequency() const;
        std::string get_name() const;
        std::string get_type() const;
        double get_component_value() const;
        double get_parasitic_1() const;
        double get_parasitic_2() const;

    }; // end of non-ideal resistor class 




    //non-ideal capacitor class
    class Capacitor : public base_components
    {
    private:
        std::string name = "C";
        std::string component_type = "Capacitor";
        double ideal_capacitance = 0;
        double parasitic_resistance = 0;
        double parasitic_inductance = 0;
        std::complex<double> impedance = 0;
        double frequency_Hz = 0;
    public:
        Capacitor() {}
        Capacitor(double C_ideal, double R_parasitic, double L_parasitic, std::string name)
            : ideal_capacitance{ C_ideal }, parasitic_resistance{ R_parasitic }, parasitic_inductance{ L_parasitic }, name{ name } {}
        Capacitor(std::shared_ptr<base_components>, double); // deep copy constructor
        ~Capacitor() {}

        // methods
        std::complex<double>calculate_impedance(double frequency_Hz);
        void table_info();
        void basic_info();

        // setters 
        void set_frequency(const double frequency);

        // getters
        double get_frequency() const;
        std::complex<double> get_impedance() const;
        double get_phase_difference() const;
        double get_impedance_magnitude() const;
        std::string get_name() const;
        std::string get_type() const;
        double get_component_value() const;
        double get_parasitic_1() const;
        double get_parasitic_2() const;

    }; // end of non-ideal capacitor class 




    //non-ideal inductor class
    class Inductor : public base_components
    {
    private:
        std::string name = "L";
        std::string component_type = "Inductor";
        double ideal_inductance = 0;
        double parasitic_capacitance = 0;
        double parasitic_resistance = 0;
        std::complex<double> impedance = 0;
        double frequency_Hz = 0;
    public:
        Inductor() {}
        Inductor(double L_ideal, double R_parasitic, double C_parasitic, std::string name)
            : ideal_inductance{ L_ideal }, parasitic_resistance{ R_parasitic }, parasitic_capacitance{ C_parasitic }, name{ name } {}
        Inductor(std::shared_ptr<base_components>, double);
        ~Inductor() {}

        // methods 
        std::complex<double>calculate_impedance(double frequency_Hz);
        void table_info();
        void basic_info();

        // setters 
        void set_frequency(const double frequency);

        // getters
        std::complex<double> get_impedance() const;
        double get_phase_difference() const;
        double get_impedance_magnitude() const;
        double get_frequency() const;
        std::string get_name() const;
        std::string get_type() const;
        double get_component_value() const;
        double get_parasitic_1() const;
        double get_parasitic_2() const;

    }; // end of non-ideal Inductor class 

} // end of namespace



#endif







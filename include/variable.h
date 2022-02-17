// Header file for class variable

#include <string>

using namespace std;

// Class to represent variables
class Variable {
public:
    string name;
    double value;
    Variable(string n, double d) :name(n), value(d) { }
};

#include <iostream>
#include "Controllers/Controller.hpp"
#include "Views/Console/ConsoleView.hpp"
using namespace std;

int main() {
    ConsoleView view(cin, cout);
    Controller controller;
    view.Run(controller);
    return 0;
}
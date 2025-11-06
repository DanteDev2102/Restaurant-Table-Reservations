#include <iostream>
#include "interface.hpp"
#include "interface.cpp"
#include "application.cpp"

using namespace std;


int main(int argc, char** argv) {
	CmdInterface cmd = CmdInterface();
	cmd.run();
	return 0;
}

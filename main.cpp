#include <iostream>
#include "interface.hpp"


using namespace std;


int main(int argc, char** argv) {
	CmdInterface cmd = CmdInterface();
	cmd.run();
	return 0;
}

#include "classes.hpp"


int main(int ac, char **av)
{
	try
	{
		Scanner scanner(ac, av);
	}
	catch (std::string error)
	{
		std::cerr << error << "\nQuitting!!!" << std::endl;
		return 1;
	}
}
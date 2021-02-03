#include <iostream>

#include <casimir/casimir.hpp>
#include <casimir/utilities/optional.hpp>

using namespace Casimir;
using namespace utilities;
using namespace literals;

int main(int, char**) {
	int* b = new int(50);
	Optional<int> a = Optional<int>::of(4);
	std::cout << a.orElse(0) << std::endl;
	std::cout << a.map<String>([](int c) { return "Test" + String::toString((cint)c); }).orElse("nothing").str() << std::endl;
	std::cout << a.isPresent() << std::endl;
	a.reset();
	
	std::cout << *b << std::endl;
}
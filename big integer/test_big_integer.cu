
#include "thrust/host_vector.h"
#include "thrust/device_vector.h"
#include "thrust/copy.h"
#include "thrust/reduce.h"
#include "thrust/sequence.h"
#include "thrust/generate.h"
#include "thrust/functional.h"

#include "big_integer.h"
#include <cstdlib>
#include <iostream>
#include <iterator>

void test_int_generate_and_reduce()
{
	std::cout<< "test generate and reduce with int" << std::endl;
	thrust::host_vector<thrust::big_integer<2>> h_v(10, 0xFFFF);
	
	std::cout << "Print host vector elements: ";
	for(int i = 0; i < h_v.size(); i++)
		std::cout << (unsigned int)h_v[i] << " ";
	std::cout << "\n";
	
	unsigned int sum = thrust::reduce(h_v.begin(), h_v.end());
	std::cout << "Reduce from host: " << sum << "\n";
	
	thrust::device_vector<thrust::big_integer<2>> d_v = h_v;
	sum = thrust::reduce(d_v.begin(), d_v.end());
	std::cout << "Reduce from device: " << sum << "\n";
}

int main()
{
	test_int_generate_and_reduce();
	
	system("pause");
	return 0;
}
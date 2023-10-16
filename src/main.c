#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define GET_BIT(x, i) (((x) >> (i)) & 1)
#define SET_BIT(x, i, b) ((x) |= (b << (i)))
#define BOOL2STR(b) ((b) ? "true" : "false")
#define UINT4_MAX 15 

bool overflow_flag = false;

typedef uint32_t(*integer_adder_fnptr)(uint32_t, uint32_t);

typedef struct test_data_s test_data_t;

struct test_data_s
{
	integer_adder_fnptr adder;
	uint32_t a;
	uint32_t b;
};

void full_bit_adder(uint32_t a, uint32_t b, uint32_t cin, uint32_t *s, uint32_t *cout)
{
	*s = (a ^ b) ^ cin;
	*cout = (a & b) | (cin & (a ^ b));
}

uint32_t binary_adder(uint32_t a, uint32_t b, uint32_t operand_size)
{
	int i = 0;
	uint32_t cin = 0;
	uint32_t cout = 0; 
	uint32_t s = 0;
	uint32_t sum = 0;
	overflow_flag = false;
	while(i < operand_size)
	{
		uint32_t bit_a = GET_BIT(a, i);
		uint32_t bit_b = GET_BIT(b, i);
		cin = cout;
		full_bit_adder(bit_a, bit_b, cin, &s, &cout);
		SET_BIT(sum, i, s);
		i++;
	} 
	if(cout)
		overflow_flag = true;
	return sum;
}

uint32_t nibble_adder(uint32_t a, uint32_t b)
{
	return binary_adder(a, b, 4);
}
uint32_t byte_adder(uint32_t a, uint32_t b)
{
	return binary_adder(a, b, 8);
}
uint32_t word_adder(uint32_t a, uint32_t b)
{
	return binary_adder(a, b, 16);
}
uint32_t dword_adder(uint32_t a, uint32_t b)
{
	return binary_adder(a, b, 32);
}

void test_adder(test_data_t test)
{
	uint32_t sum = test.adder(test.a, test.b);
	printf("%d + %d = %d, OF = %s\n", test.a, test.b, sum, BOOL2STR(overflow_flag));
}



int main()
{
	test_data_t tests[] = {
		// Overflow tests
		(test_data_t) { nibble_adder, 1, UINT4_MAX },
		(test_data_t) { byte_adder, 1, UINT8_MAX },
		(test_data_t) { word_adder, 1, UINT16_MAX },
		(test_data_t) { dword_adder, 1, UINT32_MAX },
	};
	return 0;
}
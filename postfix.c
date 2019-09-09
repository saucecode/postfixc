/*
	saucecode's postfix calculator in C
	version 1.0          2019-09-10
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Define the internal float type to use
#ifndef PF_FLOAT
#define PF_FLOAT double
#endif

// Define the internal string to float conversion type to use
#ifndef PF_ATOF
#define PF_ATOF atof
#endif

#define PF_TYPE_NUMBER 1
#define PF_TYPE_OPERATION 2
#define PF_TYPE_NULL 3

struct symbol {
	char type;
	union {
		PF_FLOAT value;
		unsigned short operation;
	};
};

struct operation {
	char arity;
	char *name;
	
	union {
		PF_FLOAT (*func_zero)();
		PF_FLOAT (*func_one)(PF_FLOAT a);
		PF_FLOAT (*func_two)(PF_FLOAT a, PF_FLOAT b);
	};
};

void convert_inputs_to_symbols(char **inputs, struct symbol *outputs, int length,
                              struct operation *operations, int operation_count) {
                              
	for(int i = 0; i < length; i += 1){
		if(isalpha(inputs[i][0]) || (strlen(inputs[i]) == 1 && NULL != strrchr("+-*/", inputs[i][0]))) {
		
			outputs[i].type = PF_TYPE_OPERATION;
			for(int j = 0; j < operation_count; j += 1){
				if(strcmp(operations[j].name, inputs[i]) == 0) {
					outputs[i].operation = j;
					break;
				}
			}
			
		} else {
			outputs[i].type = PF_TYPE_NUMBER;
			outputs[i].value = (PF_FLOAT) PF_ATOF(inputs[i]);
			
		}
	}
}

PF_FLOAT *do_postfix_expr(char **inputs, int input_count, struct operation *operations, int operation_count, int *output_length) {
	struct symbol *symbols = (struct symbol*) malloc((input_count + 1) * sizeof(struct symbol));
	symbols[input_count].type = PF_TYPE_NULL; // null terminator, but for struct symbol
	
	convert_inputs_to_symbols(inputs, symbols, input_count, operations, operation_count);
	
	int cursor = 0;
	PF_FLOAT *stack = malloc(sizeof(PF_FLOAT) * input_count);
	int stack_counter = 0;
	
	while(symbols[cursor].type != PF_TYPE_NULL) {
		if(symbols[cursor].type == PF_TYPE_NUMBER) {
			stack[stack_counter] = symbols[cursor].value;
			stack_counter += 1;
			cursor += 1;
			
		} else if(symbols[cursor].type == PF_TYPE_OPERATION) {
			struct symbol *s = &symbols[cursor];
			struct operation *op = &operations[s->operation];
			
			if(op->arity == 0) {
				stack[stack_counter] = op->func_zero();
				stack_counter += 1;
				cursor += 1;
			} else if(op->arity == 1) {
				stack[stack_counter - 1] = op->func_one(stack[stack_counter - 1]);
				cursor += 1;
			} else if(op->arity == 2) {
				stack[stack_counter - 2] = op->func_two(stack[stack_counter - 2], stack[stack_counter - 1]);
				stack_counter -= 1;
				cursor += 1;
			}
			
		}
	}
	
	free(symbols);
	*output_length = stack_counter;
	return stack;
}

PF_FLOAT PF_FUNC_ADD(PF_FLOAT a, PF_FLOAT b) { return a + b; }
PF_FLOAT PF_FUNC_SUB(PF_FLOAT a, PF_FLOAT b) { return a - b; }
PF_FLOAT PF_FUNC_MUL(PF_FLOAT a, PF_FLOAT b) { return a * b; }
PF_FLOAT PF_FUNC_DIV(PF_FLOAT a, PF_FLOAT b) { return a / b; }
PF_FLOAT PF_FUNC_SQRT(PF_FLOAT a) { return sqrt(a); }
PF_FLOAT PF_FUNC_POW(PF_FLOAT a, PF_FLOAT b) { return pow(a, b); }
PF_FLOAT PF_FUNC_EXP(PF_FLOAT a) { return exp(a); }
PF_FLOAT PF_FUNC_LOG(PF_FLOAT a) { return log(a); }

PF_FLOAT PF_FUNC_SIN(PF_FLOAT a) { return sin(a); }
PF_FLOAT PF_FUNC_COS(PF_FLOAT a) { return cos(a); }
PF_FLOAT PF_FUNC_TAN(PF_FLOAT a) { return tan(a); }
PF_FLOAT PF_FUNC_ASIN(PF_FLOAT a) { return asin(a); }
PF_FLOAT PF_FUNC_ACOS(PF_FLOAT a) { return acos(a); }
PF_FLOAT PF_FUNC_ATAN(PF_FLOAT a) { return atan(a); }
PF_FLOAT PF_FUNC_TO_DEGREES(PF_FLOAT a) { return a * 180.0 / M_PI; }
PF_FLOAT PF_FUNC_TO_RADIANS(PF_FLOAT a) { return a / 180.0 * M_PI; }

PF_FLOAT PF_FUNC_INVERSE(PF_FLOAT a) { return 1.0 / a; }

PF_FLOAT PF_FUNC_CONST_PI() { return (PF_FLOAT) M_PI; }
PF_FLOAT PF_FUNC_CONST_EULER() { return (PF_FLOAT) M_E; }

int main(int argc, char **argv) {
	if(argc <= 1) {
		printf("USAGE: %s [RPN expression]\nExample: %s 3 4 + 2 /\n", argv[0], argv[0]);
		return 1;
	}
	
	struct operation operations[19];
	operations[0].arity = 2;
	operations[0].name = "+";
	operations[0].func_two = &PF_FUNC_ADD;
	
	operations[1].arity = 2;
	operations[1].name = "-";
	operations[1].func_two = &PF_FUNC_SUB;
	
	operations[2].arity = 2;
	operations[2].name = "*";
	operations[2].func_two = &PF_FUNC_MUL;
	
	operations[3].arity = 2;
	operations[3].name = "/";
	operations[3].func_two = &PF_FUNC_DIV;
	
	operations[4].arity = 1;
	operations[4].name = "sqrt";
	operations[4].func_one = &PF_FUNC_SQRT;
	
	operations[5].arity = 2;
	operations[5].name = "pow";
	operations[5].func_two = &PF_FUNC_POW;
	
	operations[6].arity = 1;
	operations[6].name = "exp";
	operations[6].func_one = &PF_FUNC_EXP;
	
	operations[7].arity = 1;
	operations[7].name = "log";
	operations[7].func_one = &PF_FUNC_LOG;
	
	operations[8].arity = 1;
	operations[8].name = "sin";
	operations[8].func_one = &PF_FUNC_SIN;
	
	operations[9].arity = 1;
	operations[9].name = "cos";
	operations[9].func_one = &PF_FUNC_COS;
	
	operations[10].arity = 1;
	operations[10].name = "tan";
	operations[10].func_one = &PF_FUNC_TAN;
	
	operations[11].arity = 1;
	operations[11].name = "asin";
	operations[11].func_one = &PF_FUNC_ASIN;
	
	operations[12].arity = 1;
	operations[12].name = "acos";
	operations[12].func_one = &PF_FUNC_ACOS;
	
	operations[13].arity = 1;
	operations[13].name = "atan";
	operations[13].func_one = &PF_FUNC_ATAN;
	
	operations[14].arity = 0;
	operations[14].name = "pi";
	operations[14].func_zero = &PF_FUNC_CONST_PI;
	
	operations[15].arity = 0;
	operations[15].name = "e";
	operations[15].func_zero = &PF_FUNC_CONST_EULER;
	
	operations[16].arity = 1;
	operations[16].name = "inv";
	operations[16].func_one = &PF_FUNC_INVERSE;

	operations[17].arity = 1;
	operations[17].name = "rad";
	operations[17].func_one = &PF_FUNC_TO_RADIANS;
	
	operations[18].arity = 1;
	operations[18].name = "deg";
	operations[18].func_one = &PF_FUNC_TO_DEGREES;

	
	int output_length = -1;
	PF_FLOAT *output = do_postfix_expr(argv+1, argc - 1, operations, sizeof(operations)/sizeof(struct operation), &output_length);
	
	if(output_length <= 0) return 2;
	
	for(int i = 0; i < output_length - 1; i += 1)
		printf("%f ", output[i]);
	printf("%f\n", output[output_length - 1]);
	
	free(output);
	return 0;
}
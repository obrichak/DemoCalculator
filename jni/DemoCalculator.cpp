#include <jni.h>

#include <android/log.h>
#include "DemoCalculator.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <vector>
#include <cstring>
using namespace std;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Calc", __VA_ARGS__))

#define INP_STATUS_OK 1
#define INP_STATUS_WRONG_END -1
#define INP_STATUS_WRONG_PARENTHESES -2
#define INP_STATUS_WRONG -3

char* substr(char* str, int offset, int len);
vector<char*> toPostfix(vector<char*> elements);
double calculate(vector<char*> elements);
bool isSign(char symbol);

//this method returns:
//-1, if expression ends with operator.
//-2, if inconsistencies with brackets.
//-3, for the rest of incorrect expressions.
JNIEXPORT jint JNICALL Java_com_demo_calculator_CalculatorActivity_checkInput(
		JNIEnv *env, jclass myclass, jstring str) {
	const char *newstring = env->GetStringUTFChars(str, NULL);
	char last = newstring[strlen(newstring) - 1];
	if (isSign(last)) {
		return INP_STATUS_WRONG_END;
	}

	int k_left = 0, k_right = 0, k;
	for (k = 0; k < strlen(newstring); k++) {
		if (newstring[k] == '(') {
			if (k == 0 && strlen(newstring) == 2)
				return INP_STATUS_WRONG;
			k_left++;
			if (k != 0 && isdigit(newstring[k - 1]))
				return INP_STATUS_WRONG;
			if (k != (strlen(newstring) - 1) && isSign(newstring[k + 1])
					&& newstring[k + 1] != '-') {
				return INP_STATUS_WRONG;
			}
		} else if (newstring[k] == ')') {
			k_right++;
			if (k_right > k_left) {
				return INP_STATUS_WRONG_PARENTHESES;
			}
			if (k != (strlen(newstring) - 1) && isdigit(newstring[k + 1])) {
				return INP_STATUS_WRONG;
			}
			if (k != 0 && isSign(newstring[k - 1])) {
				return INP_STATUS_WRONG;
			}
		}
		else if (newstring[k] == '.') {
			if (k != (strlen(newstring) - 1)) {
				if (newstring[k + 1] == '.') {
					return INP_STATUS_WRONG;
				} else if (isSign(newstring[k + 1])
						&& (k == 0 || isSign(newstring[k - 1]))) {
					return INP_STATUS_WRONG;
				}
			}
		} else if (isSign(newstring[k])) {
			if (k == 0 && newstring[k] != '-')
				return INP_STATUS_WRONG;
			if (k != (strlen(newstring) - 1) && isSign(newstring[k + 1])) {
				return INP_STATUS_WRONG;
			}
		}
	}

	if (k_right != k_left) {
		return INP_STATUS_WRONG_PARENTHESES;
	}

	return INP_STATUS_OK;
}

bool isSign(char symbol) {
	if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') {
		return true;
	} else {
		return false;
	}
}

JNIEXPORT jdouble JNICALL Java_com_demo_calculator_CalculatorActivity_makeCalculations(
		JNIEnv *env, jclass myclass, jstring str) {
	const char *newstring = env->GetStringUTFChars(str, NULL);
	char* cstr = new char[strlen(newstring) + 1];
	strcpy(cstr, newstring);
	int start_sym = 0;
	vector<char*> elements;
	int index, size = strlen(cstr);
	for (index = 0; index <= size; index++) {
		if ((index == size || !isdigit(cstr[index])) && cstr[index] != '.') {
			if (start_sym != index) {
				char *current;
				current = substr(cstr, start_sym, index - start_sym);
				elements.push_back(current);
			}
			if (index != size) {
				char *symbol = new char[1];
				symbol[0] = cstr[index];
				if ((index == 0 || cstr[index - 1] == '(')
						&& cstr[index] == '-') {
				} else {
					elements.push_back(symbol);
					start_sym = index + 1;
				}
			}
		}
	}
	vector<char*> postfixElements = toPostfix(elements);
	return calculate(postfixElements);
}

vector<char*> toPostfix(vector<char*> elements) {
	vector<char*> stack;
	vector<char*> postfixElements;
	int i;
	for (i = 0; i < elements.size(); i++) {
		char* current = elements[i];
		if (current[0] == '0' || current[1] == '0'
				|| strtod(current, NULL) != 0.0) {
			postfixElements.push_back(current);
		} else {
			switch (current[0]) {
			case '+':
			case '-': {
				int j;
				for (j = stack.size() - 1; j >= 0; j--) {
					if (stack.size() != 0) {
						char* last = stack[j];
						if (last[0] == '+' || last[0] == '-' || last[0] == '*'
								|| last[0] == '/') {
							postfixElements.push_back(last);
							stack.pop_back();
						} else
							break;
					}
				}
				stack.push_back(current);
			}
				break;
			case '*':
			case '/': {
				int j;
				for (j = stack.size() - 1; j >= 0; j--) {
					if (stack.size() != 0) {
						char* last = stack[j];
						if (last[0] == '*' || last[0] == '/') {
							postfixElements.push_back(last);
							stack.pop_back();
						} else
							break;
					}
				}
				stack.push_back(current);
			}
				break;
			case '(':
				stack.push_back(current);
				break;
			case ')':
				char* last = stack.at(stack.size() - 1);
				while (stack.size() > 0) {
					if (last[0] != '(') {
						postfixElements.push_back(last);
						stack.pop_back();
						last = stack.at(stack.size() - 1);
					} else {
						stack.pop_back();
						break;
					}
				}

				break;
			}
		}
	}
	while (stack.size() != 0) {
		char* last = stack.at(stack.size() - 1);
		postfixElements.push_back(last);
		stack.pop_back();
	}
	return postfixElements;
}

double calculate(vector<char*> elements) {
	vector<char*> stack;
	int i;
	for (i = 0; i < elements.size(); i++) {
		char* current = elements[i];
		if (current[0] == '0' || current[1] == '0'
				|| strtod(current, NULL) != 0.0) {
			stack.push_back(current);
		} else {
			double result;
			double last = strtod(stack.at(stack.size() - 1), NULL);
			double beforeLast = strtod(stack.at(stack.size() - 2), NULL);
			switch (current[0]) {
			case '+':
				result = beforeLast + last;
				break;
			case '-':
				result = beforeLast - last;
				break;
			case '*':
				result = beforeLast * last;
				break;
			case '/':
				result = beforeLast / last;
				break;
			}
			stack.pop_back();
			stack.pop_back();
			char res[100];
			sprintf(res, "%f", result);
			stack.push_back(res);
		}
	}
	char* tojava = stack[0];
	return strtod(tojava, NULL);
}

char* substr(char str[], int offset, int len) {
	char* res = new char[len];
	int i;
	for (i = 0; i < len; i++) {
		res[i] = str[offset + i];
	}
	return res;
}

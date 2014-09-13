/*
 * DemoCalculator.h
 *
 *  Created on: Sep 12, 2014
 *      Author: mbeloded
 */

#ifndef DEMOCALCULATOR_H_
#define DEMOCALCULATOR_H_

#include <jni.h>

extern "C" {
#endif
	JNIEXPORT jint JNICALL Java_com_demo_calculator_CalculatorActivity_checkInput(
		JNIEnv *, jclass, jstring);
	JNIEXPORT jdouble JNICALL Java_com_demo_calculator_CalculatorActivity_makeCalculations(
			JNIEnv *, jclass, jstring);

#ifdef __cplusplus
}

#endif /* DEMOCALCULATOR_H_ */

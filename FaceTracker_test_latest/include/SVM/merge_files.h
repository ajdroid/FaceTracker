/*
 * merge_files.h
 *
 *  Created on: 15-Jul-2015
 *      Author: abhijat
 */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "SVM/svm.h"

#ifndef MERGE_FILES_H_
#define MERGE_FILES_H_

void output_target(double value);
void output(int index, double value);
char* readline(FILE *input, char *line);
int clean_up(FILE *fp_restore, FILE *fp, const char *msg, char *line);
void predict(svm_model* svmmodel, struct svm_node * x, double predict_label);
int svmrun (svm_model* svmmodel);



#endif /* MERGE_FILES_H_ */

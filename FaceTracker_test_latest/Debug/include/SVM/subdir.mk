################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../include/SVM/merge_files.cpp \
../include/SVM/svm.cpp 

OBJS += \
./include/SVM/merge_files.o \
./include/SVM/svm.o 

CPP_DEPS += \
./include/SVM/merge_files.d \
./include/SVM/svm.d 


# Each subdirectory must supply rules for building sources it contributes
include/SVM/%.o: ../include/SVM/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/abhijat/workspace/FaceTracker_test/include" -I/FaceTracker_test/include/FaceTracker -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



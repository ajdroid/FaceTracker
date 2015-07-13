################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/exe/face_tracker.cc 

OBJS += \
./src/exe/face_tracker.o 

CC_DEPS += \
./src/exe/face_tracker.d 


# Each subdirectory must supply rules for building sources it contributes
src/exe/%.o: ../src/exe/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/abhijat/workspace/FaceTracker_test/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



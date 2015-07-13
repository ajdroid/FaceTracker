################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/lib/CLM.cc \
../src/lib/FCheck.cc \
../src/lib/FDet.cc \
../src/lib/IO.cc \
../src/lib/PAW.cc \
../src/lib/PDM.cc \
../src/lib/Patch.cc \
../src/lib/Tracker.cc 

OBJS += \
./src/lib/CLM.o \
./src/lib/FCheck.o \
./src/lib/FDet.o \
./src/lib/IO.o \
./src/lib/PAW.o \
./src/lib/PDM.o \
./src/lib/Patch.o \
./src/lib/Tracker.o 

CC_DEPS += \
./src/lib/CLM.d \
./src/lib/FCheck.d \
./src/lib/FDet.d \
./src/lib/IO.d \
./src/lib/PAW.d \
./src/lib/PDM.d \
./src/lib/Patch.d \
./src/lib/Tracker.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/abhijat/workspace/FaceTracker_test/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



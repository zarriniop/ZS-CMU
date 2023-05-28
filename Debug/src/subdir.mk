################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/aes.c 

CPP_SRCS += \
../src/DNP3.cpp \
../src/Software_Parameter.cpp \
../src/gpio.cpp \
../src/lansocket.cpp \
../src/main.cpp \
../src/math_lib.cpp \
../src/rs232.cpp \
../src/rs485.cpp \
../src/tools.cpp 

OBJS += \
./src/DNP3.o \
./src/Software_Parameter.o \
./src/aes.o \
./src/gpio.o \
./src/lansocket.o \
./src/main.o \
./src/math_lib.o \
./src/rs232.o \
./src/rs485.o \
./src/tools.o 

C_DEPS += \
./src/aes.d 

CPP_DEPS += \
./src/DNP3.d \
./src/Software_Parameter.d \
./src/gpio.d \
./src/lansocket.d \
./src/main.d \
./src/math_lib.d \
./src/rs232.d \
./src/rs485.d \
./src/tools.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	mipsel-openwrt-linux-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	mipsel-openwrt-linux-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



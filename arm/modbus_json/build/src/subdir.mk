
# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/modbus_json_generator.c \
../src/modbus_json_sender.c 

OBJS += \
./src/main.o \
./src/modbus_json_generator.o \
./src/modbus_json_sender.o 

C_DEPS += \
./src/main.d \
./src/modbus_json_generator.d \
./src/modbus_json_sender.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	$(CC) -I$(INCLUDES) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



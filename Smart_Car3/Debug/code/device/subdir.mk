################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/device/Encoder.c" \
"../code/device/Motor_Contrl.c" \
"../code/device/Servo.c" 

COMPILED_SRCS += \
"code/device/Encoder.src" \
"code/device/Motor_Contrl.src" \
"code/device/Servo.src" 

C_DEPS += \
"./code/device/Encoder.d" \
"./code/device/Motor_Contrl.d" \
"./code/device/Servo.d" 

OBJS += \
"code/device/Encoder.o" \
"code/device/Motor_Contrl.o" \
"code/device/Servo.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/device/Encoder.src":"../code/device/Encoder.c" "code/device/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/device/Encoder.o":"code/device/Encoder.src" "code/device/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/device/Motor_Contrl.src":"../code/device/Motor_Contrl.c" "code/device/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/device/Motor_Contrl.o":"code/device/Motor_Contrl.src" "code/device/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/device/Servo.src":"../code/device/Servo.c" "code/device/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/device/Servo.o":"code/device/Servo.src" "code/device/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-device

clean-code-2f-device:
	-$(RM) ./code/device/Encoder.d ./code/device/Encoder.o ./code/device/Encoder.src ./code/device/Motor_Contrl.d ./code/device/Motor_Contrl.o ./code/device/Motor_Contrl.src ./code/device/Servo.d ./code/device/Servo.o ./code/device/Servo.src

.PHONY: clean-code-2f-device


################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/control/pid.c" 

COMPILED_SRCS += \
"code/control/pid.src" 

C_DEPS += \
"./code/control/pid.d" 

OBJS += \
"code/control/pid.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/control/pid.src":"../code/control/pid.c" "code/control/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/control/pid.o":"code/control/pid.src" "code/control/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-control

clean-code-2f-control:
	-$(RM) ./code/control/pid.d ./code/control/pid.o ./code/control/pid.src

.PHONY: clean-code-2f-control


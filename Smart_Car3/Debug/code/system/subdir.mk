################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/system/init.c" 

COMPILED_SRCS += \
"code/system/init.src" 

C_DEPS += \
"./code/system/init.d" 

OBJS += \
"code/system/init.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/system/init.src":"../code/system/init.c" "code/system/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/system/init.o":"code/system/init.src" "code/system/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-system

clean-code-2f-system:
	-$(RM) ./code/system/init.d ./code/system/init.o ./code/system/init.src

.PHONY: clean-code-2f-system


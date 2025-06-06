################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/image/camera_ctrl.c" 

COMPILED_SRCS += \
"code/image/camera_ctrl.src" 

C_DEPS += \
"./code/image/camera_ctrl.d" 

OBJS += \
"code/image/camera_ctrl.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/image/camera_ctrl.src":"../code/image/camera_ctrl.c" "code/image/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/image/camera_ctrl.o":"code/image/camera_ctrl.src" "code/image/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-image

clean-code-2f-image:
	-$(RM) ./code/image/camera_ctrl.d ./code/image/camera_ctrl.o ./code/image/camera_ctrl.src

.PHONY: clean-code-2f-image


################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/image/sources/core.c" \
"../code/image/sources/ipm.c" \
"../code/image/sources/otsu.c" \
"../code/image/sources/utils.c" 

COMPILED_SRCS += \
"code/image/sources/core.src" \
"code/image/sources/ipm.src" \
"code/image/sources/otsu.src" \
"code/image/sources/utils.src" 

C_DEPS += \
"./code/image/sources/core.d" \
"./code/image/sources/ipm.d" \
"./code/image/sources/otsu.d" \
"./code/image/sources/utils.d" 

OBJS += \
"code/image/sources/core.o" \
"code/image/sources/ipm.o" \
"code/image/sources/otsu.o" \
"code/image/sources/utils.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/image/sources/core.src":"../code/image/sources/core.c" "code/image/sources/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/image/sources/core.o":"code/image/sources/core.src" "code/image/sources/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/image/sources/ipm.src":"../code/image/sources/ipm.c" "code/image/sources/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/image/sources/ipm.o":"code/image/sources/ipm.src" "code/image/sources/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/image/sources/otsu.src":"../code/image/sources/otsu.c" "code/image/sources/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/image/sources/otsu.o":"code/image/sources/otsu.src" "code/image/sources/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/image/sources/utils.src":"../code/image/sources/utils.c" "code/image/sources/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/infineon/Project/Smart_Car3/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/image/sources/utils.o":"code/image/sources/utils.src" "code/image/sources/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-image-2f-sources

clean-code-2f-image-2f-sources:
	-$(RM) ./code/image/sources/core.d ./code/image/sources/core.o ./code/image/sources/core.src ./code/image/sources/ipm.d ./code/image/sources/ipm.o ./code/image/sources/ipm.src ./code/image/sources/otsu.d ./code/image/sources/otsu.o ./code/image/sources/otsu.src ./code/image/sources/utils.d ./code/image/sources/utils.o ./code/image/sources/utils.src

.PHONY: clean-code-2f-image-2f-sources


cmake_minimum_required(VERSION 3.15...3.30)

set(MCU attiny85)
set(F_CPU 16000000)
set(PROG_TYPE usbtiny)

add_compile_definitions(F_CPU=${F_CPU})

set(AVRC avr-gcc)
set(AVRCPP avr-g++)
set(AVRSTRIP avr-strip)
set(OBJCOPY avr-objcopy)
set(OBJDUMP avr-objdump)
set(AVRSIZE avr-size)
set(AVRDUDE avrdude)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER ${AVRC})
set(CMAKE_ASM_COMPILER ${AVRC})

project(pwm-fan-ctrl VERSION 1.0.1 LANGUAGES C ASM)

add_compile_options(-Os -mmcu=${MCU} -DF_CPU=${F_CPU}
                    --param=min-pagesize=0 -std=gnu99 -pedantic-errors
                    -Wall -Wextra -Wundef -Wno-strict-prototypes
                    -Wno-gnu-zero-variadic-macro-arguments -Wno-unknown-attributes
                    -funsigned-char -funsigned-bitfields -fpack-struct
                    -fshort-enums -ffunction-sections -fdata-sections)

add_link_options(-mmcu=${MCU})

set(SRC_FILES src/main.c)

set(PN ${PROJECT_NAME})
set(HEX_FLAGS -j .text -j .data -O ihex)
set(FLASH_FLAGS -c ${PROG_TYPE} -p ${MCU})

add_executable(${PN} ${SRC_FILES})
set_target_properties(${PN} PROPERTIES OUTPUT_NAME "${PN}.elf")

add_custom_target(strip ALL ${AVRSTRIP} "${PN}.elf" DEPENDS ${PN})
add_custom_target(hex ALL ${OBJCOPY} -O ihex -j.text -j.data "${PN}.elf" "${PN}.hex" DEPENDS strip)
add_custom_target(flash ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PN}.hex:i DEPENDS hex)

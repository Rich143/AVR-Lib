# lib/Makefile
include lib_std_defs.mk

CC := avr-gcc
MODULES = TWI/TWI LCD_Driver/LCD_Driver Uart/uart Debug/Debug
LD := avr-ld
LDFLAGS := -r
AR = avr-ar


#libs.la: $(MODULES).o
#	$(LD) $(LDFLAGS) -o $(output) $(inputs)

libs.a: $(MODULES).o
	$(AR) rcs $(output) $(inputs)

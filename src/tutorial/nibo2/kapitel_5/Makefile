
# Configuration
TARGET = kapitel_5
DEVICE = atmega128
F_CPU = 16000000

OBJS = \
    main.o \



# AVR compiler setup
PREFIX =
CC = $(PREFIX)avr-gcc
OBJCOPY = $(PREFIX)avr-objcopy
OBJDUMP = $(PREFIX)avr-objdump


# AVR compiler and linker flags
CFLAGS += -Os -ffunction-sections -DAVR -I../../../../include -mmcu=$(DEVICE) -std=c99
CFLAGS += -DF_CPU=$(F_CPU) -DVERSION="\"$(VERSION)\"" -D_NIBO_2_
CLDFLAGS += -v -mmcu=$(DEVICE) -L../../../../lib

# MATH lib
MATH = -lm -Wl,-u,vfprintf -lprintf_flt
#MATH = -lm
#MATH =

# Nibo library
LIBS = -lnibo2

# build intel hex files
all: $(TARGET).hex

main.o: main.c

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

%.d:%.c
	set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@ ; \
	[ -s $@ ] || rm -f $@

# avr specific entries
%.elf: $(OBJS)
	$(CC) $(CLDFLAGS) -o $@ $(OBJS) $(LIBS) $(MATH)
	avr-size -A $(TARGET).elf

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(TARGET).lss: $(TARGET).elf
	avr-objdump -h -S $(TARGET).elf >$(TARGET).lss

lss: $(TARGET).lss


avrdude: $(TARGET).hex
	avrdude -c stk500v2 -p m128 -B 1 -U lfuse:w:0xff:m -U hfuse:w:0xc1:m -U efuse:w:0xff:m -U flash:w:$(TARGET).hex

clean:
	rm -f *.d *.o *~ *.elf $(TARGET).hex $(TARGET).lss

include $(OBJS:.o=.d)

.PHONY: clean
.SECONDARY: $(TARGET).hex $(OBJS)


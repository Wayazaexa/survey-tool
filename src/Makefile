CC=/home/randomprat/tools/avr-gcc-12.1.0-x64-linux/bin/avr-gcc
LD=/home/randomprat/tools/avr-gcc-12.1.0-x64-linux/bin/avr-ld
OBJCOPY="/home/randomprat/tools/avr-gcc-12.1.0-x64-linux/bin/avr-objcopy"
OBJDUMP="/home/randomprat/tools/avr-gcc-12.1.0-x64-linux/bin/avr-objdump"
AVRSIZE="/home/randomprat/tools/avr-gcc-12.1.0-x64-linux/bin/avr-size"
OBJISP="C:\avr\bin\avrdude"
MCU=atmega328p
CFLAGS=-Wall -Wextra  -Wundef -pedantic \
		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU} -DBAUD=115200
LDFLAGS=-mmcu=$(MCU)
PORT=\\\\.\\COM3
BIN=../survey-tool
OUT=${BIN}.hex
SOURCES = main.c uart.c ESP8266.c lcd.c

DEBUG?=1

GTEST = gtest
LIBGTEST = C:\msys64\mingw64\lib\libgtest_main.a C:\msys64\mingw64\lib\libgtest.a
TEST=check.exe


ifeq ($(DEBUG), 1)
	OUTPUTDIR=../bin/debug
else
	OUTPUTDIR=../bin/release
endif

OBJS =  $(addprefix $(OUTPUTDIR)/,$(SOURCES:.c=.o))

all: $(OUTPUTDIR)  $(OUT) 

$(OBJS): Makefile

$(OUTPUTDIR)/%.o:%.c
	$(CC) $(CFLAGS) -MD -o $@ -c $<

%.lss: %.elf
	$(OBJDUMP) -h -S -s $< > $@

%.elf: $(OBJS)
	$(CC) -Wl,-Map=$(@:.elf=.map) $(LDFLAGS) -o $@ $^
	$(AVRSIZE) $@


$(OBJS):$(SOURCES)

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@

isp: ${BIN}.hex
	$(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$<


clean:
	del "$(OUT)"  *.map *.P *.d

$(OUTPUTDIR): 
	@mkdir $(OUTPUTDIR)

$(TEST): TestFetchAndSend.cpp
	g++ -o $@ $^ -DF_CPU=16000000UL -I $(GTEST)  $(LIBGTEST)	





test: $(TEST)
	./$(TEST)
		   	
.PHONY: clean dirs

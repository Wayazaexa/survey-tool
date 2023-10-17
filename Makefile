## ==== Uncomment for Linux ==== ##
CC      := avr-gcc
LD      := avr-ld
OBJCOPY := avr-objcopy
OBJDUMP := avr-objdump
AVRSIZE := avr-size
OBJISP  := avrdude
#PORT    := /dev/ttyS0
## ============================= ##
## ---- Uncomment for Windows ---- ##
#CC      := C:\avr\bin\avr-gcc
#LD      := C:\avr\bin\avr-ld
#OBJCOPY := C:\avr\bin\avr-objcopy
#OBJDUMP := C:\avr\bin\avr-objdump
#AVRSIZE := C:\avr\bin\avr-size
#OBJISP  := C:\avr\bin\avrdude
#PORT    := \\\\.\\COM3
## ------------------------------- ##
MCU      := atmega328p
CPPFLAGS := -Iinclude
CFLAGS   := -Wall -Wextra  -Wundef -pedantic -Os \
			-std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU} -DBAUD=115200
LDFLAGS  := -mmcu=$(MCU)

SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj

BIN := survey-tool.hex
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: clean isp all

all: $(BIN_DIR)/$(BIN)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MD -o $@ -c $<

%.lss: %.elf
	$(OBJDUMP) -h -S -s $< > $@

%.elf: $(OBJ)
	$(CC) -Wl,-Map=$(@:.elf=.map) $(LDFLAGS) -o $@ $^
	$(AVRSIZE) $@
	@mv $(BIN_DIR)/*.map $(OBJ_DIR)

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@

#isp: ${BIN}
#	$(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$<


clean:
	@rm -rv $(BIN_DIR)/$(BIN) $(OBJ_DIR)

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

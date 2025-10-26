TARGET = patches.elf

all: $(TARGET)

$(TARGET):
	@[ -f $(TARGET) ] || touch $(TARGET)

patches.bin:
	@[ -f patches.bin ] || touch patches.bin

clean:
	rm -f $(TARGET) patches.bin

.PHONY: all clean

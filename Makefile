VERSION ?= PAL
BASENAME := main
TC       := tools/toolchain/bin
PY       := .venv/bin/python
AS       := $(TC)/mipsel-none-elf-as
LD       := $(TC)/mipsel-none-elf-ld
OBJCOPY  := $(TC)/mipsel-none-elf-objcopy
BUILD    := build/$(VERSION)
CFG      := configs/$(VERSION)/$(BASENAME).yaml
LD_SCRIPT:= linkers/$(VERSION)/$(BASENAME).ld
TARGET   := assets/$(VERSION)/main.exe
ELF      := $(BUILD)/main.elf
OUT      := $(BUILD)/main.exe
SHA      := $(shell cut -d' ' -f1 configs/$(VERSION)/checksum.sha)

ASM_SRCS := $(shell find asm/$(VERSION) -name '*.s' -not -path '*/nonmatchings/*' 2>/dev/null)
C_SRCS   := $(shell find src/main/$(VERSION) -name '*.c' 2>/dev/null)
HEADERS  := $(shell find include -name '*.h' 2>/dev/null)
MASPSX_SRCS := tools/maspsx/maspsx.py $(wildcard tools/maspsx/maspsx/*.py)
OBJS     := $(ASM_SRCS:%.s=$(BUILD)/%.s.o) $(C_SRCS:%.c=$(BUILD)/%.c.o)

.PHONY: all split build check clean
all: check

split:
	$(PY) tools/scripts/gen_config.py $(VERSION)
	$(PY) -m splat split $(CFG)
	$(PY) tools/scripts/normalize_bios_stubs.py $(VERSION)

$(BUILD)/%.s.o: %.s include/macro.inc include/bios_calls.inc include/gte_macros.inc
	@mkdir -p $(dir $@)
	$(AS) -EL -G0 -march=r3000 -no-pad-sections -Iinclude -o $@ $<

$(BUILD)/%.c.o: %.c $(HEADERS) tools/scripts/cc.sh $(MASPSX_SRCS)
	@mkdir -p $(dir $@)
	tools/scripts/cc.sh $< $@

$(ELF): $(OBJS) $(LD_SCRIPT)
	$(LD) -EL -T $(LD_SCRIPT) -T linkers/$(VERSION)/undefined_syms_auto.$(BASENAME).txt -T linkers/$(VERSION)/undefined_funcs_auto.$(BASENAME).txt -Map $(BUILD)/main.map -o $@

# splat's header segment already carries the 0x800-byte PS-X EXE header
$(OUT): $(ELF)
	$(OBJCOPY) -O binary $< $@

build: $(OUT)

check: build
	@echo "$(SHA)  $(OUT)" | shasum -c -

clean:
	rm -rf $(BUILD)

debt:
	.venv/bin/python tools/scripts/crutch_debt.py
debt-check:
	.venv/bin/python tools/scripts/crutch_debt.py --check
.PHONY: debt debt-check

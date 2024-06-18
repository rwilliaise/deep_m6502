
#ifndef PRINTER_H_
#define PRINTER_H_

#include "m6502.h"
#include "m6522.h"

typedef struct {
	m6502_t cpu;
	m6522_t via;

	uint64_t pins;
	uint8_t ram[0x6000];
	uint8_t rom[0x8000];
	bool last_pb0;
} printer_t;

void printer_init(printer_t *sys);
void printer_tick(printer_t *sys);

int printer_load(printer_t *sys, const char *filename);
int printer_exec(printer_t *sys);

#ifdef CHIPS_IMPL

#include <stdio.h>

#define MAX_CYCLES 1<<12
#define RST_CYCLES 5

void printer_init(printer_t *sys) {
	sys->pins = m6502_init(&sys->cpu, &(m6502_desc_t) {});
	m6522_init(&sys->via);
}

int printer_load(printer_t *sys, const char *filename) {
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) return 1;
	fread(sys->rom, sizeof(*sys->rom), sizeof(sys->rom), fp);
	fclose(fp);
	return 0;
}

void printer_tick(printer_t *sys) {
	uint64_t pins = m6502_tick(&sys->cpu, sys->pins);
	uint64_t via_pins = pins & M6502_PIN_MASK;

	uint16_t addr = M6502_GET_ADDR(pins);
	if (addr < 0x6000) { // ram
		if (pins & M6502_RW) {
			M6502_SET_DATA(pins, sys->ram[addr]);
		} else {
			sys->ram[addr] = M6502_GET_DATA(pins);
		}
	} else if (addr < 0x8000) { // via
		via_pins |= M6522_CS1;
	} else { // rom
		if (pins & M6502_RW) {
			M6502_SET_DATA(pins, sys->rom[addr - 0x8000]);
		}
	}

	via_pins = m6522_tick(&sys->via, via_pins);

	// printing
	bool pb0 = (via_pins & M6522_PB0) != 0;
	if (pb0 && (pb0 != sys->last_pb0)) {
		putc(M6522_GET_PA(via_pins), stdout);
	}
	sys->last_pb0 = pb0;

	if (via_pins & M6522_IRQ) {
		pins |= M6502_NMI;
	}
	if ((via_pins & (M6522_CS1|M6522_RW)) == (M6522_CS1|M6522_RW)) {
		pins = M6502_COPY_DATA(pins, via_pins);
	}

	sys->pins = pins;
}

int printer_exec(printer_t *sys) {
	uint32_t cycles = 0;

	while (!(sys->cpu.P & 0x4) || cycles <= RST_CYCLES) {
		if (cycles == RST_CYCLES) sys->cpu.P &= ~0x4;
		if (cycles++ >= MAX_CYCLES) return -1;
		printer_tick(sys);
	}
	return sys->cpu.A;
}

#endif // CHIPS_IMPL

#endif // PRINTER_H_


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <linux/limits.h>

#define CHIPS_IMPL
#include "m6502.h"

#define MAX_CYCLES 1<<12
#define RST_CYCLES 5

static char path[PATH_MAX];

static const char *pathcat(const char *A, const char *B) {
	*path = 0;
	strncat(path, A, PATH_MAX - 1);
	strncat(path, B, PATH_MAX - strlen(path) - 1);
	return path;
}

static int runbin(const char *filename, char **failmsg) {
	uint8_t mem[1<<16] = {};
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) return 1;
	fread(mem, sizeof(*mem), sizeof(mem), fp);
	fclose(fp);

	m6502_t cpu;
	uint64_t pins = m6502_init(&cpu, &(m6502_desc_t) {});
	uint32_t cycles = 0;

	// return with interrupt disable flag (sei)
	while (!(cpu.P & 0x4) || cycles <= RST_CYCLES) {
		if (cycles == RST_CYCLES) cpu.P &= ~0x4;
		if (cycles++ >= MAX_CYCLES) {
			*failmsg = "exhausted cycle time";
			return 1;
		}
		pins = m6502_tick(&cpu, pins);
		const uint16_t addr = M6502_GET_ADDR(pins);
		if (pins & M6502_RW) {
			M6502_SET_DATA(pins, mem[addr]);
		} else {
			mem[addr] = M6502_GET_DATA(pins);
		}
	}
	return cpu.A;
}

int main(int argc, char *argv[]) {
	if (argc <= 1) return 1;

	for (int i = 1; i < argc; i++) {
		char *target = argv[i];
		char *failmsg = "";
		int status;
		FILE *fp = fopen(target, "rb");
		if (fp == NULL) { 
			status = runbin(pathcat(target, ".bin"), &failmsg);
		} else {
			fclose(fp);
			FILE *p = popen(pathcat("./", target), "r");
			if (p == NULL) {
				printf("[FAIL] %s - does not exist!\n", target);
				continue;
			}
			char buffer[128];
			while (!feof(p)) {
				if (fgets(buffer, sizeof(buffer), p) != NULL)
					fputs(buffer, stdout);
			}
			status = pclose(p);
		}
		if (status == 0) {
			printf("[ OK ] %s\n", target);
		} else {
			printf("[FAIL] %s - rv: %d %s\n", target, status, failmsg);
		}
	}

	return 0;
}


#define CHIPS_IMPL
#include "printer.h"

int main(void) {
	printer_t sys;
	printer_init(&sys);
	if (printer_load(&sys, "printer02.bin")) return -2;
	return printer_exec(&sys);
}


#include <stdlib.h>
#include <stdio.h>

#include <mee/cpu.h>
#include <mee/pmp.h>

#define ECODE_STORE_FAULT	7

/* Mark the variable volatile to ensure that we access the memory location
 * on write and align to 4 bytes for PMP */
volatile int protected_global __attribute__((aligned(4)));

void store_access_fault_handler(struct mee_cpu *cpu, int ecode)
{
	/* Get faulting instruction and instruction length */
	unsigned long epc = mee_cpu_get_exception_pc(cpu);
	int inst_len = mee_cpu_get_instruction_length(cpu, epc);

	/* Advance the exception program counter by the length of the
	 * instruction to return execution after the faulting store */
	mee_cpu_set_exception_pc(cpu, epc + inst_len);

	if(ecode == ECODE_STORE_FAULT) {
		printf("Exception: Store Access Fault at Instruction 0x%04x\n", epc);	
	} else {
		printf("Spurious Exception %d at Instruction 0x%04x\n", ecode, epc);

		exit(7);
	}
}

int main()
{
	int rc;
	struct mee_cpu *cpu0;
	struct mee_interrupt *cpu_intr;
	struct mee_pmp *pmp;

	/* PMP addresses are 4-byte aligned, drop the bottom two bits */
	size_t protected_addr = ((size_t) &protected_global) >> 2;

	printf("PMP Driver Example\n");

	/* Initialize interrupt handling on CPU 0 */
	cpu0 = mee_cpu_get(0);
	if(!cpu0) {
		printf("Unable to get CPU 0 handle\n");
		return 1;
	}
	cpu_intr = mee_cpu_interrupt_controller(cpu0);
	if(!cpu_intr) {
		printf("Unable to get CPU 0 Interrupt handle\n");
		return 2;
	}
	mee_interrupt_init(cpu_intr);

	/* Register a handler for the store access fault exception */
	rc = mee_cpu_exception_register(cpu0, ECODE_STORE_FAULT, store_access_fault_handler);
	if(rc < 0) {
		printf("Failed to register exception handler %d\n");
		return 3;
	}

	/* Initialize PMPs */
	pmp = mee_pmp_get_device();
	if(!pmp) {
		printf("Unable to get PMP Device\n");
		return 4;
	}
	mee_pmp_init(pmp);

	/* Perform a write to the memory we're about to protect access to */
	protected_global = 0;

	/* Configure PMP 0 to only allow reads to protected_global. The
	 * PMP region is locked so that the configuration applies to M-mode
	 * accesses. */
	struct mee_pmp_config config = {
		.L = MEE_PMP_LOCKED,
		.A = MEE_PMP_NA4, /* Naturally-aligned 4-byte region */
		.X = 0,
		.W = 0,
		.R = 1,
	};
	rc = mee_pmp_set_region(pmp, 0, config, protected_addr);
	if(rc != 0) {
		printf("Failed to configure PMP 0: %d\n", rc);
		return 5;
	}

	printf("Attempting to write to protected address\n");

	/* Attempt to write to protected_global. This should generate a store
	 * access fault exception. */
	protected_global = 6;

	/* If execution returns to here, print and return the value of protected
	 * global to demonstrate that we can still read the value */
	printf("The value of the protected memory is %d\n", protected_global);

	/* If the write succeeds, the test fails */
	return protected_global;
}

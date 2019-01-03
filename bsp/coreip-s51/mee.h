#ifndef ASSEMBLY

#ifndef COREIP_S51__MEE_H
#define COREIP_S51__MEE_H

#define __MEE_CLINT_2000000_INTERRUPTS 		2
#define MEE_MAX_CLINT_INTERRUPTS	 __MEE_CLINT_2000000_INTERRUPTS

#define __MEE_INTERRUPT_CONTROLLER_C000000_INTERRUPTS 		1
#define MEE_MAX_PLIC_INTERRUPTS	 __MEE_INTERRUPT_CONTROLLER_C000000_INTERRUPTS

#include <mee/drivers/riscv,cpu.h>
#include <mee/drivers/riscv,clint0.h>
#include <mee/drivers/riscv,plic0.h>
#include <mee/drivers/sifive,test0.h>
#include <mee/drivers/riscv,rv64,pmp.h>
/* From pmp@0 */
asm (".weak __mee_dt_pmp_0");
struct __mee_driver_riscv_rv64_pmp __mee_dt_pmp_0;

/* From cpu@0 */
asm (".weak __mee_dt_cpu_0");
struct __mee_driver_cpu __mee_dt_cpu_0;

/* From clint@2000000 */
asm (".weak __mee_dt_clint_2000000");
struct __mee_driver_riscv_clint0 __mee_dt_clint_2000000;

/* From interrupt_controller@c000000 */
asm (".weak __mee_dt_interrupt_controller_c000000");
struct __mee_driver_riscv_plic0 __mee_dt_interrupt_controller_c000000;

/* From interrupt_controller */
asm (".weak __mee_dt_interrupt_controller");
struct __mee_driver_riscv_cpu_intc __mee_dt_interrupt_controller;

/* From teststatus@4000 */
asm (".weak __mee_dt_teststatus_4000");
struct __mee_driver_sifive_test0 __mee_dt_teststatus_4000;

/* From cpu@0 */
struct __mee_driver_cpu __mee_dt_cpu_0 = {
    .vtable = &__mee_driver_vtable_cpu,
    .cpu.vtable = &__mee_driver_vtable_cpu.cpu_vtable,
    .timebase = 1000000UL,
    .interrupt_controller = &__mee_dt_interrupt_controller.controller,
};

/* From cpu@0 */
#define __MEE_DT_RISCV_CPU_HANDLE (&__mee_dt_cpu_0.cpu)

#define __MEE_DT_CPU_0_HANDLE (&__mee_dt_cpu_0.cpu)

/* From interrupt_controller */
struct __mee_driver_riscv_cpu_intc __mee_dt_interrupt_controller = {
    .vtable = &__mee_driver_vtable_riscv_cpu_intc,
    .controller.vtable = &__mee_driver_vtable_riscv_cpu_intc.controller_vtable,
    .init_done = 0,
    .interrupt_controller = 1,
};

/* From interrupt_controller */
#define __MEE_DT_RISCV_CPU_INTC_HANDLE (&__mee_dt_interrupt_controller.controller)

#define __MEE_DT_INTERRUPT_CONTROLLER_HANDLE (&__mee_dt_interrupt_controller.controller)

/* From clint@2000000 */
struct __mee_driver_riscv_clint0 __mee_dt_clint_2000000 = {
    .vtable = &__mee_driver_vtable_riscv_clint0,
    .clint.vtable = &__mee_driver_vtable_riscv_clint0.clint_vtable,
    .control_base = 33554432UL,
    .control_size = 65536UL,
    .init_done = 0,
    .num_interrupts = MEE_MAX_CLINT_INTERRUPTS,
    .interrupt_parent = &__mee_dt_interrupt_controller.controller,
    .interrupt_lines[0] = 3,
    .interrupt_lines[1] = 7,
};

/* From clint@2000000 */
#define __MEE_DT_RISCV_CLINT0_HANDLE (&__mee_dt_clint_2000000.clint)

#define __MEE_DT_CLINT_2000000_HANDLE (&__mee_dt_clint_2000000.clint)

/* From interrupt_controller@c000000 */
struct __mee_driver_riscv_plic0 __mee_dt_interrupt_controller_c000000 = {
    .vtable = &__mee_driver_vtable_riscv_plic0,
    .plic0.vtable = &__mee_driver_vtable_riscv_plic0.controller_vtable,
    .init_done = 0,
/* From interrupt_controller */
    .interrupt_parent = &__mee_dt_interrupt_controller.controller,
    .interrupt_line = 11UL,
    .control_base = 201326592UL,
    .control_size = 67108864UL,
    .max_priority = 7UL,
    .num_interrupts = 255UL,
    .interrupt_controller = 1,
};

/* From interrupt_controller@c000000 */
#define __MEE_DT_RISCV_PLIC0_HANDLE (&__mee_dt_interrupt_controller_c000000.plic0)

#define __MEE_DT_INTERRUPT_CONTROLLER_C000000_HANDLE (&__mee_dt_interrupt_controller_c000000.plic0)

/* From pmp@0 */
struct __mee_driver_riscv_rv64_pmp __mee_dt_pmp_0 = {
    .vtable = &__mee_driver_vtable_riscv_rv64_pmp,
    .pmp.vtable = &__mee_driver_vtable_riscv_rv64_pmp.pmp,
    .num_regions = 8UL,
};

/* From pmp@0 */
#define __MEE_DT_PMP_HANDLE (&__mee_dt_pmp_0.pmp)

#define __MEE_DT_PMP_0_HANDLE (&__mee_dt_pmp_0.pmp)

/* From teststatus@4000 */
struct __mee_driver_sifive_test0 __mee_dt_teststatus_4000 = {
    .vtable = &__mee_driver_vtable_sifive_test0,
    .shutdown.vtable = &__mee_driver_vtable_sifive_test0.shutdown,
    .base = 16384UL,
    .size = 4096UL,
};

/* From teststatus@4000 */
#define __MEE_DT_SHUTDOWN_HANDLE (&__mee_dt_teststatus_4000.shutdown)

#define __MEE_DT_TESTSTATUS_4000_HANDLE (&__mee_dt_teststatus_4000.shutdown)

#define __MEE_DT_MAX_HARTS 1

asm (".weak __mee_cpu_table");
struct __mee_driver_cpu *__mee_cpu_table = {
					&__mee_dt_cpu_0};

#endif /*MEE__MACHINE__COREIP_S51__MEE_H*/

#endif/*ASSEMBLY*/

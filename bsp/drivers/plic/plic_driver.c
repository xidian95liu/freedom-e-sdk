// See LICENSE for license details.

#include "sifive/devices/plic.h"
#include "plic/plic_driver.h"
#include "platform.h"
#include "encoding.h"
#include <string.h>


// Note that there are no assertions or bounds checking on these
// parameter values.

void volatile_memzero(uint8_t * base, unsigned int size)
{
  volatile uint8_t * ptr;
  for (ptr = base; ptr < (base + size); ptr++){
    *ptr = 0;
  }
}//拿到基地址，通过循环对基地址尺寸范围内所有地址全部赋零

void PLIC_init (
                plic_instance_t * this_plic,//指针，指向plic_instance_t该结构体的一个指针变量
                uintptr_t         base_addr,//plic的基地址
                uint32_t num_sources,//中断源
                uint32_t num_priorities//优先级信息
                )
{
  
  this_plic->base_addr = base_addr; //-> 指向结构体成员
  this_plic->num_sources = num_sources;
  this_plic->num_priorities = num_priorities;
  
  // Disable all interrupts (don't assume that these registers are reset).
  unsigned long hart_id = read_csr(mhartid); //关闭所有中断 从寄存器读处理器ID
  volatile_memzero((uint8_t*) (this_plic->base_addr +
                               PLIC_ENABLE_OFFSET +
                               (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET)),
                   (num_sources + 8) / 8);//拿到enable 的base地址 与base地址size 进行初始化置零
  
  // Set all priorities to 0 (equal priority -- don't assume that these are reset).
  volatile_memzero ((uint8_t *)(this_plic->base_addr +
                                PLIC_PRIORITY_OFFSET),
                    (num_sources + 1) << PLIC_PRIORITY_SHIFT_PER_SOURCE);//拿到优先级地址，与size进行初始化置零

  // Set the threshold to 0.
  volatile plic_threshold* threshold = (plic_threshold*)
    (this_plic->base_addr +
     PLIC_THRESHOLD_OFFSET +
     (hart_id << PLIC_THRESHOLD_SHIFT_PER_TARGET));//拿到threshold地址，与size进行初始化置零

  *threshold = 0;
  
}

void PLIC_set_threshold (plic_instance_t * this_plic,//操作threshold threshold_ptr为其地址
			 plic_threshold threshold){

  unsigned long hart_id = read_csr(mhartid);  
  volatile plic_threshold* threshold_ptr = (plic_threshold*) (this_plic->base_addr +
                                                              PLIC_THRESHOLD_OFFSET +
                                                              (hart_id << PLIC_THRESHOLD_SHIFT_PER_TARGET));

  *threshold_ptr = threshold;

}
  

void PLIC_enable_interrupt (plic_instance_t * this_plic, plic_source source){

  unsigned long hart_id = read_csr(mhartid);
  volatile uint8_t * current_ptr = (volatile uint8_t *)(this_plic->base_addr +
                                                        PLIC_ENABLE_OFFSET +
                                                        (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET) +
                                                        (source >> 3));//??
  uint8_t current = *current_ptr;
  current = current | ( 1 << (source & 0x7));
  *current_ptr = current;

}//set enable

void PLIC_disable_interrupt (plic_instance_t * this_plic, plic_source source){
  
  unsigned long hart_id = read_csr(mhartid);
  volatile uint8_t * current_ptr = (volatile uint8_t *) (this_plic->base_addr +
                                                         PLIC_ENABLE_OFFSET +
                                                         (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET) +
                                                         (source >> 3));
  uint8_t current = *current_ptr;
  current = current & ~(( 1 << (source & 0x7)));
  *current_ptr = current;
  
}

void PLIC_set_priority (plic_instance_t * this_plic, plic_source source, plic_priority priority){

  if (this_plic->num_priorities > 0) {
    volatile plic_priority * priority_ptr = (volatile plic_priority *)//指向优先级的指针
      (this_plic->base_addr +
       PLIC_PRIORITY_OFFSET +
       (source << PLIC_PRIORITY_SHIFT_PER_SOURCE));
    *priority_ptr = priority;
  }
}

plic_source PLIC_claim_interrupt(plic_instance_t * this_plic){//返回PLIC 发出claim的中断源
  
  unsigned long hart_id = read_csr(mhartid);

  volatile plic_source * claim_addr = (volatile plic_source * )
    (this_plic->base_addr +
     PLIC_CLAIM_OFFSET +
     (hart_id << PLIC_CLAIM_SHIFT_PER_TARGET));

  return  *claim_addr;
  
}

void PLIC_complete_interrupt(plic_instance_t * this_plic, plic_source source){
  
  unsigned long hart_id = read_csr(mhartid);
  volatile plic_source * claim_addr = (volatile plic_source *) (this_plic->base_addr +
                                                                PLIC_CLAIM_OFFSET +
                                                                (hart_id << PLIC_CLAIM_SHIFT_PER_TARGET));
  *claim_addr = source;
  
}


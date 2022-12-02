/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __XEN_PMU_H
#define __XEN_PMU_H

#include <xen/interface/xenpmu.h>

<<<<<<< HEAD
=======
extern bool is_xen_pmu;

>>>>>>> rebase
irqreturn_t xen_pmu_irq_handler(int irq, void *dev_id);
#ifdef CONFIG_XEN_HAVE_VPMU
void xen_pmu_init(int cpu);
void xen_pmu_finish(int cpu);
#else
static inline void xen_pmu_init(int cpu) {}
static inline void xen_pmu_finish(int cpu) {}
#endif
<<<<<<< HEAD
bool is_xen_pmu(int cpu);
=======
>>>>>>> rebase
bool pmu_msr_read(unsigned int msr, uint64_t *val, int *err);
bool pmu_msr_write(unsigned int msr, uint32_t low, uint32_t high, int *err);
int pmu_apic_update(uint32_t reg);
unsigned long long xen_read_pmc(int counter);

#endif /* __XEN_PMU_H */

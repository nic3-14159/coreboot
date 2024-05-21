/* SPDX-License-Identifier: GPL-2.0-only */

#include <console/console.h>
#include <commonlib/bsd/clamp.h>
#include <cf9_reset.h>
#include <device/pci_ops.h>
#include <delay.h>
#include <romstage_handoff.h>
#include "sandybridge.h"
#include <arch/romstage.h>
#include <device/pci_def.h>
#include <device/device.h>
#include <device/mmio.h>
#include <northbridge/intel/sandybridge/chip.h>
#include <security/intel/txt/txt.h>
#include <security/intel/txt/txt_platform.h>
#include <security/intel/txt/txt_register.h>
#include <southbridge/intel/bd82x6x/pch.h>
#include <southbridge/intel/common/pmclib.h>
#include <elog.h>
#include <pc80/vga.h>
#include <cpu/x86/mtrr.h>
#include <cpu/x86/msr.h>
#include "early_graphics.h"
#include "raminit_common.h"
#include <cpu/intel/model_206ax/model_206ax.h>
#include <southbridge/intel/bd82x6x/me.h>

__weak void mainboard_early_init(int s3resume)
{
}

__weak void mainboard_late_rcba_config(void)
{
}

static void wait_txt_clear(void)
{
	struct cpuid_result cp = cpuid_ext(1, 0);

	/* Check if TXT is supported */
	if (!(cp.ecx & (1 << 6)))
		return;

	/* Some TXT public bit */
	if (!(read32p(0xfed30010) & 1))
		return;

	/* Wait for TXT clear */
	while (!(read8p(0xfed40000) & (1 << 7)))
		;
}

static void configure_dpr(void)
{
	union dpr_register dpr = txt_get_chipset_dpr();

	/*
	 * Just need to program the size of DPR, enable and lock it.
	 * The dpr.top will always point to TSEG_BASE (updated by hardware).
	 * We do it early because it will be needed later to calculate cbmem_top.
	 */
	dpr.lock = 1;
	dpr.epm = 1;
	dpr.size = CONFIG_INTEL_TXT_DPR_SIZE;
	pci_write_config32(HOST_BRIDGE, DPR, dpr.raw);
}

static void early_pch_reset_pmcon(void)
{
	/* Reset RTC power status */
	pci_and_config8(PCH_LPC_DEV, GEN_PMCON_3, ~(1 << 2));
}

#define SNB_MIN_DCLK_133_MULT	3
#define SNB_MAX_DCLK_133_MULT	8
#define IVB_MIN_DCLK_133_MULT	3
#define IVB_MAX_DCLK_133_MULT	10
#define IVB_MIN_DCLK_100_MULT	7
#define IVB_MAX_DCLK_100_MULT	12

/* Frequency multiplier */
static u32 get_FRQ(const ramctr_timing *ctrl)
{
	const u32 FRQ = 256000 / (ctrl->tCK * ctrl->base_freq);

	if (IS_IVY_CPU(ctrl->cpu)) {
		if (ctrl->base_freq == 100)
			return clamp_u32(IVB_MIN_DCLK_100_MULT, FRQ, IVB_MAX_DCLK_100_MULT);

		if (ctrl->base_freq == 133)
			return clamp_u32(IVB_MIN_DCLK_133_MULT, FRQ, IVB_MAX_DCLK_133_MULT);

	} else if (IS_SANDY_CPU(ctrl->cpu)) {
		if (ctrl->base_freq == 133)
			return clamp_u32(SNB_MIN_DCLK_133_MULT, FRQ, SNB_MAX_DCLK_133_MULT);
	}

	die("Unsupported CPU or base frequency.");
}

static void dram_freq(void)
{
	ramctr_timing ctrl;
	ctrl.cpu = cpu_get_cpuid();
	ctrl.base_freq = 133;
	ctrl.tCK = TCK_800MHZ;
	ctrl.FRQ = get_FRQ(&ctrl);

		u8 val2;
		u32 reg1 = 0;

		/*
		 * The MPLL will never lock if the requested frequency is already set.
		 * Exit early to prevent a system hang.
		 */
		reg1 = mchbar_read32(MC_BIOS_DATA);
		val2 = (u8)reg1;
		if (val2)
			return;

		/* Step 2 - Request MPLL frequency through the PCU */
		reg1 = ctrl.FRQ;

		reg1 |= (1 << 31);	/* Set running bit */
		mchbar_write32(MC_BIOS_REQ, reg1);
		int i = 0;
		printk(BIOS_DEBUG, "MPLL busy... ");
		while (reg1 & (1 << 31)) {
			udelay(10);
			i++;
			reg1 = mchbar_read32(MC_BIOS_REQ);
		}
		printk(BIOS_DEBUG, "done in %d us\n", i * 10);

		/* Step 3 - Verify lock frequency */
		reg1 = mchbar_read32(MC_BIOS_DATA);
		val2 = (u8)reg1;
		printk(BIOS_DEBUG, "MC_BIOS_DATA: 0x%x, ctrl.FRQ: %d\n", val2, ctrl.FRQ);
		if (val2 >= ctrl.FRQ) {
			printk(BIOS_DEBUG, "MPLL frequency is set at : %d MHz\n",
			       (1000 << 8) / ctrl.tCK);
			return;
		}
		printk(BIOS_DEBUG, "MPLL didn't lock. Retrying at lower frequency\n");
}

/* The romstage entry point for this platform is not mainboard-specific, hence the name */
void mainboard_romstage_entry(void)
{
	int s3resume = 0;

	if (mchbar_read16(SSKPD_HI) == 0xcafe)
		system_reset();

	/* Init LPC, GPIO, BARs, disable watchdog ... */
	early_pch_init();

	/* When using MRC, USB is initialized by MRC */
	if (CONFIG(USE_NATIVE_RAMINIT)) {
		early_usb_init(mainboard_usb_ports);
	}

	/* Perform some early chipset init needed before RAM initialization can work */
	systemagent_early_init();
	printk(BIOS_DEBUG, "Back from systemagent_early_init()\n");

	s3resume = southbridge_detect_s3_resume();


	elog_boot_notify(s3resume);

	post_code(0x38);

	mainboard_early_init(s3resume);

	post_code(0x39);

	if (CONFIG(INTEL_TXT)) {
		configure_dpr();
		intel_txt_romstage_init();
		disable_intel_txt();
	}

	intel_early_me_init();

	printk(BIOS_DEBUG, "Starting native Platform init\n");

	wait_txt_clear();

	wrmsr(0x2e6, (msr_t) { .lo = 0, .hi = 0 });

	const u32 sskpd = mchbar_read32(SSKPD);	// !!! = 0x00000000
	if ((pci_read_config16(SOUTHBRIDGE, 0xa2) & 0xa0) == 0x20 && sskpd && !s3resume) {
		mchbar_write32(SSKPD, 0);
		system_reset();
	}

	early_pch_init_native();
	early_init_dmi();
	early_thermal_init();

	printk(BIOS_DEBUG, "dram_freq()\n");
	dram_freq();

	printk(BIOS_DEBUG, "Setting normal operation early\n");
	for (int channel = 0; channel < NUM_CHANNELS; channel++) {
		mchbar_write32(MC_INIT_STATE_ch(channel), 1 << 12 | 0x3);
	}

	if (CONFIG(EARLY_GFX_GMA)) {
		RCBA16(DISPBDF) = 0x0010;
		RCBA32_OR(FD2, PCH_ENABLE_DBDF);

		/* Setup GFX stolen as cachable */
		/*
		printk(BIOS_DEBUG, "Setting up GFX stolen MTRR\n");
		msr_t msr = (msr_t){.lo = 0x80a00006, .hi=0x00000000};
		wrmsr(MTRR_PHYS_BASE(2), msr);
		msr = (msr_t){.lo = 0xffffe000 | MTRR_PHYS_MASK_VALID, .hi = 0x0000000f};
		wrmsr(MTRR_PHYS_MASK(2), msr);

		printk(BIOS_DEBUG, "Test read/write\n");
		for (int i = 0; i < 1024; i+=4) {
			printk(BIOS_DEBUG, "%08x\n", i);
			read32((void*)(0x80a00000+i));
			write32((void*)(0x80a00000+i), i);
		}
		printk(BIOS_DEBUG, "Readback\n");
		for (int i = 0; i < 1024; i+=4) {
			printk(BIOS_DEBUG, "%08x: %08x\n", i, read32((void*)(0x80a00000+i)));
		}
		*/

		/* TOLUD */
		uint32_t reg = pci_read_config32(HOST_BRIDGE, TOLUD);
		reg = (reg & ~0xfff00000) | (0x82a << 20);
		printk(BIOS_DEBUG, "PCI(0, 0, 0)[%x] = %x\n", TOLUD, reg);
		pci_write_config32(HOST_BRIDGE, TOLUD, reg);

		/* GFX stolen memory */
		reg = pci_read_config32(HOST_BRIDGE, BDSM);
		reg = (reg & ~0xfff00000) | (0x80a << 20);
		printk(BIOS_DEBUG, "PCI(0, 0, 0)[%x] = %x\n", BDSM, reg);
		pci_write_config32(HOST_BRIDGE, BDSM, reg);
		printk(BIOS_DEBUG, "Before early_graphics_init\n");
		if (!early_graphics_init())
			printk(BIOS_DEBUG, "early_graphics_init failed!\n");
		else
			vga_write_text(VGA_TEXT_CENTER, VGA_TEXT_HORIZONTAL_MIDDLE,
				(const unsigned char *)"Hello from romstage!");
		die("Halt\n");

	}

	perform_raminit(s3resume);

	post_code(0x3b);
	/* Perform some initialization that must run before stage2 */
	early_pch_reset_pmcon();
	post_code(0x3c);

	southbridge_configure_default_intmap();
	southbridge_rcba_config();
	mainboard_late_rcba_config();

	post_code(0x3d);

	northbridge_romstage_finalize();

	post_code(0x3f);

	romstage_handoff_init(s3resume);
}

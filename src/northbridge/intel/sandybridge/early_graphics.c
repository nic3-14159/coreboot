/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/pci.h>
#include <console/console.h>
#include <drivers/intel/gma/libgfxinit.h>
#include "early_graphics.h"

#define SA_DEV_SLOT_IGD 0x02
#define SA_DEVFN_IGD PCI_DEVFN(SA_DEV_SLOT_IGD, 0)
#define SA_DEV_IGD PCI_DEV(0, SA_DEV_SLOT_IGD, 0)

static bool initialized;

static void device_init(void)
{
	/* Disable response in IO and MMIO space. */
	printk(BIOS_DEBUG, "Disable IO/MMIO response\n");
	pci_and_config16(SA_DEV_IGD, PCI_COMMAND, ~(PCI_COMMAND_IO | PCI_COMMAND_MEMORY));

	/* Program IGD Base Address Register 0. */
	printk(BIOS_DEBUG, "Program IGD BAR0\n");
	pci_write_config32(SA_DEV_IGD, PCI_BASE_ADDRESS_0, CONFIG_GFX_GMA_DEFAULT_MMIO);

	/* Enable response in IO and MMIO space. */
	printk(BIOS_DEBUG, "Enable IO/MMIO response\n");
	pci_or_config16(SA_DEV_IGD, PCI_COMMAND, (PCI_COMMAND_IO | PCI_COMMAND_MEMORY));
}

bool early_graphics_init(void)
{
	int ret;
	if (!CONFIG(MAINBOARD_USE_EARLY_LIBGFXINIT))
		return false;

	/* Perform minimal graphic MMIO configuration. */
	printk(BIOS_DEBUG, "device_init()\n");
	device_init();

	/* Configure display panel. */
	printk(BIOS_DEBUG, "early_graphics_panel_init()\n");
	early_graphics_panel_init();

	printk(BIOS_DEBUG, "gma_gfxinit()\n");
	gma_gfxinit(&ret);
	return !!ret;
}

void early_graphics_stop(void)
{
	if (!initialized)
		return;
	gma_gfxstop();
}

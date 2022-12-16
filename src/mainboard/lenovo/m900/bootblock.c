/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootblock_common.h>
#include <device/pnp_ops.h>
#include <superio/nuvoton/common/nuvoton.h>

#define GLOBAL_DEV PNP_DEV(0x2e, 0)
#define COM1_LDN 0x02

static void early_config_superio(void)
{
	nuvoton_pnp_enter_conf_state(GLOBAL_DEV);
	/* Select SIO pin mux states */
	pnp_write_config(GLOBAL_DEV, 0x24, 0x61);
	nuvoton_pnp_exit_conf_state(GLOBAL_DEV);
	const pnp_devfn_t serial_dev = PNP_DEV(0x2e, COM1_LDN);
	nuvoton_enable_serial(serial_dev, CONFIG_TTYS0_BASE);
}

void bootblock_mainboard_early_init(void)
{
	early_config_superio();
}

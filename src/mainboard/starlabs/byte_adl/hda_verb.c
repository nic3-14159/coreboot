/* SPDX-License-Identifier: GPL-2.0-only */

#include <console/console.h>
#include <device/azalia_device.h>
#include <option.h>
#include <types.h>

#define AZALIA_CODEC_SN6140	0x14f11f87

static const u32 override_verb[] = {
	AZALIA_PIN_CFG(0, 0x1a, 0x00000000),
};

static void disable_microphone(u8 *base)
{
	azalia_program_verb_table(base, override_verb, ARRAY_SIZE(override_verb));
}

void mainboard_azalia_program_runtime_verbs(u8 *base, u32 viddid)
{
	if (viddid == AZALIA_CODEC_SN6140) {
		printk(BIOS_DEBUG, "CMOS: viddid = %08x\n", viddid);
		if (get_uint_option("microphone", 1) == 0)
			disable_microphone(base);
	}
}

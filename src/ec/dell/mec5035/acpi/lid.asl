/* SPDX-License-Identifier: GPL-2.0-only */

Device(LID)
{
	Name(_HID, "PNP0C0D")

	Name(_PRW, Package () {24, 3})

	Method(_LID, 0, NotSerialized)
	{
		return (LIDS)
	}
}

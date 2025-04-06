/* SPDX-License-Identifier: GPL-2.0-only */

Device (FAN)
{
	Name (_HID, EISAID ("PNP0C0B"))

	Name (_FIF, Package ()
	{
		0,	// Revision
		1,
		2,
		0
	})

	Name (_FPS, Package ()
	{
		0, /* Revision */
		Package ()
		{
			50,
			0xFFFFFFFF,
			3000,
			0xFFFFFFFF,
			0xFFFFFFFF
		}
	})

	Method (_FSL, 1, NotSerialized)
	{
	}

	Name (FSTP, Package(){
		0, /* Revision */
		50, /* Control */
		0x00, /* Speed */
	})

	Method (_FST, 0, NotSerialized)
	{
		EMXW(2, 0x02)
		EMXW(3, 0)
		EMXW(4, 0)
		EMXW(5, 0)
		EMXW(6, 0)
		EMXC(0x2e)
		Local0 = EMXR(2)
		Local0 = (EMXR(3) << 8) | Local0
		FSTP[2] = Local0
		Return (FSTP)
	}
}

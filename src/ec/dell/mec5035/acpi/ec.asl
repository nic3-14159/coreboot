/* SPDX-License-Identifier: GPL-2.0-only */

Device (EC)
{
	Name (_HID, EisaId("PNP0C09"))

	Name (_CRS, ResourceTemplate()
	{
		IO (Decode16, 0x930, 0x930, 0, 1)
		IO (Decode16, 0x934, 0x934, 0, 1)
	})

	/* Mainboard must define MEC5035_EC_GPE with the GPE number
	 * corresponding to the GPIO the EC SCI is routed to */
	Name (_GPE, MEC5035_EC_GPE)

	Method (_REG, 2, NotSerialized)
	{
		if (Arg1 == 1) {
			SCIE = 1
		}
	}

	Method (EPTS, 1)
	{
		if (Arg0 == 3) {
			EMXW(2, 3)
			EMXW(3, 0)
			EMXC(0x64)
		}
	}

	/* EmbeddedControl Mutex */
	Mutex (ECLK, 0)

	/* Radio Control */
	Method (RADC, 1)
	{
		EMXW(2, 0) /* WLAN */
		EMXW(3, 2) /* Set state */
		EMXW(4, Arg0)
		EMXC(0x2b)
		EMXW(2, 1) /* WWAN */
		EMXW(3, 2) /* Set state */
		EMXW(4, Arg0)
		EMXC(0x2b)
		EMXW(2, 2) /* Bluetooth */
		EMXW(3, 2) /* Set state */
		EMXW(4, Arg0)
		EMXC(0x2b)
	}

	OperationRegion(ERAM, EmbeddedControl, 0, 256)
	Field(ERAM, ByteAcc, NoLock, Preserve)
	{
		Offset (0x00),
				ACPR, 1, /* AC Present */
				BPRT, 1, /* Battery Present */
				    , 2,
				LIDS, 1, /* Lid State */
		Offset (0x05),
				/* SCI Enable */
				SCIE, 8,
	}

	/* Last Lid State */
	Name (LIDL, 1)

	/* Last Battery Present State */
	Name (BPRS, 0)

	/* Last Battery Status State */
	Name (BSTL, 0)

	Method (_Q66)
	{
		if (BPRT != BPRS) {
			BPRS = BPRT
			Notify(BAT0, 0x81)
		}
		if (BSTL != BAST) {
			BSTL = BAST
			Notify(BAT0, 0x80)
		}
		if (LIDS != LIDL) {
			LIDL = LIDS
			Notify(LID, 0x80)
		}
		RADC(SCIE & 1)
	}

	#include "ec_mailbox.asl"
	#include "ac.asl"
	#include "battery.asl"
	#include "lid.asl"
	#include "thermal.asl"
}

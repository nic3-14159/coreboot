/* SPDX-License-Identifier: GPL-2.0-only */

/* Mailbox Interface Mutex */
Mutex (MLCK, 0)

/* EC Mailbox Interface */
OperationRegion(EMBX, SystemIO, 0x910, 2)
Field(EMBX, ByteAcc, NoLock, Preserve)
{
	EMXI, 8,
	EMXD, 8,
}

/* EC Mailbox Read */
Method (EMXR, 1)
{
	EMXI = Arg0 + 0x10
	Return (EMXD)
}

/* EC Mailbox Write */
Method (EMXW, 2)
{
	EMXI = Arg0 + 0x10
	EMXD = Arg1
}

/* EC Mailbox Command */
Method (EMXC, 1)
{
	EMXI = 0
	EMXD = Arg0

	/* Wait for EC */
	Local0 = 1
	while (Local0) {
		EMXI = 0
		Local0 = EMXD
	}
}

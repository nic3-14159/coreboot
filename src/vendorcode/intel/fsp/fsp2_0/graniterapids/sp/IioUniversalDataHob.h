/** @file

Copyright (c) 2024, Intel Corporation. All rights reserved.<BR>

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
* Neither the name of Intel Corporation nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.

  This file is automatically generated. Please do NOT modify !!!

**/

#ifndef _IIO_UNIVERSAL_DATA_HOB_H_
#define _IIO_UNIVERSAL_DATA_HOB_H_

#define IIO_UNIVERSAL_DATA_GUID { 0x7FF396A1, 0xEE7D, 0x431E, { 0xBA, 0x53, 0x8F, 0xCA, 0x12, 0x7C, 0x44, 0xC0 } }

#ifndef MAX_SOCKET
#define MAX_SOCKET              2
#endif

#if (MAX_SOCKET == 1)
  #define MAX_FW_KTI_PORTS     4    // Maximum KTI PORTS to be used in structure definition.
#else
  #define MAX_FW_KTI_PORTS     6    // Maximum KTI PORTS to be used in structure definition
#endif //(MAX_SOCKET == 1)

#define MAX_LOGIC_IIO_STACK          18

#ifndef MAX_IIO_PCIROOTS_PER_STACK
#define MAX_IIO_PCIROOTS_PER_STACK   3   // PCI roots that can be created for a stack
#endif

#define MAX_COMPUTE_DIE            3
#define MAX_CHA_MAP                (2 * MAX_COMPUTE_DIE)  //for GNR & SRF only, each compute die has its own CAPID6 & CAPID7 (i.e. 2 CAPID registers)

#ifndef MAX_MESSAGE_LENGTH
#define MAX_MESSAGE_LENGTH  500
#endif

#define DINO_UID                            0
#define PC_UID                              1
#define UB_UID                              4
#define CPM0_UID                            5
#define HQM0_UID                            6

typedef enum {
  IioStack0 = 0,
  IioStack1 = 1,
  IioStack2 = 2,
  IioStack3 = 3,
  IioStack4 = 4,
  IioStack5 = 5,
  IioStack6 = 6,
  IioStack7 = 7,
  IioStack8 = 8,
  IioStack9 = 9,
  IioStack10 = 10,
  IioStack11 = 11,
  IioStack12 = 12,
  IioStack13 = 13,
  IioStack14 = 14,
  IioStack15 = 15,
  IioStack16 = 16,
  IioStackUnknown = 0xFF
} IIO_STACK;

//--------------------------------------------------------------------------------------//
// Structure definitions for Universal Data Store (UDS)
//--------------------------------------------------------------------------------------//
#pragma pack(1)

typedef struct {
  UINT8                     Valid;         // TRUE, if the link is valid (i.e reached normal operation)
  UINT8                     PeerSocId;     // Socket ID
  UINT8                     PeerSocType;   // Socket Type (0 - CPU; 1 - IIO)
  UINT8                     PeerPort;      // Port of the peer socket
} QPI_PEER_DATA;

typedef struct {
  UINT8                     Valid;
  UINT8                     PcieSegment;
  UINT64                    SegMmcfgBase;
  UINT32                    StackPresentBitmap;
  UINT16                    Cxl1p1PresentBitmap; // Bitmap of stacks where CXL 1p1 is connected
  UINT16                    CxlCapableBitmap;    // Bitmap of stacks capable of CXL
  UINT8                     TotCha;
  UINT32                    SocId;
  QPI_PEER_DATA             PeerInfo[MAX_FW_KTI_PORTS];    // QPI LEP info
} QPI_CPU_DATA;

/**
 * PCI resources that establish one PCI hierarchy for PCI Enumerator.
 */
typedef struct {
  UINT16                  UidType;               // Type of UID for this root bridge.
  UINT8                   BusBase;               // Base of PCI bus numbers available for PCI devices
  UINT8                   BusLimit;              // Limit of PCI bus numbers available for PCI devices
  UINT16                  IoBase;                // Base of IO resources available for PCI devices
  UINT16                  IoLimit;               // Limit of IO resources available for PCI devices
  UINT32                  Mmio32Base;            // Base of low MMIO resources available for PCI devices
  UINT32                  Mmio32Limit;           // Limit of low MMIO resources available for PCI devices
  UINT64                  Mmio64Base;            // Base of high MMIO resources available for PCI devices
  UINT64                  Mmio64Limit;           // Limit of high MMIO resources available for PCI devices
} UDS_PCIROOT_RES;

/**
 * This structore keeps resources configured in Host I/O Processor (HIOP) for one stack.
 * One HIOP may produce more than one PCI hierarchy, these are in PciRoot[] table.
 */
typedef struct {
  UINT8                   Personality;
  UINT8                   PciRootBridgeNum;      // Number of valid entries in PciRoot[] table
  UINT8                   Segment;               // Segment for this stack
  UINT8                   BusBase;               // Base of Bus configured for this stack
  UINT8                   BusLimit;              // Limit of Bus configured for this stack
  UINT8                   Reserved[3];           // Reserved for alignment
  UINT16                  IoBase;                // Base of IO configured for this stack
  UINT16                  IoLimit;               // Limit of IO configured for this stack
  UINT32                  Mmio32Base;            // Base of low MMIO configured for this stack in memory map
  UINT32                  Mmio32Limit;           // Limit of low MMIO configured for this stack in memory map
  UINT32                  Mmio32MinSize;         // The size of MMIO32 needed in PEI that must be preserved in rebalance
  UINT64                  Mmio64Base;            // Base of high MMIO configured for this stack in memory map
  UINT64                  Mmio64Limit;           // Limit of high MMIO configured for this stack in memory map
  UINT64                  VtbarAddress;          // VT-d Base Address
  UINT64                  ChbcrBarAddress;       // CXL Host Bridge Component Registers (CHBCR) region
  UDS_PCIROOT_RES         PciRoot[MAX_IIO_PCIROOTS_PER_STACK];
} UDS_STACK_RES;

/**
 * This structore keeps resource ranges configured in one socket. It contains a table of IO stacks provided by
 // the socket. The stacks are also groupded by IO dies, but dies are not reflected in UDS.
 */
typedef struct {
  UINT8                   Valid;
  UINT8                   SocketID;               // Socket ID of the IIO (0..3)
  UINT16                  IoBase;
  UINT16                  IoLimit;
  UINT32                  Mmio32Base;             // Base of low MMIO configured for this socket in memory map
  UINT32                  Mmio32Limit;            // Limit of low MMIO configured for this socket in memory map
  UINT64                  Mmio64Base;             // Base of high MMIO configured for this socket in memory map
  UINT64                  Mmio64Limit;            // Limit of high MMIO configured for this socket in memory map
  UDS_STACK_RES           StackRes[MAX_LOGIC_IIO_STACK];
} UDS_SOCKET_RES;

/**
 * This structore keeps resource ranges configured in whole system.
 */
typedef struct {
  UINT16                  SystemIoBase;           // System IO Base
  UINT16                  SystemIoLimit;          // System IO Limit
  UINT32                  SystemIoApicBase;       // Legacy IOAPIC base address, one in the system
  UINT32                  SystemIoApicLimit;
  UINT32                  SystemMmio32Base;       // System low MMIO base
  UINT32                  SystemMmio32Limit;      // System low MMIO limit
  UINT64                  SystemMmio64Base;       // System high MMIO Base
  UINT64                  SystemMmio64Limit;      // System high MMIO Limit
  QPI_CPU_DATA            CpuQpiInfo[MAX_SOCKET]; // QPI related info per CPU
  UINT64                  PciExpressBase;         // PCI Config Space base address
  UINT64                  PciExpressSize;         // PCI Config Space size
  UDS_SOCKET_RES          IIO_resource[MAX_SOCKET];
  UINT32                  packageBspApicID[MAX_SOCKET]; // This data array is valid only for SBSP, not for non-SBSP CPUs. <AS> for CpuSv
  UINT16                  IoGranularity;
  UINT32                  Mmio32Granularity;
  UINT64                  Mmio64Granularity;
  UINT32                  UboxMmioSize;
  UINT32                  MemTolm;
  UINT32                  MemTsegSize;
  UINT32                  MaxAddressBits;
} PLATFORM_DATA;

typedef struct {
    BOOLEAN                 FailFlag;
    CHAR16                  Message[MAX_MESSAGE_LENGTH];
} REBALANCE_FAIL_INFO;

typedef struct {
    UINT8                   CurrentUpiiLinkSpeed;    // Current programmed UPI Link speed (Slow/Full speed mode)
    UINT8                   CurrentUpiLinkFrequency; // Current requested UPI Link frequency (in GT)
    UINT8                   OutKtiCpuSktHotPlugEn;            // 0 - Disabled, 1 - Enabled for PM X2APIC
    UINT32                  meRequestedSize;        // Size of the memory range requested by ME FW, in MB
    UINT8                   SystemRasType;
    UINT8                   numCpus;           // 1,..4. Total number of CPU packages installed and detected (1..4)by QPI RC
    UINT8                   OutSncEn;
    UINT8                   OutNumOfCluster;
    UINT8                   BitsUsed;    //For 5 Level Paging
    REBALANCE_FAIL_INFO     RebalanceFailInfo;
} SYSTEM_STATUS;

typedef struct {
    PLATFORM_DATA           PlatformData;
    SYSTEM_STATUS           SystemStatus;
    UINT32                  OemValue;
} IIO_UDS;

#pragma pack()

#endif  // _IIO_UNIVERSAL_DATA_HOB_H_


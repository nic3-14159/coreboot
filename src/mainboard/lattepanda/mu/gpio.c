/* SPDX-License-Identifier: GPL-2.0-only */

#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <commonlib/helpers.h>

/* Pad configuration in ramstage*/
static const struct pad_config gpio_table[] = {
	/* CORE_VID0 */
	PAD_CFG_NF(GPP_B0, NONE, DEEP, NF1),
	/* CORE_VID1 */
	PAD_CFG_NF(GPP_B1, NONE, DEEP, NF1),
	/* VR_ALERT_N */
	PAD_CFG_NF(GPP_B2, NONE, DEEP, NF1),
	/* SLP_S0_N */
	PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
	/* PLTRST_N */
	PAD_CFG_NF(GPP_B13, NONE, DEEP, NF1),
	PAD_CFG_GPO(GPP_B14, 0, PLTRST),

	/* ESPI_IO0_EC_R / ESPI_IO0_HDR */
	PAD_CFG_NF(GPP_A0, NONE, DEEP, NF1),
	/* ESPI_IO1_EC_R / ESPI_IO1_HDR */
	PAD_CFG_NF(GPP_A1, NONE, DEEP, NF1),
	/* ESPI_IO2_EC_R / ESPI_IO2_HDR */
	PAD_CFG_NF(GPP_A2, NONE, DEEP, NF1),
	/* ESPI_IO3_EC_R / ESPI_IO3_HDR */
	PAD_CFG_NF(GPP_A3, NONE, DEEP, NF1),
	/* ESPI_CS0_EC_R_N / ESPI_CS0_HDR_N */
	PAD_CFG_NF(GPP_A4, NONE, DEEP, NF1),
	/* ESPI_ALERT0_EC_R_N / ESPI_ALERT0_HDR_N */
	PAD_CFG_NF(GPP_A5, NONE, DEEP, NF1),
	/* ESPI_ALERT1_EC_R_N / ESPI_ALERT1_HDR_N */
	PAD_CFG_NF(GPP_A6, NONE, DEEP, NF1),
	/* ESPI_CLK_EC_R / ESPI_CLK_HDR */
	PAD_CFG_NF(GPP_A9, NONE, DEEP, NF1),
	/* ESPI_RST_EC_R_N / ESPI_RST_HDR_N */
	PAD_CFG_NF(GPP_A10, NONE, DEEP, NF1),

	/* H15 : DDPB_CTRLCLK ==> DDIB_HDMI_CTRLCLK */
	PAD_CFG_NF(GPP_H15, NONE, DEEP, NF1),
	/* H17 : DDPB_CTRLDATA ==> DDIB_HDMI_CTRLDATA */
	PAD_CFG_NF(GPP_H17, NONE, DEEP, NF1),

	/* I5  : NC */
	PAD_NC(GPP_I5, NONE),
	/* I7  : EMMC_CMD ==> EMMC_CMD */
	PAD_CFG_NF(GPP_I7, NONE, DEEP, NF1),
	/* I8  : EMMC_DATA0 ==> EMMC_D0 */
	PAD_CFG_NF(GPP_I8, NONE, DEEP, NF1),
	/* I9  : EMMC_DATA1 ==> EMMC_D1 */
	PAD_CFG_NF(GPP_I9, NONE, DEEP, NF1),
	/* I10 : EMMC_DATA2 ==> EMMC_D2 */
	PAD_CFG_NF(GPP_I10, NONE, DEEP, NF1),
	/* I11 : EMMC_DATA3 ==> EMMC_D3 */
	PAD_CFG_NF(GPP_I11, NONE, DEEP, NF1),
	/* I12 : EMMC_DATA4 ==> EMMC_D4 */
	PAD_CFG_NF(GPP_I12, NONE, DEEP, NF1),
	/* I13 : EMMC_DATA5 ==> EMMC_D5 */
	PAD_CFG_NF(GPP_I13, NONE, DEEP, NF1),
	/* I14 : EMMC_DATA6 ==> EMMC_D6 */
	PAD_CFG_NF(GPP_I14, NONE, DEEP, NF1),
	/* I15 : EMMC_DATA7 ==> EMMC_D7 */
	PAD_CFG_NF(GPP_I15, NONE, DEEP, NF1),
	/* I16 : EMMC_RCLK ==> EMMC_RCLK */
	PAD_CFG_NF(GPP_I16, NONE, DEEP, NF1),
	/* I17 : EMMC_CLK ==> EMMC_CLK */
	PAD_CFG_NF(GPP_I17, NONE, DEEP, NF1),
	/* I18 : EMMC_RESET# ==> EMMC_RST_L */
	PAD_CFG_NF(GPP_I18, NONE, DEEP, NF1),

	/* EDP1_HPD_MIPI_PNL_RST */
	PAD_CFG_NF(GPP_E14, NONE, DEEP, NF1),

	/* PM_SLP_S0_N */
	PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
	/* PLT_RST_N */
	PAD_CFG_NF(GPP_B13, NONE, DEEP, NF1),
	/* PM_SLP_DRAM_N */
	PAD_CFG_NF(GPP_E8, NONE, DEEP, NF2),
	/* CPU_C10_GATE_N_R */
	PAD_CFG_NF(GPP_H18, NONE, DEEP, NF1),

	/* DDIB_DP_HDMI_ALS_HDP */
	PAD_CFG_NF(GPP_A18, NONE, DEEP, NF1),
	/* PM_BATLOW_N */
	PAD_CFG_NF(GPD0, UP_20K, PWROK, NF1),
	/* AC_PRESENT */
	PAD_CFG_NF(GPD1, NATIVE, PWROK, NF1),
	PAD_CFG_GPI_SCI(GPD2, NONE, DEEP, EDGE_SINGLE, INVERT),
	/* PWR_BTN_N */
	PAD_CFG_NF(GPD3, UP_20K, PWROK, NF1),
	/* SUSB_N_PCH */
	PAD_CFG_NF(GPD4, NONE, PWROK, NF1),
	/* SUSC_N_PCH */
	PAD_CFG_NF(GPD5, NONE, PWROK, NF1),
	/* SLP_A_N */
	PAD_CFG_NF(GPD6, NONE, PWROK, NF1),
	PAD_CFG_GPO(GPD7, 0, PWROK),
	/* SUS_CLK */
	PAD_CFG_NF(GPD8, NONE, PWROK, NF1),
	/* SLP_WLAN_N */
	PAD_CFG_NF(GPD9, NONE, PWROK, NF1),
	/* SLP_S5_N */
	PAD_CFG_NF(GPD10, NONE, PWROK, NF1),
	PAD_NC(GPD11, NONE),
	PAD_CFG_NF(GPD_INPUT3VSEL, NONE, PWROK, NF1),
	PAD_CFG_NF(GPD_SLP_LANB, NONE, PWROK, NF1),
	PAD_CFG_NF(GPD_SLP_SUSB, NONE, PWROK, NF1),
	PAD_CFG_NF(GPD_WAKEB, NONE, PWROK, NF1),
	PAD_CFG_NF(GPD_DRAM_RESETB, NONE, PWROK, NF1),

	/* SMB_CLK */
	PAD_CFG_NF(GPP_C0, NONE, DEEP, NF1),
	/* SMB_DATA */
	PAD_CFG_NF(GPP_C1, NONE, DEEP, NF1),
	/* SMB_ALERT_N */
	PAD_CFG_NF(GPP_C2, NONE, DEEP, NF1),
	/* SML0_CLK */
	PAD_CFG_NF(GPP_C3, NONE, DEEP, NF1),
	/* SML1_DATA */
	PAD_CFG_NF(GPP_C4, NONE, DEEP, NF1),
	PAD_CFG_TERM_GPO(GPP_C5, 1, DN_20K, PLTRST),
	/* SML1_CLK */
	PAD_CFG_NF(GPP_C6, NONE, RSMRST, NF1),
	/* SML1_DATA */
	PAD_CFG_NF(GPP_C7, NONE, RSMRST, NF1),
};

void variant_configure_gpio_pads(void)
{
	gpio_configure_pads(gpio_table, ARRAY_SIZE(gpio_table));
}

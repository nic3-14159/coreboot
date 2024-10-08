# SPDX-License-Identifier: GPL-2.0-or-later

SPD_SOURCES_DIR=$(dir $(word 1, $(SPD_SOURCES)))

APCB_SOURCES=$(foreach f, $(basename $(notdir $(SPD_SOURCES))), $(obj)/APCB_$(f).gen)

# APCB binary with magic numbers to be replaced by apcb_edit tool
ifeq ($(CONFIG_ZORK_UMA_SIZE_64MB),y)
APCB_NAME=uma_64.apcb
else ifeq ($(CONFIG_ZORK_UMA_SIZE_128MB),y)
APCB_NAME=uma_128.apcb
else ifeq ($(CONFIG_ZORK_UMA_SIZE_256MB),y)
APCB_NAME=uma_256.apcb
else ifeq ($(CONFIG_ZORK_UMA_SIZE_512MB),y)
APCB_NAME=uma_512.apcb
endif

APCB_PATH=$(src)/mainboard/$(MAINBOARDDIR)

$(obj)/APCB_%.gen:	$(SPD_SOURCES_DIR)%.hex \
			$(APCB_EDIT_TOOL) \
			$(APCB_PATH)/$(APCB_NAME)
	$(APCB_EDIT_TOOL) \
	$(APCB_PATH)/$(APCB_NAME) \
	$@ \
	--hex \
	--strip_manufacturer_information \
	--spd_0_0 $< \
	$(if $(APCB_POPULATE_2ND_CHANNEL), --spd_1_0 $<, ) \
	--board_id_gpio0 $(APCB_BOARD_ID_GPIO0) \
	--board_id_gpio1 $(APCB_BOARD_ID_GPIO1) \
	--board_id_gpio2 $(APCB_BOARD_ID_GPIO2) \
	--board_id_gpio3 $(APCB_BOARD_ID_GPIO3)

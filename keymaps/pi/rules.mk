MCU_HELIOS = yes

ifeq ($(strip $(MCU_HELIOS)), yes)
   OPT_DEFS += -DMCU_HELIOS
endif

TAP_DANCE_ENABLE = yes
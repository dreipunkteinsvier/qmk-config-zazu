MCU_HELIOS = yes

ifeq ($(strip $(MCU_HELIOS)), yes)
   OPT_DEFS += -DMCU_HELIOS
endif

SRC += nshot_mod.c \
       repeat.c
VPATH += keyboards/gboards
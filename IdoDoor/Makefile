# name of the application
APPLICATION = IdoDoor

# If no BOARD is found in the environment, use this default:
BOARD ?= idosens_base

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT/

# required modules
#USEMODULE +=  shell
#USEMODULE +=  shell_commands
USEMODULE +=  xtimer
USEMODULE += sx1276
USEMODULE += crypto_aes_128

# required features
#FEATURES_REQUIRED +=  periph_adc
FEATURES_REQUIRED +=  periph_rtc
FEATURES_REQUIRED +=  periph_uart
FEATURES_REQUIRED += periph_spi

INCLUDES += -I$(CURDIR)/include
INCLUDES += -I$(CURDIR)/../IdoCommon

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

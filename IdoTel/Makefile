# name of the application
APPLICATION = IdoTel

# If no BOARD is found in the environment, use this default:
BOARD ?= idosens_remote

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT/

# required modules
USEMODULE +=  shell
USEMODULE +=  shell_commands
USEMODULE += sx1276
USEMODULE += crypto_aes_128

# required features
FEATURES_REQUIRED +=  periph_rtc
FEATURES_REQUIRED +=  periph_uart periph_uart_modecfg
FEATURES_REQUIRED += periph_spi

INCLUDES += -I$(CURDIR)/include
INCLUDES += -I$(CURDIR)/../IdoCommon

#Baudrate par défault du minitel
CFLAGS+=-DSTDIO_UART_BAUDRATE=1200
#Surcharge la function stdio_write pour rajouter le \r
LINKFLAGS+=$(LINKFLAGPREFIX)--wrap=stdio_write

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

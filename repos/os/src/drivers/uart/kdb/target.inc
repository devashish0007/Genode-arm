TARGET   = kdb_uart_drv
SRC_CC   = main.cc
LIBS    += base
INC_DIR += $(REP_DIR)/src/drivers/uart $(REP_DIR)/src/drivers/uart/kdb

vpath main.cc $(REP_DIR)/src/drivers/uart

CC_CXX_WARN_STRICT_CONVERSION =

TARGET     = virtio_mmio_nic
SRC_CC     = mmio_device.cc
LIBS       = base nic_driver
INC_DIR    = $(REP_DIR)/src/drivers/nic/virtio
CONFIG_XSD = ../../config.xsd

vpath % $(REP_DIR)/src/drivers/nic/virtio

CC_CXX_WARN_STRICT_CONVERSION =

KERNEL_OBJS :=
KERNEL_OBJS += $(BUILD_DIR)/kernel/startup.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/interrupt_dispatch.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/system_time.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/vector.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/irq_handler.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/kernel.c.o

KERNEL_LIBS :=
KERNEL_LIBS += $(BUILD_DIR)/button/button.a
KERNEL_LIBS += $(BUILD_DIR)/lcd/lcd.a
KERNEL_LIBS += $(BUILD_DIR)/hw/hw.a
KERNEL_LIBS += $(BUILD_DIR)/timer/timer.a

KERNEL_TOOLCHAIN_LIBS :=
KERNEL_TOOLCHAIN_LIBS += none.lib

$(BUILD_DIR)/kernel/kernel: $(SRC_DIR)/kernel/system.cfg $(KERNEL_OBJS) $(KERNEL_LIBS)
	$(echo_build_message)
	$(echo_recipe)ld65 -o $@ --lib-path $(BUILD_DIR) -C $^ $(KERNEL_TOOLCHAIN_LIBS) && chmod a+x $@

all: $(BUILD_DIR)/kernel/kernel

.PHONY: install
install: $(BUILD_DIR)/kernel/kernel
	$(echo_build_message)
	$(echo_recipe)minipro -p AT28C256 -w $(BUILD_DIR)/kernel/kernel

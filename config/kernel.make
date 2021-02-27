KERNEL_OBJS :=
KERNEL_OBJS += $(BUILD_DIR)/kernel/delay.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/interrupt_dispatch.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/interrupt_wait.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/irq_handler.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/kernel.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/log.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/memory.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/startup.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/system_time.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/vector.s.o

KERNEL_LIBS :=
KERNEL_LIBS += $(BUILD_DIR)/button/button.a
KERNEL_LIBS += $(BUILD_DIR)/buzzer/buzzer.a
KERNEL_LIBS += $(BUILD_DIR)/configure/configure.a
KERNEL_LIBS += $(BUILD_DIR)/console/console.a
KERNEL_LIBS += $(BUILD_DIR)/hw/hw.a
KERNEL_LIBS += $(BUILD_DIR)/lcd/lcd.a
KERNEL_LIBS += $(BUILD_DIR)/log/log.a
KERNEL_LIBS += $(BUILD_DIR)/memtest/memtest.a
KERNEL_LIBS += $(BUILD_DIR)/switcher/switcher.a
KERNEL_LIBS += $(BUILD_DIR)/timer/timer.a
KERNEL_LIBS += $(BUILD_DIR)/uptime/uptime.a

KERNEL_TOOLCHAIN_LIBS :=
KERNEL_TOOLCHAIN_LIBS += $(BUILD_DIR)/cc65/cc65.a

$(BUILD_DIR)/kernel/kernel: $(SRC_DIR)/kernel/system.cfg $(KERNEL_OBJS) $(KERNEL_LIBS) $(KERNEL_TOOLCHAIN_LIBS)
	$(echo_build_message)
	$(echo_recipe)ld65 -o $@ --lib-path $(BUILD_DIR) -C $< --start-group $(filter-out $<,$^) none.lib --end-group && chmod a+x $@

$(BUILD_DIR)/kernel/kernel.checksum: $(BUILD_DIR)/kernel/kernel
	$(echo_build_message)
	$(echo_recipe)cat $^ | gzip -c | tail -c8 | head -c4 >$@

$(BUILD_DIR)/kernel/kernel.img: $(BUILD_DIR)/kernel/kernel.checksum $(BUILD_DIR)/kernel/kernel
	$(echo_build_message)
	$(echo_recipe)cat $^ >$@

all: $(BUILD_DIR)/kernel/kernel.img

.PHONY: install
install: $(BUILD_DIR)/kernel/kernel.img
	$(echo_build_message)
	$(echo_recipe)minipro -p AT28C256 -w $^

.PHONY: clean-kernel
clean-kernel:
	$(echo_recipe)[ ! -d $(build_dir) ] || find $(build_dir) -type f \( -name '*.checksum' -o -name '*.img' \) -delete

clean: clean-kernel

KERNEL_OBJS :=
KERNEL_OBJS += $(BUILD_DIR)/kernel/delay.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/interrupt_dispatch.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/interrupt_wait.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/irq_handler.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/kernel.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/memory.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/nmi_handler.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/startup.s.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/system_time.c.o
KERNEL_OBJS += $(BUILD_DIR)/kernel/vector.s.o

KERNEL_LIBS :=
KERNEL_LIBS += $(BUILD_DIR)/configure/configure.a
KERNEL_LIBS += $(BUILD_DIR)/controller/controller.a
KERNEL_LIBS += $(BUILD_DIR)/display/display.a
KERNEL_LIBS += $(BUILD_DIR)/display/driver/hd44780/hd44780.a
KERNEL_LIBS += $(BUILD_DIR)/display/driver/us2066/us2066.a
KERNEL_LIBS += $(BUILD_DIR)/distraction/scroll/scroll.a
KERNEL_LIBS += $(BUILD_DIR)/distraction/wisdom/wisdom.a
KERNEL_LIBS += $(BUILD_DIR)/ft245r/ft245r.a
KERNEL_LIBS += $(BUILD_DIR)/io/io.a
KERNEL_LIBS += $(BUILD_DIR)/loader/loader.a
KERNEL_LIBS += $(BUILD_DIR)/log/log.a
KERNEL_LIBS += $(BUILD_DIR)/memtest/memtest.a
KERNEL_LIBS += $(BUILD_DIR)/panic/panic.a
KERNEL_LIBS += $(BUILD_DIR)/sd/sd.a
KERNEL_LIBS += $(BUILD_DIR)/speedtest/speedtest.a
KERNEL_LIBS += $(BUILD_DIR)/spi/spi.a
KERNEL_LIBS += $(BUILD_DIR)/switcher/switcher.a
KERNEL_LIBS += $(BUILD_DIR)/timer/timer.a
KERNEL_LIBS += $(BUILD_DIR)/ui/ui.a
KERNEL_LIBS += $(BUILD_DIR)/uptime/uptime.a
KERNEL_LIBS += $(BUILD_DIR)/util/util.a
KERNEL_LIBS += $(BUILD_DIR)/via/via.a

#SBC_VERSION ?= 0
#SBC_VERSION ?= 1
#SBC_VERSION ?= 2
SBC_VERSION ?= 2_2

CXXFLAGS += -DHW_CONFIGURATION_SBC_V$(SBC_VERSION)=1
CXX.MKDEPS.CXXFLAGS += -DHW_CONFIGURATION_SBC_V$(SBC_VERSION)=1

SBC_CONFIG = config/sbc-v$(SBC_VERSION).cfg

$(BUILD_DIR)/kernel/kernel: $(SBC_CONFIG) $(KERNEL_OBJS) $(KERNEL_LIBS)
	$(echo_build_message)
	$(echo_recipe)ld65 -o $@ --lib-path $(BUILD_DIR) -C $< --start-group $(filter-out $<,$^) none.lib --end-group && chmod a+x $@

$(BUILD_DIR)/kernel/kernel.with.commit.img: $(BUILD_DIR)/kernel/kernel
	$(echo_build_message)
	$(echo_recipe)([[ $$(git status --porcelain) == "" ]] && (git rev-parse HEAD | cut -b 1-7 | tr '\n' '\0') || echo -ne 'adhoc  \0') >$@
	$(echo_recipe)cat $^ >>$@

$(BUILD_DIR)/kernel/kernel.img: $(BUILD_DIR)/kernel/kernel.with.commit.img
	$(echo_build_message)
	$(echo_recipe)cat $^ | gzip -c | tail -c8 | head -c4 >$@
	$(echo_recipe)cat $^ >>$@

all: $(BUILD_DIR)/kernel/kernel.img

.PHONY: install
install: $(BUILD_DIR)/kernel/kernel.img
	$(echo_build_message)
	$(echo_recipe)minipro -p AT28C256 -w $^

.PHONY: clean-kernel
clean-kernel:
	$(echo_recipe)[ ! -d $(build_dir) ] || find $(build_dir) -type f -name '*.img' -delete

clean: clean-kernel

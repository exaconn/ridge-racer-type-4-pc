# Convenience commands for the native C build.
BUILD_DIR ?= build
CMAKE ?= cmake

.PHONY: all configure build run test ci clean
all: build

configure:
	$(CMAKE) -S . -B $(BUILD_DIR)

build: configure
	$(CMAKE) --build $(BUILD_DIR) --parallel

run: build
	./$(BUILD_DIR)/ridge-racer-type-4-pc $(RUN_ARGS)

test:
	$(CMAKE) -S . -B $(BUILD_DIR) -DBUILD_TESTING=ON
	$(CMAKE) --build $(BUILD_DIR) --parallel
	ctest --test-dir $(BUILD_DIR) --output-on-failure

ci: build

clean:
	$(CMAKE) --build $(BUILD_DIR) --target clean

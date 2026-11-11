# Convenience commands for the native C build.
BUILD_DIR ?= build
CMAKE ?= cmake

.PHONY: all configure build game check run test ci clean
all: build

configure:
	$(CMAKE) -S . -B $(BUILD_DIR) $(CMAKE_ARGS)

build: configure
	$(CMAKE) --build $(BUILD_DIR) --parallel

# Game modules only (static library rr4_game).
game: configure
	$(CMAKE) --build $(BUILD_DIR) --target rr4_game --parallel

# Compile-check game sources with the host flags, without a CMake build.
check:
	tools/port/check.sh $(CHECK_PATHS)

run: build
	./$(BUILD_DIR)/ridge-racer-type-4-pc $(RUN_ARGS)

test:
	$(CMAKE) -S . -B $(BUILD_DIR) -DBUILD_TESTING=ON
	$(CMAKE) --build $(BUILD_DIR) --parallel
	ctest --test-dir $(BUILD_DIR) --output-on-failure

ci: build

clean:
	$(CMAKE) --build $(BUILD_DIR) --target clean

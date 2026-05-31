PROJECT := $(notdir $(CURDIR))
config ?= release
jobs ?= $(shell nproc 2>/dev/null || echo 4)
PREMAKE ?= $(or $(wildcard $(HOME)/tools/premake5),premake5)
CCACHE := $(shell command -v ccache 2>/dev/null)

ifneq ($(CCACHE),)
	BUILD_ENV := CCACHE_CPP2=yes CC="$(CCACHE) gcc" CXX="$(CCACHE) g++"
endif

.PHONY: configure build run format lint prepare-for-commit clean

configure:
	@mkdir -p build
	@tmp=$$(mktemp); cp Makefile $$tmp; \
	printf '%s\n' 'workspace "saffron-engine-2d-ci"' '  architecture "x64"' '  configurations { "Debug", "Release", "Dist" }' '  flags { "MultiProcessorCompile" }' 'dofile("premake5.lua")' > ci_premake5.lua; \
	$(PREMAKE) --file=ci_premake5.lua gmake2; \
	mv Makefile build/Makefile; \
	cp $$tmp Makefile; chmod 0644 Makefile; \
	rm -f $$tmp
	@if [ -d build/obj ] && grep -Rqs 'source/' build/obj; then rm -rf build/obj; fi

build: configure
	$(BUILD_ENV) $(MAKE) --no-print-directory -f build/Makefile config=$(config) $(PROJECT) -j$(jobs)

run: build
	@echo "$(PROJECT) is a static library; no executable is produced."

format:
	@command -v clang-format >/dev/null || { echo "clang-format is required for make format"; exit 1; }
	@find src -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' -o -name '*.c' \) -print0 | xargs -0 clang-format -i

lint:
	@! grep -RInE '\b(throw|try|catch)\b' src --include='*.cpp' --include='*.h' --include='*.hpp'
	@! grep -RIn 'namespace std' src --include='*.cpp' --include='*.h' --include='*.hpp'
	@! grep -RInP '\([^;{}]*\b[A-Za-z_][A-Za-z0-9_:<>]*\s+[A-Za-z_][A-Za-z0-9_]*\s*=[^=][^;{}]*\)\s*(const\s*)?(override\s*)?(final\s*)?;' src --include='*.h' --include='*.hpp'
	@! grep -RInE '(^|[^$$[:alnum:]_])source/' . --exclude=Makefile --exclude-dir=.git --exclude-dir=build --exclude-dir=deps --exclude-dir=.github

prepare-for-commit: format lint

clean:
	@if [ -f build/Makefile ]; then $(MAKE) --no-print-directory -f build/Makefile clean; fi

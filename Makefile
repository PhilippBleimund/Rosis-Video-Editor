QTSOURCE := /usr/lib/qt6/bin/
SOURCE := ../Rosis-Video-Editor/
BUILD := ./build/Desktop-Debug/

default:
	cmake -DCMAKE_PREFIX_PATH=$(QTSOURCE) -S $(SOURCE) -B $(BUILD)
	\cp ./build/Desktop-Debug/compile_commands.json ./compile_commands.json
	cmake --build $(BUILD)
	$(BUILD)Rosis-Video-Editor

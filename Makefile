BUILD_PATH	:= build/forcerush
LDFLAGS	:= `sdl2-config --cflags`
CFLAGS	:= -c -g
LIBS	:= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lsmpeg2 -lmad -lSDL2_ttf -lsamplerate
VPATH	:= main : prepare : game : loading : user : about : gui : tools
OBJ	:= \
	$(BUILD_PATH)/about_state.o \
	$(BUILD_PATH)/animator.o \
	$(BUILD_PATH)/background.o \
	$(BUILD_PATH)/beatmap.o \
	$(BUILD_PATH)/button.o \
	$(BUILD_PATH)/column.o \
	$(BUILD_PATH)/control_handler.o \
	$(BUILD_PATH)/file_explorer.o \
	$(BUILD_PATH)/file_system.o \
	$(BUILD_PATH)/gameplay_wizard_state.o \
	$(BUILD_PATH)/game_header.o \
	$(BUILD_PATH)/game_state.o \
	$(BUILD_PATH)/list.o \
	$(BUILD_PATH)/list_widget.o \
	$(BUILD_PATH)/loading_state.o \
	$(BUILD_PATH)/main.o \
	$(BUILD_PATH)/main_state.o \
	$(BUILD_PATH)/mod_widget.o \
	$(BUILD_PATH)/prepare_header.o \
	$(BUILD_PATH)/prepare_state.o \
	$(BUILD_PATH)/result_state.o \
	$(BUILD_PATH)/setting.o \
	$(BUILD_PATH)/shape.o \
	$(BUILD_PATH)/song_list.o \
	$(BUILD_PATH)/song_path_manager.o \
	$(BUILD_PATH)/sound_manager.o \
	$(BUILD_PATH)/sprite.o \
	$(BUILD_PATH)/system.o \
	$(BUILD_PATH)/text_area.o \
	$(BUILD_PATH)/text_input_box.o \
	$(BUILD_PATH)/texture_manager.o \
	$(BUILD_PATH)/timer.o

all: run

.PHONY: clear prepare
.IGNORE: clear prepare

clear:
	rm -rf build/forcerush

prepare:
	mkdir build
	mkdir build/forcerush

run: $(OBJ)
	$(CXX) $(LDFLAGS) $(LIBS) $(OBJ) -o $@ -g

$(BUILD_PATH)/about_state.o: about_state.cpp about_state.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/animator.o: animator.cpp animator.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/background.o: background.cpp background.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/beatmap.o: beatmap.cpp beatmap.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/button.o: button.cpp button.h sprite.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/column.o: column.cpp column.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/control_handler.o: control_handler.cpp control_handler.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/file_explorer.o: file_explorer.cpp file_explorer.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/file_system.o: file_system.cpp file_system.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/gameplay_wizard_state.o: gameplay_wizard_state.cpp gameplay_wizard_state.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/game_header.o: game_header.cpp game_header.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/game_state.o: game_state.cpp game_state.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/list.o: list.cpp list.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/list_widget.o: list_widget.cpp list_widget.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/loading_state.o: loading_state.cpp loading_state.h state.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/main.o: main.cpp prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/main_state.o: main_state.cpp main_state.h state.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/mod_widget.o: mod_widget.cpp mod_widget.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/prepare_header.o: prepare_header.cpp prepare_header.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/prepare_state.o: prepare_state.cpp prepare_state.h state.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/result_state.o: result_state.cpp result_state.h state.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/setting.o: setting.cpp setting.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/shape.o: shape.cpp shape.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/song_list.o: song_list.cpp song_list.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/song_path_manager.o: song_path_manager.cpp song_path_manager.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/sound_manager.o: sound_manager.cpp sound_manager.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/sprite.o: sprite.cpp sprite.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/system.o: system.cpp system.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/text_area.o: text_area.cpp text_area.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/text_input_box.o: text_input_box.cpp text_input_box.h button.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/texture_manager.o: texture_manager.cpp texture_manager.h prepare
	$(CXX) $< $(CFLAGS) -o $@
$(BUILD_PATH)/timer.o: timer.cpp timer.h prepare
	$(CXX) $< $(CFLAGS) -o $@
LDFLAGS	:= `sdl2-config --cflags`
CFLAGS	:= -c -g
LIBS	:= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lsmpeg2 -lmad -lSDL2_ttf
VPATH	:= main : prepare : game : loading : user : about : gui

all: run

OBJ	:= about_state.o animator.o background.o beatmap.o button.o column.o control_handler.o file_system.o gameplay_wizard_state.o game_header.o game_state.o list.o list_widget.o loading_state.o main.o main_state.o mod_widget.o prepare_header.o prepare_state.o result_state.o setting.o shape.o song_list.o sound_manager.o sprite.o system.o text_area.o text_input_box.o texture_manager.o timer.o user_profile.o
run: $(OBJ)
	$(CXX) $(LDFLAGS) $(LIBS) $(OBJ) -o $@ -g

about_state.o: about_state.cpp about_state.h
	$(CXX) $< $(CFLAGS)
animator.o: animator.cpp animator.h
	$(CXX) $< $(CFLAGS)
background.o: background.cpp background.h
	$(CXX) $< $(CFLAGS)
beatmap.o: beatmap.cpp beatmap.h
	$(CXX) $< $(CFLAGS)
button.o: button.cpp button.h sprite.h
	$(CXX) $< $(CFLAGS)
column.o: column.cpp column.h
	$(CXX) $< $(CFLAGS)
control_handler.o: control_handler.cpp control_handler.h
	$(CXX) $< $(CFLAGS)
file_system.o: file_system.cpp file_system.h
	$(CXX) $< $(CFLAGS)
gameplay_wizard_state.o: gameplay_wizard_state.cpp gameplay_wizard_state.h
	$(CXX) $< $(CFLAGS)
game_header.o: game_header.cpp game_header.h
	$(CXX) $< $(CFLAGS)
game_state.o: game_state.cpp game_state.h
	$(CXX) $< $(CFLAGS)
list.o: list.cpp list.h
	$(CXX) $< $(CFLAGS)
list_widget.o: list_widget.cpp list_widget.h
	$(CXX) $< $(CFLAGS)
loading_state.o: loading_state.cpp loading_state.h state.h
	$(CXX) $< $(CFLAGS)
main.o: main.cpp
	$(CXX) $< $(CFLAGS)
main_state.o: main_state.cpp main_state.h state.h
	$(CXX) $< $(CFLAGS)
mod_widget.o: mod_widget.cpp mod_widget.h
	$(CXX) $< $(CFLAGS)
prepare_header.o: prepare_header.cpp prepare_header.h
	$(CXX) $< $(CFLAGS)
prepare_state.o: prepare_state.cpp prepare_state.h state.h
	$(CXX) $< $(CFLAGS)
result_state.o: result_state.cpp result_state.h state.h
	$(CXX) $< $(CFLAGS)
setting.o: setting.cpp setting.h
	$(CXX) $< $(CFLAGS)
shape.o: shape.cpp shape.h
	$(CXX) $< $(CFLAGS)
song_list.o: song_list.cpp song_list.h
	$(CXX) $< $(CFLAGS)
sound_manager.o: sound_manager.cpp sound_manager.h
	$(CXX) $< $(CFLAGS)
sprite.o: sprite.cpp sprite.h
	$(CXX) $< $(CFLAGS)
system.o: system.cpp system.h
	$(CXX) $< $(CFLAGS)
text_area.o: text_area.cpp text_area.h
	$(CXX) $< $(CFLAGS)
text_input_box.o: text_input_box.cpp text_input_box.h button.h
	$(CXX) $< $(CFLAGS)
texture_manager.o: texture_manager.cpp texture_manager.h
	$(CXX) $< $(CFLAGS)
timer.o: timer.cpp timer.h
	$(CXX) $< $(CFLAGS)
user_profile.o: user_profile.cpp user_profile.h
	$(CXX) $< $(CFLAGS)
LDFLAGS	:= `sdl2-config --cflags`
CFLAGS	:= -O0 -c -g
LIBS	:= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf
VPATH	:= main : select : game : loading : user

all: run

OBJ	:= beatmap.o button.o column.o control_handler.o character.o file_system.o game_state.o loading_state.o main.o main_state.o message_box.o select_state.o song_list.o song_header.o sound_manager.o sprite.o system.o text_area.o texture_manager.o timer.o
run: $(OBJ)
	$(CXX) $(LDFLAGS) $(LIBS) $(OBJ) -O0 -o $@

#about_sidedialog.o: main/about_sidedialog.cpp main/about_sidedialog.h
#	$(CXX) main/about_sidedialog.cpp $(CFLAGS)
beatmap.o: beatmap.cpp beatmap.h
	$(CXX) $< $(CFLAGS)
button.o: button.cpp button.h sprite.h
	$(CXX) $< $(CFLAGS)
character.o: character.cpp character.h
	$(CXX) $< $(CFLAGS)
column.o: column.cpp column.h
	$(CXX) $< $(CFLAGS)
control_handler.o: control_handler.cpp control_handler.h
	$(CXX) $< $(CFLAGS)
#crafting_state.o: crafting/crafting_state.cpp crafting/crafting_state.h state.h
#	$(CXX) crafting/crafting_state.cpp $(CFLAGS)
#debug_widget.o: debug_widget.cpp debug_widget.h
#	$(CXX) debug_widget.cpp $(CFLAGS)
file_system.o: file_system.cpp file_system.h
	$(CXX) $< $(CFLAGS)
game_state.o: game_state.cpp game_state.h
	$(CXX) $< $(CFLAGS)
loading_state.o: loading_state.cpp loading_state.h state.h
	$(CXX) $< $(CFLAGS)
main.o: main.cpp
	$(CXX) $< $(CFLAGS)
main_state.o: main_state.cpp main_state.h state.h
	$(CXX) $< $(CFLAGS)
message_box.o: message_box.cpp message_box.h
	$(CXX) $< $(CFLAGS)
select_state.o: select_state.cpp select_state.h state.h
	$(CXX) $< $(CFLAGS)
song_list.o: song_list.cpp song_list.h
	$(CXX) $< $(CFLAGS)
song_header.o: song_header.cpp song_header.h
	$(CXX) $< $(CFLAGS)
sound_manager.o: sound_manager.cpp sound_manager.h
	$(CXX) $< $(CFLAGS)
sprite.o: sprite.cpp sprite.h
	$(CXX) $< $(CFLAGS)
system.o: system.cpp system.h
	$(CXX) $< $(CFLAGS)
text_area.o: text_area.cpp text_area.h
	$(CXX) $< $(CFLAGS)
texture_manager.o: texture_manager.cpp texture_manager.h
	$(CXX) $< $(CFLAGS)
timer.o: timer.cpp timer.h
	$(CXX) $< $(CFLAGS)
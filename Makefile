LDFLAGS	:= `sdl2-config --cflags`
CFLAGS := -O0 -c -g
LIBS	:= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf

all: run

OBJ	:= button.o control_handler.o loading_state.o main.o main_state.o select_state.o song_list.o song_header.o sound_manager.o sprite.o system.o text_area.o texture_manager.o timer.o
run: $(OBJ)
	$(CXX) $(LDFLAGS) $(LIBS) $(OBJ) -O0 -o run

#about_sidedialog.o: main/about_sidedialog.cpp main/about_sidedialog.h
#	$(CXX) main/about_sidedialog.cpp $(CFLAGS)
button.o: button.cpp button.h sprite.h
	$(CXX) button.cpp $(CFLAGS)
control_handler.o: control_handler.cpp control_handler.h
	$(CXX) control_handler.cpp $(CFLAGS)
#crafting_state.o: crafting/crafting_state.cpp crafting/crafting_state.h state.h
#	$(CXX) crafting/crafting_state.cpp $(CFLAGS)
#debug_widget.o: debug_widget.cpp debug_widget.h
#	$(CXX) debug_widget.cpp $(CFLAGS)
loading_state.o: loading/loading_state.cpp loading/loading_state.h state.h
	$(CXX) loading/loading_state.cpp $(CFLAGS)
main.o: main.cpp
	$(CXX) main.cpp $(CFLAGS)
main_state.o: main/main_state.cpp main/main_state.h state.h
	$(CXX) main/main_state.cpp $(CFLAGS)
select_state.o: select/select_state.cpp select/select_state.h state.h
	$(CXX) select/select_state.cpp $(CFLAGS)
song_list.o: select/song_list.cpp select/song_list.h
	$(CXX) select/song_list.cpp $(CFLAGS)
song_header.o: select/song_header.cpp select/song_header.h
	$(CXX) select/song_header.cpp $(CFLAGS)
sound_manager.o: sound_manager.cpp sound_manager.h
	$(CXX) sound_manager.cpp $(CFLAGS)
sprite.o: sprite.cpp sprite.h
	$(CXX) sprite.cpp $(CFLAGS)
system.o: system.cpp system.h
	$(CXX) system.cpp $(CFLAGS)
text_area.o: text_area.cpp text_area.h
	$(CXX) text_area.cpp $(CFLAGS)
texture_manager.o: texture_manager.cpp texture_manager.h
	$(CXX) texture_manager.cpp $(CFLAGS)
timer.o: timer.cpp timer.h
	$(CXX) timer.cpp $(CFLAGS)
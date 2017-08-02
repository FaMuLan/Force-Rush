LDFLAGS	:= `sdl2-config --cflags`
CFLAGS := -O0 -c -g
LIBS	:= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf

HEADER	:= button.h control_handler.h debug_widget.h sound_manager.h sprite.h state.h system.h texture_manager.h timer.h main/main_state.h main/about_sidedialog.h crafting/crafting_state.h select/select_state.h select/song_list.h  loading/loading_state.h

all: run

run: about_sidedialog.o button.o control_handler.o crafting_state.o debug_widget.o loading_state.o main.o main_state.o select_state.o song_list.o sound_manager.o sprite.o state.o system.o texture_manager.o timer.o
	$(CXX) $(LDFLAGS) $(LIBS) about_sidedialog.o button.o control_handler.o crafting_state.o debug_widget.o loading_state.o main.o main_state.o select_state.o song_list.o sound_manager.o sprite.o state.o system.o texture_manager.o timer.o -O2 -o run

about_sidedialog.o: main/about_sidedialog.cpp $(HEADER)
	$(CXX) main/about_sidedialog.cpp $(CFLAGS)
button.o: button.cpp $(HEADER)
	$(CXX) button.cpp $(CFLAGS)
control_handler.o: control_handler.cpp $(HEADER)
	$(CXX) control_handler.cpp $(CFLAGS)
crafting_state.o: crafting/crafting_state.cpp $(HEADER)
	$(CXX) crafting/crafting_state.cpp $(CFLAGS)
debug_widget.o: debug_widget.cpp $(HEADER)
	$(CXX) debug_widget.cpp $(CFLAGS)
loading_state.o: loading/loading_state.cpp $(HEADER)
	$(CXX) loading/loading_state.cpp $(CFLAGS)
main.o: main.cpp
	$(CXX) main.cpp $(CFLAGS)
main_state.o: main/main_state.cpp $(HEADER)
	$(CXX) main/main_state.cpp $(CFLAGS)
select_state.o: select/select_state.cpp $(HEADER)
	$(CXX) select/select_state.cpp $(CFLAGS)
song_list.o: select/song_list.cpp $(HEADER)
	$(CXX) select/song_list.cpp $(CFLAGS)
sound_manager.o: sound_manager.cpp $(HEADER)
	$(CXX) sound_manager.cpp $(CFLAGS)
sprite.o: sprite.cpp $(HEADER)
	$(CXX) sprite.cpp $(CFLAGS)
state.o: state.cpp $(HEADER)
	$(CXX) state.cpp $(CFLAGS)
system.o: system.cpp $(HEADEE)
	$(CXX) system.cpp $(CFLAGS)
texture_manager.o: texture_manager.cpp $(HEADER)
	$(CXX) texture_manager.cpp $(CFLAGS)
timer.o: timer.cpp $(HEADER)
	$(CXX) timer.cpp $(CFLAGS)
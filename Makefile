LDFLAGS	:= `sdl2-config --cflags`
CFLAGS := -O0 -c -g
LIBS	:= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf

HEADER	:= button.h control_handler.h sound_manager.h sprite.h state.h system.h texture_manager.h timer.h main/main_state.h crafting/crafting_state.h loading/loading_state.h

all: run

run: button.o control_handler.o crafting_state.o loading_state.o main.o main_state.o sound_manager.o sprite.o state.o system.o texture_manager.o timer.o
	$(CXX) $(LDFLAGS) $(LIBS) button.o control_handler.o crafting_state.o loading_state.o main.o main_state.o sound_manager.o sprite.o state.o system.o texture_manager.o timer.o -O2 -o run

button.o: button.cpp $(HEADER)
	$(CXX) button.cpp $(CFLAGS)
control_handler.o: control_handler.cpp $(HEADER)
	$(CXX) control_handler.cpp $(CFLAGS)
crafting_state.o: crafting/crafting_state.cpp $(HEADER)
	$(CXX) crafting/crafting_state.cpp $(CFLAGS)
loading_state.o: loading/loading_state.cpp $(HEADER)
	$(CXX) loading/loading_state.cpp $(CFLAGS)
main.o: main.cpp
	$(CXX) main.cpp $(CFLAGS)
main_state.o: main/main_state.cpp $(HEADER)
	$(CXX) main/main_state.cpp $(CFLAGS)
sound_manager.o: sound_manager.cpp $(HEADER)
	$(CXX) sound_manager.cpp $(CFLAGS)
sprite.o: sprite.cpp $(HEADER)
	$(CXX) sprite.cpp $(CFLAGS)
state.o: state.cpp $(HEADER)
	$(CXX) state.cpp $(CFLAGS)
system.o: system.cpp $(HEADEE)
	$(CXX) system.cpp $(CFLAGS)
#template_map.o: template_map.cpp template_map.h
#	$(CXX) template_map.cpp -O0 -c -g
texture_manager.o: texture_manager.cpp $(HEADER)
	$(CXX) texture_manager.cpp $(CFLAGS)
timer.o: timer.cpp $(HEADER)
	$(CXX) timer.cpp $(CFLAGS)
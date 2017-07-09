LDFLAGS	:= `sdl2-config --cflags`
LIBS	:= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf

HEADER	:= button.h control_handler.h sound_manager.h sprite.h state.h system.h texture_manager.h timer.h main/main_state.h crafting/crafting_state.h loading/loading_state.h

all: run

run: button.o control_handler.o crafting_state.o loading_state.o main.o main_state.o sound_manager.o sprite.o state.o system.o texture_manager.o timer.o
	$(CXX) $(LDFLAGS) $(LIBS) button.o control_handler.o crafting_state.o loading_state.o main.o main_state.o sound_manager.o sprite.o state.o system.o texture_manager.o timer.o -O0 -o run

button.o: button.cpp $(HEADER)
	$(CXX) button.cpp -O0 -c -g
control_handler.o: control_handler.cpp $(HEADER)
	$(CXX) control_handler.cpp -O0 -c -g
crafting_state.o: crafting/crafting_state.cpp $(HEADER)
	$(CXX) crafting/crafting_state.cpp -O0 -c -g
loading_state.o: loading/loading_state.cpp $(HEADER)
	$(CXX) loading/loading_state.cpp -O0 -c -g
main.o: main.cpp
	$(CXX) main.cpp -O0 -c -g
main_state.o: main/main_state.cpp $(HEADER)
	$(CXX) main/main_state.cpp -O0 -c -g
sound_manager.o: sound_manager.cpp $(HEADER)
	$(CXX) sound_manager.cpp -O0 -c -g
sprite.o: sprite.cpp $(HEADER)
	$(CXX) sprite.cpp -O0 -c -g
state.o: state.cpp $(HEADER)
	$(CXX) state.cpp -O0 -c -g
system.o: system.cpp $(HEADEE)
	$(CXX) system.cpp -O0 -c -g
#template_map.o: template_map.cpp template_map.h
#	$(CXX) template_map.cpp -O0 -c -g
texture_manager.o: texture_manager.cpp $(HEADER)
	$(CXX) texture_manager.cpp -O0 -c -g
timer.o: timer.cpp $(HEADER)
	$(CXX) timer.cpp -O0 -c -g
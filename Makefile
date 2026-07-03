CXX = g++
build:
	$(CXX)  src/logic.cpp src/moveEvaluator.cpp  src/input.cpp src/TerminalDrawer.cpp src/TerminalRenderer.cpp -o chess_engine -I include -static

clean:
	rm chess_engine	

	
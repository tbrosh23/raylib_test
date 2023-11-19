hellomake : helloWorld.cpp dynamicsFunction.cpp
	g++ -o hellomake helloWorld.cpp dynamicsFunction.cpp -lraylib -lopengl32 -lgdi32 -lwinmm
default:
	@g++ -o raytracing main.cpp -lGL -lGLU -lglut

clear:
	@> raytracing && rm raytracing

run:
	@./raytracing

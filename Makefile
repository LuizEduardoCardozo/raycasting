default:
	@g++ -o raytracing main.cpp -lGL -lGLU -lglut

lint:
	@clang-tidy -header-filter=.* -checks=.-* -fix main.cpp -- -std=c++17

clear:
	@> raytracing && rm raytracing

run:
	@./raytracing

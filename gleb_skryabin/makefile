all:	
	clear
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp
	clang++ main.cpp -o run -std=c++17 -Werror
	./run

	python3 Pods/vis.py

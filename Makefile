b: MainWin.cpp
	g++ MainWin.cpp include/Application.cpp include/Camera.cpp include/CameraController.cpp include/Graphics.cpp include/Funclib.cpp include/InputUtility.cpp include/Matrix.cpp -o main.out -L/usr/lib/x86_64-linux-gnu -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -Wall -g -pthread

r: MainWin.cpp
	./main.out

br: MainWin.cpp
	make b && make r
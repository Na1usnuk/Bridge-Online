sudo g++ PythonWrapper.cpp -o BridgeGame.so -I/usr/include/python3.13 -L/usr/lib -L/lib -lboost_python3 -lpython3 --shared -fPIC

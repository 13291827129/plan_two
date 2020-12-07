CC=g++
LIB=-ljsoncpp -lpaho-mqtt3c -lpthread -lwiringPi
OBJ= Serialtools.o Analyase_tools.o Mqtt_tools.o main.o
BIN=MQTT.bin
FLAH=-Wall -Werror -DDEBUG

all:$(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(BIN)
%.o:%.cpp
	$(CC)  -c $< -o $@
clean:
	rm $(OBJ) $(BIN) 	

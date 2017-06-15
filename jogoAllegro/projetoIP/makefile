serverFile=examples/simpleChatServer
clientFile=tronco/game
serverName=server
clientName=client

all: compServer compClient

server: compServer runServer

client: compClient runClient

compClient:
	gcc -std=c99 -o $(clientName) $(clientFile).c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf
 
compServer:
	gcc -std=c99 -o $(serverName) $(serverFile).c game.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf lib/server.c 

runClient:
	./$(clientName)

runServer:
	./$(serverName)

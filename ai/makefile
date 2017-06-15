serverFile=troncoServer
clientFile=troncoClient
serverName=SERVER
clientName=CLIENT
botName=PHANTOM
botFile=phantom

all: compBot runBot

server: compServer runServer

client: compClient runClient

bot: compBot runBot

compClient:
	gcc -std=c99 -o $(clientName) $(clientFile).c libAllegro/AllegroCore.c libSocket/client.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf 

compServer:
	gcc -std=c99 -o $(serverName) $(serverFile).c libAllegro/AllegroCore.c libSocket/server.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf 

runClient:
	./$(clientName)

runServer:
	./$(serverName)

compBot:
	gcc -std=c99 -o $(botName) $(botFile).c libAllegro/AllegroCore.c libSocket/client.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf 

runBot:
	./$(botName)


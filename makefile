serverFile=troncoServer
clientFile=troncoClient
serverName=SERVER
clientName=CLIENT
botName=PHANTOM
botFile=ai/phantom
highscoreFile = "libTronco/highscore.c"
dependencies="dependencies"
score="score"
dependencie_files="libTronco/allegroUsefull.c"


all:  compDependencies compScore compServer compClient

server: compScore compServer runServer

client: compScore compDependencies compClient runClient

bot: compDependencies compBot runBot

compClient:
	gcc -std=c99 -o $(clientName) $(clientFile).c $(dependencies) $(score) libAllegro/AllegroCore.c libSocket/client.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

compServer:
	gcc -std=c99 -o $(serverName) $(serverFile).c $(score) libSocket/server.c

runClient:
	./$(clientName)

runServer:
	./$(serverName)

compBot:
	gcc -std=c99 -o $(botName) $(botFile).c $(dependencies) libAllegro/AllegroCore.c libSocket/client.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf 

runBot:
	./$(botName)

# Dependências de pacotes para compilação
# até o momento só dependências de clientes.
compDependencies:
	gcc -c $(dependencie_files) -o dependencies
 
compScore:
	gcc -c $(highscoreFile) -o score


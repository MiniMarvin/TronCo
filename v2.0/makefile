serverFile=troncoServer
clientFile=troncoClient
serverName=SERVER
clientName=CLIENT
botName=PHANTOM
botFile=../ai/phantom
dependencies="dependencies"
dependencie_files="libTronco/allegroUsefull.c"


all:  compDependencies compServer compClient

server: compServer runServer

client: compDependencies compClient runClient

bot: compDependencies compBot runBot

compClient:
	gcc -std=c99 -o $(clientName) $(clientFile).c $(dependencies)  libAllegro/AllegroCore.c libSocket/client.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf 

compServer:
	# gcc -std=c99 -o $(serverName) $(serverFile).c libAllegro/AllegroCore.c libSocket/server.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf 
	gcc -std=c99 -o $(serverName) $(serverFile).c libAllegro/AllegroCore.c libSocket/server.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf 

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
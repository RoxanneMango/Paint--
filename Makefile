.PHONY : all run clean play linux

SFML = C:\Users\Roxanne\Desktop\STUDIE_HBO_ICT\Jaar_4\CPSE2\SFML-2.5.1

INCLUDE = -I$(SFML)/include
LIB = -L$(SFML)/lib -L$(SFML)/bin
LIBS = 	-lsfml-graphics -lsfml-window -lsfml-system
EXEC = main

all:
	@echo "Making object files. . ."
	@g++ -c *.cpp $(INCLUDE) -std=c++17
	@echo "Doing stuff with object files. . ."
	@g++ *.o -o $(EXEC) $(LIB) $(LIBS)
	@./$(EXEC)

clean: 
	@echo "Removing object files again. . ."
	@del *.o
	@del *.exe

play:
	@$(EXEC)
	
linux:
	@echo "Making object files. . ."
	@g++ -c *.cpp $(INCLUDE) -std=c++11 
	@echo "Doing stuff with object files. . ."
	@g++ *.o -o $(EXEC) $(LIB) $(LIBS)
	@echo "Removing object files again. . ."
	@rm *.o
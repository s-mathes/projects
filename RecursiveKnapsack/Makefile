pa5: main.o initializeInventory.o inventorySize.o valueOf.o maxVal.o maxValue.o
	gcc -g -o pa5 main.o initializeInventory.o inventorySize.o valueOf.o maxVal.o maxValue.o

main.o: main.c mylib.h
	gcc -g -c main.c

initializeInventory.o: initializeInventory.c mylib.h
	gcc -g -c initializeInventory.c

inventorySize.o: inventorySize.c mylib.h
	gcc -g -c inventorySize.c

valueOf.o: valueOf.c mylib.h
	gcc -g -c valueOf.c

maxVal.o: maxVal.c mylib.h
	gcc -g -c maxVal.c

maxValue.o: maxValue.c mylib.h
	gcc -g -c maxValue.c

clean:
	rm pa5 main.o initializeInventory.o inventorySize.o valueOf.o maxVal.o maxValue.o


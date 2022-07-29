make:
	rm -f roller.exe
	gcc main.c -o roller.exe
	./roller.exe
test:
	rm -f roller.exe
	gcc main.c -o roller.exe
	./roller.exe -t
clean:
	rm -f roller.exe
build:
	gcc main.c -o roller.exe
run:
	./roller.exe
run-test:
	./roller.exe -t
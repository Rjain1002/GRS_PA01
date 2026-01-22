CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lm

PART_A_SRC = MT25080_Part_A_Program_A.c MT25080_Part_A_workers.c
PART_B_SRC = MT25080_Part_B_Program_B.c MT25080_Part_A_workers.c
PART_A_EXE = MT25080_Part_A_Program_A
PART_B_EXE = MT25080_Part_B_Program_B

all: $(PART_A_EXE) $(PART_B_EXE)

$(PART_A_EXE): $(PART_A_SRC)
	$(CC) $(CFLAGS) -o $@ $(PART_A_SRC) $(LDFLAGS)

$(PART_B_EXE): $(PART_B_SRC)
	$(CC) $(CFLAGS) -o $@ $(PART_B_SRC) $(LDFLAGS)

clean:
	rm -f *.o $(PART_A_EXE) $(PART_B_EXE) temp_worker_*.dat *.png

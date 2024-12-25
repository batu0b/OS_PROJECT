#GRUP ADI: 12AB Grubu
#GRUP UYELERI
#Ömer Faruk Aydın - G221210370 - 1.Öğretim A Grubu
#Tarık Kartal - Y245012016 - 1. Öğretim A Grubu
#Harun Mahmut Yeşilyurt - G211210089 - 2. Öğretim B Grubu
#Batuhan Bahayetmez - G211210043 - 2. Öğretim A Grubu
#Talha İmran Göneç - G221210032 - 2. Öğretim B Grubu

# Derleyici ve bayraklar
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Dizinler
SRC_DIR = src
INC_DIR = include
LIB_DIR = lib
BIN_DIR = bin

# Hedefler
TARGET = $(BIN_DIR)/Program
OBJS = $(LIB_DIR)/main.o $(LIB_DIR)/shell.o

# Tüm hedef
all: $(TARGET)
	@echo "Uygulama çalıştırılıyor..."
	@./$(TARGET)

# Uygulama oluştur
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# main.o oluştur
$(LIB_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/shell.h
	@mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/main.c -o $(LIB_DIR)/main.o

# shell.o oluştur
$(LIB_DIR)/shell.o: $(SRC_DIR)/shell.c $(INC_DIR)/shell.h
	@mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $(SRC_DIR)/shell.c -o $(LIB_DIR)/shell.o

# Temizlik işlemi
clean:
	rm -rf $(LIB_DIR)/*.o $(TARGET)

# Daha detaylı temizleme
clean_all:
	rm -rf $(LIB_DIR) $(BIN_DIR)

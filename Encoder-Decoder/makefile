# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -g

# Исполняемый файл
TARGET = encode

# Исходные файлы и автоматическое получение объектных
SRCS = encode-decode.c
OBJS = $(SRCS:.c=.o)

# Удаление объектного файла после линковки, не ломая инкрементальную компиляцию
.INTERMEDIATE: $(OBJS)

# Цель по умолчанию
all: $(TARGET)

# Линковка
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(FLAGS)

# Компиляция .c -> .o (шаблонное правило)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка проекта
clean:
	rm -f $(OBJS) $(TARGET)

# Указываем, что all и clean не являются файлами
.PHONY: all clean

CC 			:= gcc
# Banderas del compilador. '-Iinclude' incluye las cabeceras desde la raíz de include/ sin rutas relativas.
# Agregar '-g' para símbolos de depuración (GDB).
CFLAGS      := -Wall -Wextra -Iinclude -g
SRC_DIR     := src
OBJ_DIR     := bin
TARGET_NAME := sentinel-dispatcher
TARGET      := $(OBJ_DIR)/$(TARGET_NAME)

# Lista de código fuente (archivos .c).
SRCS := $(shell find $(strip $(SRC_DIR)) -name '*.c')
# Transforma cada 'src/.../*.c' en su respectivo 'bin/.../*.o'.
# Conserva la arquitectura entre archivos fuentes y sus respectivos objetos.
OBJS := $(patsubst $(strip $(SRC_DIR))/%.c, $(strip $(OBJ_DIR))/%.o, $(SRCS))

all: $(TARGET) 

$(TARGET) : $(OBJS)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(OBJS) -o $(TARGET)

# Regla de patrón. Compila cada '.c' en su correspondiente '.o' dentro de bin/.
# $@ representa el nombre del archivo objetivo que se está generando (ej. bin/core/main.o)
# $< representa el nombre del primer prerrequisito (ej. src/core/main.c)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) 
	@$(CC) $(CFLAGS) -c $< -o $@

.SECONDARY: $(OBJS)

# Objetos falsos. No representan archivos reales, sino acciones de utilidad.
.PHONY: all clean run print-vars
clean:
	@rm -rf $(OBJ_DIR)

run: $(TARGET)
	@./$(TARGET) $(ARGS)

print-vars:
	@echo "SRCS -> $(SRCS)"
	@echo "OBJS -> $(OBJS)"
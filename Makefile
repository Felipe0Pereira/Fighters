# Compilador e flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iinclude $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --cflags)
LDFLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --libs)

# Diretórios
SRCDIR = src
OBJDIR = obj

# Arquivos fonte e objetos
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Executável final
EXEC = AS

# Regra padrão
all: $(EXEC)

# Linkagem
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilação
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação do diretório obj se não existir
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Limpeza
clean:
	rm -rf $(OBJDIR)

purge:
	rm -rf $(OBJDIR) $(EXEC)

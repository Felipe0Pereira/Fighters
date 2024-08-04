# Compilador e flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iinclude $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --cflags)
LDFLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --libs)

# Diretorios
SRCDIR = src
OBJDIR = obj

# Arquivos fonte e objetos
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Executavel final
EXEC = LeakFighters

# Regra padrao
all: $(EXEC)

# Linkagem
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilacao
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Cria o diretorio obj se nao existir
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Limpa objetos
clean:
	rm -rf $(OBJDIR)

# Limpa objetos e executavel
purge:
	rm -rf $(OBJDIR) $(EXEC)

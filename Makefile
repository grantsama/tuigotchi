CFLAGS = -std=c99 -Wall -Wextra
SRCDIR = src
PREFIX = /usr/local
INSTALL_DIR = $(PREFIX)/bin
CONFIG_DIR = /home/$(USER)/.config/tuigotchi

.PHONY: tuigotchi clean build install uninstall

# Target for building the executable
tuigotchi: $(SRCDIR)/main.c $(SRCDIR)/gotchi.o $(SRCDIR)/data.o $(SRCDIR)/render.o
	gcc $(CFLAGS) -o $@ $(SRCDIR)/main.c $(SRCDIR)/gotchi.o $(SRCDIR)/data.o $(SRCDIR)/render.o -lncursesw

$(SRCDIR)/data.o: $(SRCDIR)/data.h $(SRCDIR)/data.c $(SRCDIR)/gotchi.h
	gcc $(CFLAGS) -c $(SRCDIR)/data.c -o $(SRCDIR)/data.o

$(SRCDIR)/render.o: $(SRCDIR)/render.h $(SRCDIR)/render.c $(SRCDIR)/gotchi.h
	gcc $(CFLAGS) -c $(SRCDIR)/render.c -o $(SRCDIR)/render.o

$(SRCDIR)/gotchi.o: $(SRCDIR)/gotchi.h $(SRCDIR)/gotchi.c
	gcc $(CFLAGS) -c $(SRCDIR)/gotchi.c -o $(SRCDIR)/gotchi.o

# Clean up object files and executable
clean:
	rm -f $(SRCDIR)/*.o tuigotchi

# Install the executable and configuration files
build: tuigotchi
	mkdir -p $(CONFIG_DIR)/dat $(CONFIG_DIR)/art
	cp -a art/. $(CONFIG_DIR)/art/
	sudo mkdir -p $(INSTALL_DIR)
	sudo mv tuigotchi $(INSTALL_DIR)
	@printf "\nInstallation complete!\n"

# Uninstall the executable, configuration, and save files
uninstall: clean
	sudo rm -f $(INSTALL_DIR)/tuigotchi
	rm -rf $(CONFIG_DIR)
	@printf "\nUninstallation complete!\n"

# To build and clean all in one go
install: build clean

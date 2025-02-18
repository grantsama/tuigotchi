CFLAGS = -std=c99
SRCDIR = src
INSTALL_DIR = /usr/bin
CONFIG_DIR = /home/$(USER)/.config/tuigotchi

# Target for building the executable
tuigotchi: $(SRCDIR)/main.c $(SRCDIR)/gotchi.o $(SRCDIR)/data.o $(SRCDIR)/render.o
	gcc $(CFLAGS) -o $@ $(SRCDIR)/main.c $(SRCDIR)/gotchi.o $(SRCDIR)/data.o $(SRCDIR)/render.o

$(SRCDIR)/data.o: $(SRCDIR)/data.h $(SRCDIR)/data.c $(SRCDIR)/gotchi.o
	gcc $(CFLAGS) -c $(SRCDIR)/data.c -o $(SRCDIR)/data.o

$(SRCDIR)/render.o: $(SRCDIR)/render.h $(SRCDIR)/render.c $(SRCDIR)/gotchi.o
	gcc $(CFLAGS) -c $(SRCDIR)/render.c -o $(SRCDIR)/render.o

$(SRCDIR)/gotchi.o: $(SRCDIR)/gotchi.h $(SRCDIR).gotchi.c
	gcc $(CFLAGS) -c $(SRCDIR)/gotchi.c -o $(SRCDIR)/gotchi.o

# Clean up object files and executable
clean:
	rm -f $(SRCDIR)/*.o

# Install the executable and configuration files
build: $(SRCDIR)/main.c $(SRCDIR)/data.o
	mkdir -p $(CONFIG_DIR)/dat $(CONFIG_DIR)/gotchis
	cp -a dat/. $(CONFIG_DIR)/dat/
	cp -a gotchis/. $(CONFIG_DIR)/gotchis/
	sudo mv tuigotchi $(INSTALL_DIR)/tuigotchi
	@printf "\nInstallation complete!\n"

# Uninstall the executable and configuration files
uninstall:
	sudo rm -f $(INSTALL_DIR)/tuigotchi
	rm -rf $(CONFIG_DIR)
	@printf "\nUninstallation complete!\n"

# To build and clean all in one go
install: tuigotchi build clean

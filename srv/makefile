SRC = src/main.cpp
TARGET = chatd
SHALIB = sha256
CHLIB = chat
UBLIB = UserBase
BINPATH = /usr/local/bin
LIBPATH = /usr/lib
DATAPATH = /var/lib/Chat
#MBASE = msg_base.dat
#UBASE = users.dat

Chat:	$(SRC) libch libub libsha
		g++ -o $(TARGET) $(SRC) -L. -l$(CHLIB) -l$(UBLIB) -l$(SHALIB)

libch:	src/$(CHLIB).cpp src/$(CHLIB).h
		g++ -o lib$(CHLIB).so src/$(CHLIB).cpp -shared -fPIC

libub:	src/$(UBLIB).cpp src/$(UBLIB).h
		g++ -o lib$(UBLIB).so src/$(UBLIB).cpp -shared -fPIC

libsha: src/$(SHALIB).cpp src/$(SHALIB).h
		g++ -o lib$(SHALIB).so src/$(SHALIB).cpp -shared -fPIC

clean:
		rm -rf $(TARGET)
		rm -rf lib$(CHLIB).so
		rm -rf lib$(UBLIB).so
		rm -rf lib$(SHALIB).so

install:
		install $(TARGET) $(BINPATH)
		install lib$(CHLIB).so $(LIBPATH)
		install lib$(UBLIB).so $(LIBPATH)
		install lib$(SHALIB).so $(LIBPATH)
		mkdir $(DATAPATH) && chmod 777 $(DATAPATH)
#		cp $(UBASE) $(DATAPATH) && chmod 777 $(DATAPATH)/$(UBASE)
#		cp $(MBASE) $(DATAPATH) && chmod 777 $(DATAPATH)/$(MBASE)

uninstall:
		rm -rf $(BINPATH)/$(TARGET)
		rm -rf $(LIBPATH)/lib$(CHLIB).so
		rm -rf $(LIBPATH)/lib$(UBLIB).so
		rm -rf $(LIBPATH)/lib$(SHALIB).so
		rm -rf $(DATAPATH)

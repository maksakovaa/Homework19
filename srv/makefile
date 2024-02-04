SRC = src/main.cpp
TARGET = chatd
SHALIB = sha256d
CHLIB = chatd
UBLIB = UserBased
NLIB = netd
BINPATH = /usr/local/bin
LIBPATH = /usr/lib
DATAPATH = /var/lib/chatd

Chat:	$(SRC) libnet libch libub libsha
		g++ -o $(TARGET) $(SRC) -L. -l$(NLIB) -l$(CHLIB) -l$(UBLIB) -l$(SHALIB)

libnet:	src/$(NLIB).cpp src/$(NLIB).h
		g++ -o lib$(NLIB).so src/$(NLIB).cpp -shared -fPIC

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
		rm -rf lib$(NLIB).so

install:
		install $(TARGET) $(BINPATH)
		install lib$(CHLIB).so $(LIBPATH)
		install lib$(UBLIB).so $(LIBPATH)
		install lib$(SHALIB).so $(LIBPATH)
		install lib$(NLIB).so $(LIBPATH)
		mkdir $(DATAPATH) && chmod 777 $(DATAPATH)

uninstall:
		rm -rf $(BINPATH)/$(TARGET)
		rm -rf $(LIBPATH)/lib$(CHLIB).so
		rm -rf $(LIBPATH)/lib$(UBLIB).so
		rm -rf $(LIBPATH)/lib$(SHALIB).so
		rm -rf $(LIBPATH)/lib$(NLIB).so
		rm -rf $(DATAPATH)

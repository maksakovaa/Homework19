SRC = src/main.cpp
TARGET = Chat
SHALIB = sha256
CHLIB = chat
UBLIB = UserBase
NAVLIB = navigation
NETLIB = net
FUNLIB = functions
BINPATH = /usr/local/bin
LIBPATH = /usr/lib
DATAPATH = /var/lib/Chat

Chat:	$(SRC) libnav libfun libch libub libnet libcrc libsha
		g++ -o $(TARGET) $(SRC) -L. -l$(NAVLIB) -l$(FUNLIB) -l$(CHLIB) -l$(UBLIB) -l$(NETLIB) -l$(SHALIB)

libnav: src/$(NAVLIB).cpp src/$(NAVLIB).h
		g++ -o lib$(NAVLIB).so src/$(NAVLIB).cpp -shared -fPIC

libfun:	src/$(FUNLIB).cpp src/$(FUNLIB).h
		g++ -o lib$(FUNLIB).so src/$(FUNLIB).cpp -shared -fPIC

libch:	src/$(CHLIB).cpp src/$(CHLIB).h
		g++ -o lib$(CHLIB).so src/$(CHLIB).cpp -shared -fPIC

libub:	src/$(UBLIB).cpp src/$(UBLIB).h
		g++ -o lib$(UBLIB).so src/$(UBLIB).cpp -shared -fPIC

libnet:	src/$(NETLIB).cpp src/$(NETLIB).h
		g++ -o lib$(NETLIB).so src/$(NETLIB).cpp -shared -fPIC

libsha: src/$(SHALIB).cpp src/$(SHALIB).h
		g++ -o lib$(SHALIB).so src/$(SHALIB).cpp -shared -fPIC

clean:
		rm -rf $(TARGET)
		rm -rf lib$(NAVLIB).so
		rm -rf lib$(FUNLIB).so
		rm -rf lib$(CHLIB).so
		rm -rf lib$(UBLIB).so
		rm -rf lib$(NETLIB).so
		rm -rf lib$(SHALIB).so

install:
		install $(TARGET) $(BINPATH)
		install lib$(NAVLIB).so $(LIBPATH)
		install lib$(FUNLIB).so $(LIBPATH)
		install lib$(CHLIB).so $(LIBPATH)
		install lib$(UBLIB).so $(LIBPATH)
		install lib$(NETLIB).so $(LIBPATH)
		install lib$(SHALIB).so $(LIBPATH)
		mkdir $(DATAPATH) && chmod 777 $(DATAPATH)

uninstall:
		rm -rf $(BINPATH)/$(TARGET)
		rm -rf $(LIBPATH)/lib$(NAVLIB).so
		rm -rf $(LIBPATH)/lib$(FUNLIB).so
		rm -rf $(LIBPATH)/lib$(CHLIB).so
		rm -rf $(LIBPATH)/lib$(UBLIB).so
		rm -rf $(LIBPATH)/lib$(NETLIB).so
		rm -rf $(LIBPATH)/lib$(SHALIB).so
		rm -rf $(DATAPATH)

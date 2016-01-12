CXXFLAGS += -O2 -Wall -g -pthread

all: clim8

clim8: RCSwitch.o RcOok.o Sensor.o main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi

clean:
	$(RM) *.o clim8

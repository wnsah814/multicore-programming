CXX = gcc
CXXFLAGS = -O0 -pthread

TARGET = tas ttas mutex backoff\
				 atomic

all: bin $(TARGET)

bin: 

$(TARGET):
	$(CXX) $(CXXFLAGS) -o $@ $@.c

clean:
	rm $(TARGET)
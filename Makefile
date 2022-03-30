OBJECTS := tscc_main.o tscc_lexer.o
TARGET := tscc
GPP := g++
RM := rm

%.o : %.cpp
	$(GPP) -std=c++2a -c -o $@ $^

$(TARGET): $(OBJECTS)
	$(GPP) -o $@ $^

.PHONY: build clean
build:
	$(MAKE) $(TARGET)

clean:
	$(RM) $(OBJECTS) $(TARGET)
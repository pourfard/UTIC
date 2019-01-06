FILE = config.cpp exact.cpp GraphParser.cpp integrator.cpp main.cpp selection.cpp
PARSER_FILE = parser.cpp

bitcnts: ${FILE} Makefile
	g++ -std=c++0x ${FILE} -o exactui
	g++ -std=c++0x ${PARSER_FILE} -o parser

clean:
	rm -rf exactui parser output* BasicBlocks*

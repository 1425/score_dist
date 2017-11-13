CXX=g++-7
CXXFLAGS=-std=c++17 -Wall -Werror

run: score_spread
	./score_spread && ./surf.py

score_spread: score_spread.cpp ../tba/db.o ../tba/curl.o ../tba/data.o ../tba/rapidjson.o ../tba/util.o
	$(CXX) $(CXXFLAGS) $^ -lsqlite3 -lcurl -o $@

clean:
	rm -f score_spread *.png *.pgm *.csv

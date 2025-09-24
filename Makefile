libSupportFiles != find ./ -type f -name '*.c*' -not -path "./Tests/*"

libSupportHeaders != find ./ -type f -name '*.h' -not -path "./Tests/*"

libSupportLibs = `pkg-config --libs --cflags icu-uc icu-io` -lz -lspdlog -lfmt

libSupportCFLAGS = -Wextra -Wall -Wpedantic -shared -fPIC -std=c++23

libsupport-d.so: $(libSupportFiles) $(libSupportHeaders)
	g++ $(libSupportFiles) ${libSupportCFLAGS} ${libSupportLibs} -pg -g -o libsupport-d.so

libsupport.so: $(libSupportFiles) $(libSupportHeaders)
	g++ $(libSupportFiles) ${libSupportCFLAGS} ${libSupportLibs} -O4 -o libsupport.so

testFilesCPP != find ./Tests/ -type f -name '*.cpp'

testFilesHS != find ./Tests/ -type f -name '*.hs'

linkLibSupport = -lsupport -L.

testCFLAGS = -Wextra -Wall -Wpedantic -std=c++23 -lz -lgtest

tests-debug: libsupport-d.so ${testFilesCPP}
	g++ ${testFilesCPP} ${testCFLAGS} -lspdlog -lfmt -L. -lsupport-d -g -pg -o tests-debug
	LD_LIBRARY_PATH=. time -v ./tests-debug
	gprof tests gmon.out > analisys.txt

tests: libsupport.so ${testFilesCPP}
	g++ ${testFilesCPP} ${testCFLAGS} -lspdlog -lfmt ${linkLibSupport} -O4 -o tests
	LD_LIBRARY_PATH=. time -v ./tests

clean:
	rm -fv *.so
	rm -fv test*
	rm -fv analisys*.txt
	rm -fv *.eventlog
	rm -fv gmon.out
	rm -fv *.dot
	rm -fv *.svg
	rm -fv *.log

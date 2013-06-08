all: prepare build test

prepare:
	git submodule update --init
	mkdir -p ./3d_party/libjingle; cd ./3d_party/libjingle; ../depot_tools/gclient config http://libjingle.googlecode.com/svn/trunk; ../depot_tools/gclient sync --force >> /dev/null;
	npm install

build:
	./node_modules/.bin/node-gyp configure
	./node_modules/.bin/node-gyp build

clean:
	rm -rf ./build/test/client.test.run
	./node_modules/.bin/node-gyp clean

.PHONY: build clean prepare test

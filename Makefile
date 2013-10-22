UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	NPM = npm_config_arch="ia32" npm
	GYP = ./node_modules/.bin/node-gyp --arch=i386
	NODE = arch --i386 node
	GENERATORS = make
	BUILD_CMD = export LDFLAGS=-L/usr/local/opt/openssl/lib; export CPPFLAGS=-I/usr/local/opt/openssl/include; make BUILDTYPE=Release libjingle_peerconnection libjingle_p2p libjingle_media libjingle_sound libjingle voice_engine video_engine_core webrtc_utility audio_conference_mixer audio_processing audio_processing_sse2 audio_device video_processing video_processing_sse2 video_render_module remote_bitrate_estimator rbe_components rtp_rtcp acm2 audio_coding_module NetEq webrtc_opus G711 iSAC iLBC CNG audioproc_debug_proto NetEq4 G722 PCM16B common_audio libsrtp opus protobuf_lite system_wrappers
else
	NPM = npm
	GYP = ./node_modules/.bin/node-gyp
	NODE = node
	GENERATORS = ninja
	BUILD_CMD = ninja -C out/Release peerconnection_client
endif

all: deps test

prepare_env: 
	mkdir -p third_party; cd third_party; if [ ! -d "depot_tools" ]; then git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git; fi
	cd third_party; ./depot_tools/gclient config http://webrtc.googlecode.com/svn/trunk/; GYP_GENERATORS=$(GENERATORS) ./depot_tools/gclient sync --force;

build_deps:
	cd third_party/trunk; $(BUILD_CMD)

deps:
	$(NPM) install

build:
	$(GYP) configure
	./node_modules/.bin/node-gyp build -v

test:
	$(NODE) ./node_modules/.bin/_mocha test/*.test.js
	./node_modules/.bin/karma  start confs/karma.conf.js --single-run

docs:
	git submodule update --init
	cd third_party/doxygen; ./configure; make
	./third_party/doxygen/bin/doxygen ./confs/docs.conf
	open docs/html/index.html

clean:
	rm -rf third_party
	rm -rf docs
	cd src; ../node_modules/.bin/node-gyp clean
	rm -rf node_modules

.PHONY: prepare_env deps build test clean docs check_os

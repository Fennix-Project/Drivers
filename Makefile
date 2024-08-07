build:
	cp -rf ../Kernel/include/interface/* include/
	mkdir -p out
	make -C library build
	make -C audio build
	make -C input build
	make -C misc build
	make -C network build
	make -C storage build
	make -C filesystem build

prepare:
	$(info Nothing to prepare)

clean:
	rm -rf out
	make -C library clean
	make -C audio clean
	make -C input clean
	make -C misc clean
	make -C network clean
	make -C storage clean
	make -C filesystem clean

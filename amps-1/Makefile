build:
	docker build --rm -t local/c7-amps-empty .

build-ubuntu:
	docker build --rm -t local/ubuntu-amps-empty -f Dockerfile.ubuntu .

mkconfig:
	@echo "Create sample config.xml now:"
	@echo "/opt/amps/bin/ampServer --sample-config > /Downloads/config-sample.xml"
	docker run -it  -v ~/Downloads/:/Downloads local/c7-amps-empty bash

run:
	docker run -ti -d -v ~/Downloads/:/Downloads -p 8085:8085 -p 9007:9007 local/c7-amps-empty

run-ubuntu:
	docker run -ti -d -v ~/Downloads/:/Downloads -p 8085:8085 -p 9007:9007 local/ubuntu-amps-empty

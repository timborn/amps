build:
	docker build --rm -t local/c7-amps-empty .

mkconfig:
	docker run -ti -v ~/Downloads/:/Downloads -p 8085:8085 -p 9007:9007 local/c7-amps-empty

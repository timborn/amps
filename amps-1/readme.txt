Fri Oct 27 15:46:47 CDT 2017
----------------------------

https://docs.docker.com/get-started/part2/#apppy


docker --version
docker run hello-world
docker build -t friendlyhello .		# build and Tag the resulting Docker image
docker images				# shows list of Docker images
docker run -p 4000:80 friendlyhello	# maps 80 in image to localhost:4000
docker run -d -p 4000:80 friendlyhello	# run Detached (background) 
docker container ls			# shows IDs of running containers
docker container stop $ID		# stop a running container
docker login				# prepare to share
docker tag image username/repo:tag	# format for tagging
docker tag friendlyhello timborn/get-started:part2
docker push username/repo:tag		# format for pushing a tagged image
docker run -p 4000:80 username/repo:tag	# run from dockerhub
docker container ls -a			# show all containers, even ones not running
docker container rm $ID			# remove container from this machine
docker container rm $(docker container ls -a -q)         # Remove all containers - DANGER!
docker image ls -a			# list all images on this machine
docker image rm $(docker image ls -a -q)   # Remove all images from this machine - DANGER!


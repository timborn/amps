# script for putting docker through it's paces
# NB assumes Docker is running 

# this one has probably already been run
# docker build -t friendlyhello .

echo
echo "We have many docker images already."
echo "Press return to see the list."
read 

# should see friendlyhello in there
docker images

echo
echo "We will start 'friendlyhello', a python app that uses port 80."
echo "We bind docker port 80 to port 4000 on localhost."
echo "Press return when ready."
read 

# maps port 80 in docker image to port 4000 on localhost
# running in detached mode; use 'docker container stop $ID' to stop
docker run -d -p 4000:80 friendlyhello
# my machine is too fast!
sleep 1
open http://localhost:4000

# alternatively ...
# curl http://localhost:4000

echo 
echo "We can see our running docker containers."
echo "use 'docker container stop \$ID' to stop"
echo "Press return when ready."
read 

# which containers are running?
docker container ls


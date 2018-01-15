# https://blog.sixeyed.com/relay-iis-log-entries-to-read-them-in-docker/
TAG="###"

echo
echo $TAG "Build a Docker image"
echo docker build -t iis-with-log-relay .
read ans
docker build -t iis-with-log-relay .

echo
echo $TAG run the container
echo id = \$\( docker run -d -p 80:80 iis-with-log-relay \)
read ans
id=$( docker run -d -p 80:80 iis-with-log-relay )

echo $TAG docker container id=$id

echo
echo $TAG view IIS logfile
echo docker logs \$id
read ans
docker logs $id

echo
echo $TAG generate some load on our web service
echo ip = \$\(docker inspect --format '{{ .NetworkSettings.Networks.nat.IPAddress }}' \$id\)
echo for i in \$\( seq 1 10 \)\; do
echo "	wget -q -O /dev/null http://\$ip"
echo done
read ans
ip=$(docker inspect --format '{{ .NetworkSettings.Networks.nat.IPAddress }}' $id)

echo $TAG docker container ip = $ip

for i in $( seq 1 10 ); do
	wget -q -O /dev/null http://$ip
done

echo
echo $TAG review IIS logfile
echo docker logs \$id
read ans
docker logs $id

echo
echo $TAG flush the output buffer manually
echo docker exec \$id netsh http flush logbuffer
read ans
docker exec $id netsh http flush logbuffer

echo
echo $TAG review IIS logfile
echo docker logs \$id
read ans
docker logs $id

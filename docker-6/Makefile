run:
	docker run -d -p 5901:5901 -p 6901:6901 consol/centos-xfce-vnc

connect:
	echo password is \"vncpassword\"
	open vnc://localhost:5901

# wishful thinking when we select a container to kill
down:
	ID=$( docker container ls -l --format \"{{.ID}}\" ) ; \
	docker container stop $ID

Thu Jan 25 16:40:43 CST 2018
----------------------------
Can we run a development desktop in Docker?

a service
- web based
- authenticate against JPMC something or other
- collect ssh pub keys from users
- spin up dev environment, for a specific user, on demand
- this env does NOT have root access
- based on an approved image (probably CentOS 7)
- contains "standard" dev tools, correctly configured
- versioned containers, user can select
- NO persistant data in the image
- users $HOME can be mounted for persistance (how?)
- dev environments get nuked after being up for 5 days

Sun Jan 28 08:53:59 CST 2018
----------------------------
Start with this base:
# git clone https://github.com/ConSol/docker-headless-vnc-container.git
I decided to clone this to my github instead

Mon Jan 29 07:04:57 CST 2018
----------------------------
installed
git
enabled logging in docker
disabled Chrome
updated firefox
installed Intellij
added HOME customization

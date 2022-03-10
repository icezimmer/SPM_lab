# TO CONNECT TO UNIPI VPN

netExtender 
Server: access.unipi.it 
User: alice 
Password: alice 
Domain: 131.114.21.15, 131.114.21.25 

# TO CONNECT TO THE REMOTE MACHINE (ssh key just generated):

ssh -i ~/.ssh/spm_server unipi_username@131.114.22.228

# TO CONNECT WITH PASSWORD

ssh unipi_username@131.114.22.228

# GIT PULL:

cd SPM_lab/ 
git pull

# COMPILE (example):

g++ -O3 -o redseq redseq.cpp

# PARALLEL COMPILE:

g++ -O3 -pthread -o redpar redpar.cpp 

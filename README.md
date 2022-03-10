# To connect to unipi VPN

netExtender  
Server: access.unipi.it  
User: alice  
Password: alice  
Domain: 131.114.21.15, 131.114.21.25

# To connect to the remote machine (ssh key just generated):

ssh -i ~/.ssh/spm_server unipi_username@131.114.22.228

# To connect with user identification

ssh unipi_username@131.114.22.228

# Git pull:

cd SPM_lab/  
git pull

# Sequential compile (example):

g++ -O3 -o redseq redseq.cpp

# Parallel compile (example):

g++ -O3 -pthread -o redpar redpar.cpp 



# To connect to unipi VPN

## Launch ConnectTunnel (gui version)

startctui

### To connect

User: alice  
Password: alice  


# To connect to the remote machine (ssh key just generated):

ssh -i ~/.ssh/spm_server unipi_username@131.114.22.228

# To connect with user identification

ssh unipi_username@131.114.22.228

# Git pull:

cd SPM_lab/  
git pull

# Sequential compile (example):

g++ -O3 -o redseq redseq.cpp dependency_files

# Parallel compile (example):

g++ -O3 -pthread -o redpar redpar.cpp dependency_files

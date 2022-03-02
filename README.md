TO CONNECT TO THE REMOTE MACHINE (ssh key just generated)
ssh -i ~/.ssh/spm_server unipi_username@131.114.22.228

GIT PULL
cd SPM_lab/
git pull

COMPILE (example)
g++ -o redseq redseq.cpp

PARALLEL COMPILE
g++ -O3 -pthread -o redpar redpar.cpp 

# Useful commands

## To connect to unipi VPN

### Launch ConnectTunnel (gui version)

startctui

#### To connect

User: alice  
Password: alice  


## To connect to the remote machine (ssh key just generated):

ssh -i ~/.ssh/spm_server unipi_username@131.114.22.228

## To connect with user identification

ssh unipi_username@131.114.22.228

## Git pull:

cd SPM_lab/  
git pull
username git
password token

## Sequential compile in remote machone (example):

g++ -O3 -o redseq redseq.cpp dependency_files

## Parallel compile in remote machine (example):

g++ -O3 -pthread -o redpar redpar.cpp dependency_files

## Compile in local machine:

g++-11 ...

# Assignments:

# Assignment 1 (March 9):

Evaluate overhead needed to start/join a thread and to start an async (launch::async policy) and get the result of the computed future. Measures should be taken as averages of a large number of events. Possibly, measure times on your machine AND on the remote virtual machine.To be completed by next Tue, March 15 (ideally).

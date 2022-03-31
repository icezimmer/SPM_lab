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

## Assignment 1 (March 9):

Evaluate overhead needed to start/join a thread and to start an async (launch::async policy) and get the result of the computed future. Measures should be taken as averages of a large number of events. Possibly, measure times on your machine AND on the remote virtual machine.To be completed by next Tue, March 15 (ideally).

## Assignment 2 (March 16):

Provide a vector<float> map(vector<float>v, int mode, function<float(float)> f, int pardegree) computing in parallel (using C++ threads or asyncs) the map(f) over v. Mode is 0 (block) or 1 (cyclic) scheduling.  To be completed by next Tue, March 22 (ideally).

## Assignment 3 (March 22): 

Implement in parallel the odd even sort. Use as input a random generated integer vector. The odd even sort algorithm looks like the following (this is sequential; n is the length of the vector, v is the vector to sort, swap is a procedure that exchanges the two positions in the vector):

while(not(sorted)) {
  sorted = true;
  for(i=1; i<n-1; i+=2)
    if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
  for(i = 0; i<n-1; i+=2)
    if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
}

with the intent to discuss scalability of the code.
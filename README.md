# network-percolation
It calculates the reduced average cluster size as well as the size of the three largest clusters in the addition of each link 

In order to use this Class you have to initialise it at first:  DynCreation myClass;

Next, you call the function mProcess that takes two arguments, the two nodes from a link: myClass.mProcess( node1, node2 );
If you want to print the current state, you can call the printState function: myClass.printState();
For printing in a file, printState can also take an ofstream file as an argument.

What is printed is the size of the three largest clusters, the reduced average cluster size, the number of nodes and links each time as well as the link inserted. These are useful to identify the link after whick the network is percolating, usually when the largest reduction of the value of the reduced average cluster size occurs.


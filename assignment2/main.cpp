#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
#include<cstdlib>
#include<math.h>
#include<time.h>


using namespace std;


void heapify(double arr[], int n, int i)
{
    int smallest = i; // Initialize smallest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
 
    // If left is smaller than smallest (root)
    if (l < n && arr[l] < arr[smallest])
        smallest = l;
 
    // If right is smaller than smallest so far
    if (r < n && arr[r] < arr[smallest])
        smallest = r;
 
    // If smallest is not root
    if (smallest != i) {
        swap(arr[i], arr[smallest]); //Swap the elements
 
        // Recursively heapify the sub-tree
        heapify(arr, n, smallest);
    }
}


void buildHeap(double arr[], int n){
	//Start from the last node that has child and go to the root
	for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
}


void heapSort(double arr[], int n)
{
    // Build heap
    buildHeap(arr, n);
 
    // One by one extract elements from the heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);
 
        // call heapify on the reduced heap
        heapify(arr, i, 0);
    }
}



int main(int argc, char* argv[]){
	
	ifstream file;
	file.open("locations.txt");
	
	if (!file){
		cerr << "File cannot be opened!";
		return 1;
	}
	
	static double hotel_longitude = 33.40819; //hotel locations
	static double hotel_latitude = 39.19001;
	
	
	int m; // you should read value of m from command line
	sscanf(argv[1], "%d", &m); // converting command line parameter to integer
	
	float p; // you should read value of p from command line
	sscanf(argv[2], "%f", &p); // converting command line parameter to float
	
	
	string line;
	getline(file, line); //this is the header line
	
	double taxis[100000][2]; //array for taxis location
	double distances[100000]; // array for taxi distances to from the hotel
	int taxiAdditions = 0;  //number of addition operations
	int distanceUpdates = 0;  //number of update operations
	int totalTaxis = 0;  //number of taxis that are in the heap
	
	double calledTaxis[10000]; //array for the called taxis
	int taxisTaken = 0;  //number of taxi calls
	
	int operations = 0;  //number of operations
	
	
	clock_t t = clock(); //starts the timer
	
	
	//reading from file if initial array is not empty
	for(int i = 0; i < totalTaxis; i++){
		
		getline(file, line, ' '); 
		taxis[i][0] = ::strtod(line.c_str(), 0);
		
		getline(file, line);
		taxis[i][1] = ::strtod(line.c_str(), 0);
		
		distances[i] = pow( pow((hotel_longitude - taxis[i][0]), 2) + pow((hotel_latitude - taxis[i][1]), 2), 0.5 ); //euclidian distance
 		
		//cout << distances[i] << endl;
		
	}

	
	
	srand(time(NULL)); //seed for random function
	while(m > 0){
		
		operations++;
		m--;
		
		if(operations == 100){ //call
			
			if(totalTaxis > 0){ //do nothing if the array is empty
				
				heapSort(distances, totalTaxis);  //sort the array 
				calledTaxis[taxisTaken] = distances[totalTaxis-1]; //smallest element is "removed" from the array and added to the calledTaxis array
			
				operations = 0;
				totalTaxis--;  //smallest element is "removed"
				taxisTaken++;  //that element is added to the new array
			
			}
			
			buildHeap(distances, totalTaxis);  //build the heap
			
		}
		
		float rng = (float) (rand() % 100 + 1) / 100; //random generated number for probability check
		
		//cout << rng << " rng" << endl;
		
		if (rng <= p){ //update
			
			if(totalTaxis > 0){
				
				int rngIndex = (rand() % totalTaxis);  //randomly choosing the index
				distances[rngIndex] -= 0.01;   //updating
				if(distances[rngIndex] < 0){   //if it gets smaller than 0
					distances[rngIndex] = 0;
				}
				
				distanceUpdates++;
				
				buildHeap(distances, totalTaxis);  //build the heap
				
			}
			
		} else { //addition
			
			getline(file, line, ' '); 
			taxis[totalTaxis][0] = ::strtod(line.c_str(), 0);
		
			getline(file, line);
			taxis[totalTaxis][1] = ::strtod(line.c_str(), 0);
		
			distances[totalTaxis] = pow( pow((hotel_longitude - taxis[totalTaxis][0]), 2) + pow((hotel_latitude - taxis[totalTaxis][1]), 2), 0.5 );
	
			//cout << distances[totalTaxis] << endl;
			
			taxiAdditions++;
			totalTaxis++;  //number of taxis increases
			
			buildHeap(distances, totalTaxis);  //build the heap
						
		}
		
	}
	
	
	cout << "Distances of the called taxis: " << endl;
	for(int i = 0; i < taxisTaken; i++){
		cout<<calledTaxis[i]<<endl;
	}
	cout << endl;
	cout << "The number of taxi additions: " << taxiAdditions << endl; 
	cout << "the number of distance updates: " << distanceUpdates << endl;
	cout << endl;
	t = clock() - t;
	printf("It took me %ld clicks (%f miliseconds).\n",t,((float)t)/CLOCKS_PER_SEC * 1000);
	
	
	return 0;
}

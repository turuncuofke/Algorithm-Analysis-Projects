#include<iostream>
#include<fstream>
#include<string>
#include<time.h>


using namespace std;

struct product{  //structure for storing the data
	string country;
	string itemType;
	string orderId;
	int unitsSold;
	float totalProfit;
};


//swap function to use in quicksort
void swap(struct product* a, struct product* b) 
{ 
    struct product temp; 
    
    temp = *a;
    *a = *b; 
    *b = temp; 
} 



int partition (struct product products[], int low, int high) 
{ 
    struct product pivot = products[high];    // pivot is chosen as the last element
    int i = (low - 1);  // index of smaller element 
  
    for (int j = low; j <= high - 1; j++) 
    { 
        if (products[j].country == pivot.country){ // checks if country names are the same
			if (products[j].totalProfit >= pivot.totalProfit){ // compares the total profit if country names are the same
				i++;   // increment index of smaller element
				swap(&products[i], &products[j]);  // swaps the places of products according to descending total profit order
			}
		}
        else if (products[j].country < pivot.country) // compares the country names
        { 
            i++;    // increment index of smaller element 
            swap(&products[i], &products[j]);  // swaps according to the alphabetical order
        }
    } 
    swap(&products[i + 1], &products[high]);  // puts pivot in the correct place
    return (i + 1);   // returns the index
} 
  

void quickSort(struct product products[], int low, int high) 
{ 
    if (low < high) 
    { 
        //pivot index
        int pivotIndex = partition(products, low, high); 
  
  
        // Separately sort elements before and after pivot index 
        quickSort(products, low, pivotIndex - 1); 
        quickSort(products, pivotIndex + 1, high); 
    } 
} 



int main(int argc, char* argv[]){
	
	ifstream file;
	file.open("sales.txt");
	
	if (!file){
		cerr << "File cannot be opened!";
		return 1;
	}
	
	
	int N; // you should read value of N from command line
	sscanf(argv[1], "%d", &N); // converting command line parameter to integer
		
	struct product *products;
	products = new struct product[N]; // allocating memory according to N variable
	
	string line;
	getline(file, line); //this is the header line
	
	
	
	//reading from file
	for(int i = 0; i < N; i++){
		
		getline(file, products[i].country, '\t'); //country (string)
		getline(file, products[i].itemType, '\t'); //item type (string)
		getline(file, products[i].orderId, '\t'); //order id (string)
		file >> products[i].unitsSold; //units sold (integer)
		file >> products[i].totalProfit; //total profit (float)
	    getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
	    
	    //cout<<products[i].country<<"\t"<<products[i].totalProfit<<endl;
	}
	
	
	
	//sorting starts
	clock_t t = clock(); //starts the timer
	
	quickSort(products, 0, N -1); 
	
	
	
	t = clock() - t;
	printf("It took me %ld clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
	//sorting ends
	
	
	
	//writing into file
	ofstream outfile;
	outfile.open("sorted.txt");
	
	
	for(int i = 0; i < N; i++){
		//cout<<products[i].country<<"  "<<products[i].totalProfit<<endl;
		outfile << products[i].country << "\t" << products[i].itemType << "\t" << products[i].orderId << "\t" << products[i].unitsSold << "\t" << products[i].totalProfit << "\t" << "\n" ;
	}
	
	
	
	delete [] products; //release the memory allocated
	
	return 0;
}

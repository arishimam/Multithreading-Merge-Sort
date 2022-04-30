#include <time.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <algorithm>
#include <cstdlib>

using namespace std;

//count of num to be sorted = n
//upper limit on range of numbers = u
// number of segments to divide into = p
//populate dynamic array "arr" with random values

vector<int> arr;
vector<int> temp_arr;

pthread_mutex_t mutex;
pthread_mutex_t mutex2;

int n;
int u;
int p;

int step;
int last_segment;

int num_threads;

void* thread_sorter(void* arg)
{
  int index = *(int*)arg;
  int beg = 0;
  int end = 0;

  if(index < p -1)
  {
    beg = index * step;
    end = beg + step ;//-1;
  }

  else
  {
    beg = index * step;
    end = beg + last_segment ;//- 1;
  }

  sort(arr.begin()+ beg, arr.begin() + end );//+ 1);

  pthread_mutex_lock(&mutex);

  cout << "Thread " << index + 1 << endl;
  //cout << "beg = " << beg << " end = " << end << endl;

  cout << "Sorted Values: ";
  for (int i = beg; i < end; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;

  pthread_mutex_unlock(&mutex);

  free(arg);

  return NULL;
}

void* thread_merger(void* arg)
{
  int index2 = *(int*)arg;
  int beg1 = 0;
  int end1 = 0;
  int beg2 = 0;
  int end2 = 0;

  if ((index2 == p/2 - 1) && (p%2 == 0) )
  {
    beg1 = index2 * step * 2;
    end1 = beg1 + step;
    beg2 = end1;
    end2 = beg2 + last_segment;
    last_segment = last_segment + step;
  }
  else
  {
    beg1 = index2 * step * 2;
    end1 = beg1 + step;
    beg2 = end1;
    end2 = beg2 + step;
  }

  merge(arr.begin() + beg1, arr.begin() + end1, arr.begin() + beg2, arr.begin() + end2, temp_arr.begin()+beg1);

  pthread_mutex_lock(&mutex2);

  cout << "Thread " << index2 + 1 << endl;
  // cout << "beg1 = " << beg1 << " end1 = " << end1;
  // cout << " beg2 = " << beg2 << " end2 = " << end2 << endl;

  cout << "Merged " << (end1 - beg1) << " and " << (end2 - beg2) << " numbers." << endl;

  for(int i = beg1; i < end2; i++)
  {
   cout << temp_arr[i] << " ";
  }
  cout << endl << endl;

  pthread_mutex_unlock(&mutex2);

  return NULL;
}

//Goal 1
int main(int argc, char* argv[])
{
  // adds arguments from command line to variables and converts to int
  n = atoi(argv[1]);
  u = atoi(argv[2]);
  p = atoi(argv[3]);

  cout << endl << "n = " << n << "  u = " << u << "  p = " << p << endl;


  //the size of the different segments we will be working with
  step = n/p;
  last_segment = n - ((p-1) * step);

  //cout << "step = " << step << " and last_segment = " << last_segment << endl;
  cout << endl;

  //Goal 2
  // Filling array with random nums
  //seeds the rand function by time so we generate different numbers every run
  srand(time(0));

  //reserves enough space for n numbers in arr
  arr.reserve(n);
  // Fills arr with n random numbers within a range of 1 - u (inclusive)
  for (int i = 0; i < n; i++)
  {
    arr.push_back((rand() % u + 1));
  }
  cout << "Original Array: " << endl << endl;
  //prints the populated array
  for (int i = 0; i < n; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl << endl;


  //Goal 3
  //Sorting

  pthread_t threads[p];
  pthread_mutex_init(&mutex, NULL);

  cout << "Sorting Threads: " << endl << endl;
  for (int i = 0; i < p; i++)
  {
    int* a = (int*)malloc(sizeof(int));
    *a = i;
    //insert thread creations
    pthread_create(&threads[i], NULL, &thread_sorter, a);
  }

  for (int i = 0; i < p; i++)
  {
    //joining threads
    pthread_join(threads[i], NULL);
  }

    cout << endl;
  //Goal 4
  //Merging

  // Reserves space for n values in temp_arr
  temp_arr.reserve(n);

  cout << "Merging Threads: " << endl << endl;
  while(p>1)
  {
    pthread_t threads2[p/2];

    cout << "Number of threads = " << p/2 << endl << endl;

    for (int i = 0; i < p/2; i++)
    {
      int* b = (int*)malloc(sizeof(int));
      *b = i;

      pthread_create(&threads2[i], NULL, &thread_merger, b);
    }

    for (int i = 0; i < p/2; i++)
    {
      pthread_join(threads2[i], NULL);
    }

    // if else copies merged values from temp_arr to arr
    if (p%2 == 0)
    {
      copy(temp_arr.begin(), temp_arr.begin() + n, arr.begin());
    }
    else
    {
      copy(temp_arr.begin(), temp_arr.begin() + (step * (p-1)), arr.begin());
    }

    cout<<endl;
    step = step * 2;
    p = p - p/2;
  }

  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex2);

  //Clear temp_arr
  temp_arr.clear();


  cout << endl << "Final Sorted and Merged Array: " << endl;

  for(int i = 0; i<n; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;

  return 0;
}

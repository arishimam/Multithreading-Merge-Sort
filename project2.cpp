
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
  for (int i = beg; i <= end; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;

  pthread_mutex_unlock(&mutex);

  free(arg);
}

void* thread_merger(void* arg)
{
  int index2 = *(int*)arg;
  int beg1 = 0;
  int end1 = 0;
  int beg2 = 0;
  int end2 = 0;

  if (index2 < num_threads - 1)
  {
    beg1 = index2 * step * 2;
    end1 = beg1 + step;
    beg2 = end1;
    end2 = beg2 + step;
  }
  else
  {
    beg1 = index2 * step * 2;
    end1 = beg1 + last_segment/2;
    beg2 = end1;
    end2 = n;
  }

  vector<int> temp_arr;

  //merge(arr.begin() + beg1, arr.begin() + end1, arr.begin() + beg2, arr.begin() + end2, temp_arr);

  // for(int i = 0; i < end2*2; i++)
  // {
  //  cout << temp_arr[i] << " ";
  // }

  pthread_mutex_lock(&mutex2);

  cout << "Thread " << index2 + 1 << endl;
  cout << "beg1 = " << beg1 << " end1 = " << end1;
  cout << " beg2 = " << beg2 << " end2 = " << end2 << endl;

  pthread_mutex_unlock(&mutex2);

}

//Goal 1
int main(int argc, char* argv[])
{

  n = atoi(argv[1]);
  u = atoi(argv[2]);
  p = atoi(argv[3]);


  printf("n = %d , u = %d , p = %d \n", n, u, p);

  step = n/p;
  last_segment = n - ((p-1) * step);

  cout << "step = " << step << " and last_segment = " << last_segment << endl;


  //Goal 2
  // Filling array with random nums
  srand(time(0));

  for (int i = 0; i < n; i++)
  {
    arr.push_back(rand() % u);
  }

  for (int i = 0; i < n; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl;


  //Goal 3
  //Sorting

  pthread_t threads[p];
  pthread_mutex_init(&mutex, NULL);

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

  //Goal 4
  //Merging
  //int num_threads = 0;
  if(p % 2 == 0) {num_threads = p/2;}
  else {num_threads = p/2 + 1;}

  pthread_t threads2[num_threads];

  cout << "num_threads = " << num_threads << endl;

  for (int i = 0; i < num_threads; i++)
  {
    int* b = (int*)malloc(sizeof(int));
    *b = i;

    pthread_create(&threads2[i], NULL, &thread_merger, b);
  }

  for (int i = 0; i < num_threads; i++)
  {
    pthread_join(threads2[i], NULL);
  }






  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex2);
  return 0;
}

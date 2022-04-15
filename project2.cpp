
#include <time.h>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <algorithm>
//#include <cstdlib>

using namespace std;

//count of num to be sorted = n
//upper limit on range of numbers = u
// number of segments to divide into = p
//populate dynamic array "arr" with random values

vector<int> arr;

pthread_mutex_t mutex;

int n;
int u;
int p;
int step;
int last_step;



void* thread_sorter(void* arg)
{
  pthread_mutex_lock(&mutex);
  int beg = *(int*)arg * step;
  int end = beg + step;


  cout << "beg = " << beg << " end = " << end << endl;
  pthread_mutex_unlock(&mutex);

}

int main(int argc, char* argv[])
{

  n = atoi(argv[1]);
  u = atoi(argv[2]);
  p = atoi(argv[3]);

  printf("n = %d , u = %d , p = %d \n", n, u, p);

  step = n/p;
  last_step = n % step;

  if(last_step == 0) {last_step = step;}

  cout << "step = " << step << " and last_step = " << last_step << endl;



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


  pthread_t threads[p];
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < p; i++)
  {
    //insert thread creations
    pthread_create(&threads[i], NULL, &thread_sorter, &i);
  }

  for (int i = 0; i < p; i++)
  {
    //joining threads
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  return 0;
}

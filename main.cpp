#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std;

#define MAX_SZ 1000

int arr[MAX_SZ] , n;

void get_input(){
   scanf("%d",&n);
   for(int i=0;i<n;i++)
      scanf("%d",&arr[i]);
}

void do_the_merge(int l,int mid,int r){
     /// merges 2 sorted ranges a[l , mid] & a[l+1,r]
     vector<int>buffer;

     int i = l , j = mid+1;

     while(j <=r && i <= mid){
        if(arr[i] < arr[j]){
           buffer.push_back(arr[i++]);
        }
        else{
           buffer.push_back(arr[j++]);
        }
     }

     while(j<=r)buffer.push_back(arr[j++]);
     while(i<=mid)buffer.push_back(arr[i++]);

     for(int i=0;i<buffer.size();i++)
           arr[l+i] = buffer[i];

}
void *mergeSort_threads(void *args){
        /// the information sent with the thread is a pair<int,int>
        /// it indicates the range it should sort

        pair<int,int> current = *(pair<int,int>*)(args);

        /// base case
        if(current.first == current.second)
            return NULL;

        pthread_t lft , rgt;

        int mid = (current.first + current.second)/2;


        /// threads creation

        pthread_create(&lft , NULL , mergeSort_threads , (void *)new pair<int,int>(current.first,mid));
        pthread_create(&rgt , NULL , mergeSort_threads , (void *)new pair<int,int>(mid+1,current.second));


        pthread_join(lft,NULL);
        pthread_join(rgt,NULL);

        /// merging 2 sorted parts
        do_the_merge(current.first,mid,current.second);

        pthread_exit(&lft);
        pthread_exit(&rgt);

}

void display(){
    printf("The array after sorting \n\n");

    for(int i=0;i<n;i++)
         printf("%d ",arr[i]);
}
int main()
{

    get_input();

    pthread_t main_thread;
    pthread_create(&main_thread , NULL , mergeSort_threads , (void *)new pair<int,int>(0,n-1));

    pthread_join(main_thread,NULL);

    display();


    return 0;
}

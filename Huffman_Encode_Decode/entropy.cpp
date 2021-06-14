#include<bits/stdc++.h>
using namespace std;

#define CHAR_SIZE 356

long long count1[CHAR_SIZE];

double calculate()
{
  double sum = 0,retVal = 0;
  for(int i=0;i<CHAR_SIZE;i++)
    sum+= count1[i];

  for(int i=0;i<CHAR_SIZE;i++)
  {
    if(count1[i]!=0)
    {
      double p = (double )count1[i]/(double )sum;
      //cout<<p<<endl;
      retVal -= p*log2(p);
    }
  }
  return retVal;
}

int main()
{
  FILE *fp = fopen("input.txt","r");

  for(int i=0;i<CHAR_SIZE;i++)
    count1[i] = 0;

  while(fp!=NULL)
  {
    char c1;
    if( fscanf(fp,"%c",&c1)!=EOF)
    {
      //cout<<c1;
      count1[c1]++;
    }
    else
      break;
  }

  fclose(fp);
  cout<<calculate()<<endl;

}


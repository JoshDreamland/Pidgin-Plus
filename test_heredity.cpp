#include <map>
#include <typeinfo>
#include <iostream>
using namespace std;

class a
{
  protected:
  int x;
};

struct b: public a
{
  int y;
  int salami ()
  {
    x=y;
  }
};

struct c: public b
{
  int balogna ()
  {
    return x=y;
  }
};

int main()
{
  c d;
  cout << (typeid(map<int,int>::iterator).name);
  return d.salami();
}



struct timequeue
{
  struct node
  {
    GTimer *time;
    node* next;
    
    node(): next(NULL) 
    {
      time = g_timer_new();
      if (time != NULL)
      g_timer_start(time);
    }
    ~node()
    {
      g_timer_destroy(time);
    }
  } *first, *last;
  
  node *pushtime()
  {
    if (first == NULL)
      return first = last = new node();
    return last = last->next = new node();
  }
  double popwithreturn()
  {
    if (first == NULL)
      return -1;
    
    double v = g_timer_elapsed(first->time,NULL);
    node *nxt = first -> next;
    delete first;
    first = nxt;
    
    return v;
  }
  
  timequeue(): first(NULL), last(NULL) {}
  ~timequeue()
  {
    for (node *d = first; d != NULL; d = d->next)
      delete d;
  }
};
struct timestack
{
  struct node
  {
    GTimer *time;
    node* prev;
    
    node(node* p): prev(p) 
    {
      time = g_timer_new();
      if (time != NULL)
      g_timer_start(time);
    }
    ~node()
    {
      g_timer_destroy(time);
    }
  } *top;
  
  node *pushtime()
  {
    return top = new node(top);
  }
  double popwithreturn()
  {
    if (top == NULL)
      return -1;
    
    double v = g_timer_elapsed(top->time,NULL);
    node *prev = top->prev;
    delete top;
    top = prev;
    
    return v;
  }
  
  timestack(): top(NULL) {}
  ~timestack()
  {
    for (node *d = top; d != NULL; d = d->prev)
      delete d;
  }
};

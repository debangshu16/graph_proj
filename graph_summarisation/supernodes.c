#include "functions.h"
extern int no_of_supernodes;
void form_superedge_zero_bin(struct bucket *bin,graph *g,int countg)
{

	int i,j,temp;	
	struct node *bptr,*lptr;
	struct node *ptr; 
	for(i=0;i<countg;i++)
	{	
				
		j=i*11;
		ptr=bin->arr[j].head;
		form_supernodes(ptr,g);
		lptr=g->arr[g->load].link_supernode;
		while(lptr!=NULL)
		{
			bptr=g->arr[lptr->vertex].head;
			while(bptr!=NULL)
			{
				temp=cal_supernode_adjacency(bptr->vertex,g);
				if(temp==getno(g->arr[g->load].link_supernode))
				{
					if(!isPresent(g->arr[g->load].head,bptr->vertex))			// && g->arr[bptr->vertex].has_superedge==0)
						addedge(g,g->load,bptr->vertex);
				}
				bptr=bptr->next;
			}
			lptr=lptr->next;
		}
		 
		 
	}
}
void form_superedge_last3(struct bucket *bin,graph* g,int countg)
{
	struct node *ptr,*nptr;
	int i,j;
	float m=0,n=0;
	float M,N;
	
	for(i=0;i<countg;i++)
	{
		j=(i*11)+10;
		ptr=merge(bin->arr[j].head,bin->arr[j-1].head);
		ptr=merge(ptr,bin->arr[j-2].head);

		form_supernodes(ptr,g);
		if(!isPresent(g->arr[g->load].head,g->load))
			addedge(g,g->load,g->load);

		ptr=g->arr[g->load].link_supernode;
		n=(float)getno(g->arr[g->load].link_supernode);

		N=n*(n-1)/2.0;

		while(ptr!=NULL)
		{
			nptr=g->arr[g->load].link_supernode;
			while(nptr!=NULL)
			{
				if(isPresent(g->arr[ptr->vertex].head,nptr->vertex))
				m++;

				nptr=nptr->next;
			}
			ptr=ptr->next;
		}

		M=m/2.0;
		g->arr[g->load].error=(N-M)/N;

		m=0;
		n=0;
	}
}
void form_supernodes(struct node *head,graph *g)
{
	struct node *ptr=head;
	int cnt=0;
	
	struct link *q=(struct link *)malloc(sizeof(struct link));
	q->link=NULL;
	while(ptr!=NULL)
	{
		if(g->arr[ptr->vertex].comp_score==1)
		{
			cnt++;
			q->link=addlink(q->link,ptr->vertex);
					
		}
		ptr=ptr->next;
	}
	if(cnt>1)
	{
		if(g->load==g->size-1)
		{
			g->size=g->size*2;
			g->arr=(struct adjlist *)realloc(g->arr,(sizeof(struct adjlist)*g->size));
		}
		g->load++;
		g->arr[g->load].id=g->load;
		g->arr[g->load].is_supernode=1;
		g->arr[g->load].comp_score=0.0;
		g->arr[g->load].link_supernode=q->link;
		no_of_supernodes++;
	}

	q->link=NULL;
	

}
float calerror_bipartite(graph *g,int a,int b)
{
	int count=0;
	struct node *ptr=g->arr[a].link_supernode;
	struct node *nptr=g->arr[b].link_supernode;

	int m=getno(ptr);
	int n=getno(nptr);
	
	while(ptr!=NULL)
	{
		nptr=g->arr[b].link_supernode;
		while(nptr!=NULL)
		{
			if(!isPresent(g->arr[ptr->vertex].head,nptr->vertex))
				count++;
			nptr=nptr->next;
		}
		ptr=ptr->next;
	}
	return (float)count/(m*n);
}
bool bfscolor(struct node *group,graph *g,int source)
{
	struct node *ptr,*nptr; 
	g->arr[source].colour=RED;
	struct que *q=createque();
	enque(q,source);

	while(!isempty(q))
	{
		ptr=deque(q);
		nptr=g->arr[ptr->vertex].head;

		while(nptr!=NULL)
		{
			
			if(isPresent(group,nptr->vertex))
			{
				if(g->arr[nptr->vertex].colour == UNCOLOURED)
				{
					enque(q,nptr->vertex);
					if(g->arr[ptr->vertex].colour==RED)
						g->arr[nptr->vertex].colour=BLUE;
					else
						g->arr[nptr->vertex].colour=RED;
				}
				else if(g->arr[ptr->vertex].colour == g->arr[nptr->vertex].colour)
					return false;
			}
			nptr=nptr->next;

		}

	}
	
	return true;
}
int form_bipartite(struct bucket *bin,graph *g,int group_no)
{
	int i=group_no,j;
	
	struct node *ptr,*group=NULL,*nptr;
	struct node *snode1=NULL,*snode2=NULL;
	j=(i*11)+1;
	while(j<(i*11+8))
	{
		ptr=bin->arr[j].head;
		if(ptr!=NULL)
			group=merge(group,ptr);
		j++;
	}
		
	 
/*	ptr=group;
	bool status=true;
	while(ptr!=NULL && status==true)
	{
		if(g->arr[ptr->vertex].colour==UNCOLOURED)
			status=bfscolor(group,g,ptr->vertex);

		ptr=ptr->next;
	}*/

	ptr=group;
	
	
	if(group!=NULL && bfscolor(group,g,group->vertex))
	{
	
	
		while(ptr!=NULL)
		{
			if(g->arr[ptr->vertex].colour==RED)
			snode1=addlink(snode1,ptr->vertex);
			else if(g->arr[ptr->vertex].colour==BLUE)
			snode2=addlink(snode2,ptr->vertex);

			ptr=ptr->next;
		}
	
		struct link *q=(struct link *)malloc(sizeof(struct link));
		struct link *q2=(struct link *)malloc(sizeof(struct link));
		q->link=NULL;
		q2->link=NULL;

		ptr=snode1;

		int cnt=0;

		while(ptr!=NULL)
		{
			if(g->arr[ptr->vertex].comp_score==1)
			{
				cnt++;
				q->link=addlink(q->link,ptr->vertex);
					
			}
			ptr=ptr->next;
		}
		if(cnt>1)
		{
		
			if(g->load==g->size-1)
			{
				g->size=g->size*2;
				g->arr=(struct adjlist *)realloc(g->arr,(sizeof(struct adjlist)*g->size));
			}
			g->load++;
			g->arr[g->load].id=g->load;
			g->arr[g->load].is_supernode=1;
			g->arr[g->load].comp_score=0.0;
			g->arr[g->load].link_supernode=q->link;
			no_of_supernodes++;
		
		}

		 

		cnt=0;
		ptr=snode2;
		while(ptr!=NULL)
		{
			if(g->arr[ptr->vertex].comp_score==1)
			{
				cnt++;
				q2->link=addlink(q2->link,ptr->vertex);
					
			}
			ptr=ptr->next;
		}
		if(cnt>1)
		{
		
			if(g->load==g->size-1)
			{
				g->size=g->size*2;
				g->arr=(struct adjlist *)realloc(g->arr,(sizeof(struct adjlist)*g->size));
			}
			g->load++;
			g->arr[g->load].id=g->load;
			g->arr[g->load].is_supernode=1;
			g->arr[g->load].comp_score=0.0;
			g->arr[g->load].link_supernode=q2->link;
			no_of_supernodes++;
		}

		 
	
		if(getno(q->link)<=1 || getno(q2->link)<=1)
		return -1;

	}
	else
	return -1;
	
}
int cal_supernode_adjacency(int vertex,graph *g)
{
	int count=0;
	struct node *ptr=g->arr[g->load].link_supernode;
	while(ptr!=NULL)
	{
		if(isPresent(g->arr[ptr->vertex].head,vertex))
		count++;
		ptr=ptr->next;
	}

	return count;
}
	
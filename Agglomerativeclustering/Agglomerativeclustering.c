#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<errno.h>  
#include<limits.h> 
#include<string.h>
#include<float.h>

#define K 3
const int CHUNK_SIZE = 12;

struct datapoint
{
	int id;
	double x;
	double y;
}datapoint;

struct cluster
{
	struct datapoint *pt;	
	int cluster_items_count;
	int size;
}cluster;

#define square(x) (double)((x)*(x))

double distance(struct datapoint *d1 ,struct datapoint *d2)
{
	
	return sqrt(square(d2->x - d1->x) + square(d2->y - d1->y));
}

void initiate_cluster(struct cluster *cl, int size) 
{

    	if(size == 0)
	{
        	cl->pt = NULL;
    	}
	else
	{
        	cl->pt = (struct datapoint*) malloc(size * sizeof(struct datapoint));
    	}
	cl->cluster_items_count = 0;
    	cl->size = size;
}

struct cluster *cluster_size_realloc(struct cluster *cl, int newSize) 
{
    
	if (cl->size >= newSize)
        return cl;
	size_t size = sizeof(struct datapoint) * newSize;
	void *ar = realloc(cl->pt, size);
    	if (ar == NULL)
        return NULL;
	cl->pt = (struct datapoint *) ar;
    	cl->size = newSize;
     	return cl;

}
void append_cluster(struct cluster *cl, struct datapoint pt) 
{
   
 	if (cl->size >= cl->cluster_items_count)
	{
        	cl = cluster_size_realloc(cl, cl->size + CHUNK_SIZE);
    	}
    	cl->pt[cl->cluster_items_count].id = pt.id;
    	cl->pt[cl->cluster_items_count].x = pt.x;
    	cl->pt[cl->cluster_items_count].y = pt.y;
    	cl->cluster_items_count++;
	//printf("%d",cl->cluster_items_count);
}

int input_data_to_cluster(char *filename, struct cluster **clarr ,int count)
{

	struct cluster *cluster_arr = malloc(count * sizeof(struct cluster));
	struct datapoint Datapoint;
	struct cluster cl;
	int MaxChar = 20;
    	char line[MaxChar];
    	int i = 0;
	FILE *fp = fopen(filename, "r");
        while (fgets(line, MaxChar, fp) != NULL && i < count)
	{
      		sscanf(line,"%d %lf %lf",&Datapoint.id,&Datapoint.x,&Datapoint.y);
		initiate_cluster(&cl, 1);
        	append_cluster(&cl, Datapoint);
        	strcpy(line, "");
        	cluster_arr[i] = cl;
        	i++;
	}
	printf("loaded clusters: %d\n", i);
	*clarr = &cluster_arr[0];
    	fclose(fp);
	
}

double cluster_distance(struct cluster *cl1, struct cluster *cl2)
{
   
	double dist, r_dist = DBL_MAX;

    	for (int i = 0; i < cl1->cluster_items_count; i++)
	{
        	for (int j = 0; j < cl2->cluster_items_count; j++)
		{
            		dist = distance(&cl1->pt[i], &cl2->pt[j]);
            		if (dist < r_dist)
			{
                		r_dist = dist;
            		}
        	}
    	}
    	return r_dist;
}

void find_closest_neighbours(struct cluster *clarr, int n, int *cl1, int *cl2) 
{

	double dist, mindist = DBL_MAX;
	for (int i = 0; i < n; i++){
        	for (int j = 1; j < n; j++)
		{
			if (i == j) continue;
			dist = cluster_distance(&clarr[i], &clarr[j]);
			if (dist < mindist)
			{
                		*cl1 = i;
                		*cl2 = j;
                		mindist = dist;
            		}
        	}
    	}
    
}

void merge_clusters(struct cluster *c1, struct cluster *c2) 
{
   
    	for (int i = 0; i < c2->cluster_items_count; i++)
	{
        	append_cluster(c1, c2->pt[i]);
    	}
}

void empty_cluster(struct cluster *cl) 
{
  	free(cl->pt);
    	initiate_cluster(cl, 0);
}

int delete_cluster(struct cluster *clarr, int n, int id)
{
   
    	empty_cluster(&clarr[id]);

    	for (int i = id; i < (n - 1); i++)
	{
        	clarr[i] = clarr[i + 1];
    	}
	return 0;
}

void clusters_formation(struct cluster *clarr, int n) 
{
    	int closest1, closest2;

    	find_closest_neighbours(clarr, n, &closest1, &closest2);
    	
	merge_clusters(&clarr[closest1], &clarr[closest2]);
    		
	delete_cluster(clarr, n, closest2);
}

void print_cluster(struct cluster *cl,int cluster_num)
{
    
    	FILE*fptr;
       
	fptr = fopen("test.data","a+");
	if(fptr == NULL)
   	{
      		printf("Error!");   
      			exit(1);             
  	}
	for (int i = 0; i < cl->cluster_items_count; i++)
	{	
		fptr = fopen("plot1.data","a+");
       		if(fptr == NULL)
        	{
                	printf("Error!");
                        exit(1);
       	 	}	
		fprintf(fptr,"%g %g %d\n",cl->pt[i].x, cl->pt[i].y,cluster_num);
         	fclose(fptr);
		//printf("%d(%f %f)", cl->pt[i].id, cl->pt[i].x, cl->pt[i].y);
    	}
    
}

void print_clusters(struct cluster *clarr, int n)
{
    	printf("Clusters:\n");
    	for (int i = 0; i < n; i++)
	{
        	printf("cluster %d: ", i);
        	print_cluster(&clarr[i],i);
    	}
}


int main(int argc, char *argv[])
{
        if(argc!=3)
        {
                printf("Error");
                return 0;
        }
       
	// convert argv[2] value from string to int
	char* p;
	errno = 0; 
	long arg = strtol(argv[2], &p, 10);
	if (*p != '\0' || errno != 0) 
	{
    		return 1; 
	}
	if (arg < INT_MIN || arg > INT_MAX)
        {
    		return 1;
	}
        int clusters_count= arg;
	printf("%d", clusters_count);
        
	struct cluster *Clusters;
	
	//read datapoints from file to respective cluster
	input_data_to_cluster(argv[1],&Clusters,clusters_count);
	
        // Create 3 clusters using single linkage(minimum dist)
	while(clusters_count>K)
	{
		clusters_formation(Clusters, clusters_count);
        	clusters_count--;
    	}      
        
        //print clusters with data
	print_clusters(Clusters, clusters_count);

        //Empty the clusters 
	for (int i = 0; i < clusters_count; i++)
	{
        empty_cluster(&(Clusters[i]));
    	}

    	free(Clusters);

	return 0;
}
